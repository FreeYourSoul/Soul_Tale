// MIT License
//
// Copyright (c) 2020 Quentin Balland
// Repository : https://github.com/FreeYourSoul/FyS
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
//         of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
//         to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//         copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
//         copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//         AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <spdlog/spdlog.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/transformations.h>

#include <common/game_context.hh>
#include <common/option_config.hh>
#include <in-arena/arena.hh>
#include <in-world/world.hh>

#include <engine_manager.hh>
#include <hud/hud_manager.hh>

namespace {

void check_instantiation(void* ptr, const std::string& component) {
  if (ptr == nullptr) {
    SPDLOG_ERROR("An error occurred at {}\n", component);
    throw std::runtime_error(component);
  }
}

bool is_mouse_event(ALLEGRO_EVENT event) {
  return event.type == ALLEGRO_EVENT_MOUSE_AXES
      || event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY
      || event.type == ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY
      || event.type == ALLEGRO_EVENT_MOUSE_WARPED
      || event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN
      || event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP;
}

//
// Used as bit mask for the key pressing
//
constexpr int KEY_SEEN = 1;
constexpr int KEY_RELEASED = 2;

}// namespace

namespace fys::st {

struct engine_manager::internal {

  ALLEGRO_TIMER* timer = nullptr;
  ALLEGRO_EVENT_QUEUE* queue = nullptr;
  ALLEGRO_DISPLAY* disp = nullptr;
  ALLEGRO_FONT* font = nullptr;

  std::unique_ptr<hud::hud_manager> hud;

  std::unique_ptr<world> world_instance = nullptr;
  std::unique_ptr<arena> arena_instance = nullptr;

  void setup_context() const {
    auto& ctx = game_context::get();
    ctx._ratio = .5f;
    ctx._display_x = al_get_display_width(disp);
    ctx._display_y = al_get_display_height(disp);
    ctx._key_map = key_map::from_config("");
  }

  void execute_event(ALLEGRO_EVENT, std::shared_ptr<network_manager>& net) const {

    if (arena_instance != nullptr) {
      arena_instance->execute_event(net);
      return;
    }
    if (world_instance != nullptr) {
      world_instance->execute_event(net);
      return;
    }
    SPDLOG_ERROR("Neither arena nor world is set in the engine");
  }

  void execute_rendering() const {
    if (arena_instance != nullptr) {
      arena_instance->render();
    } else if (world_instance != nullptr) {
      world_instance->render();
    }
    hud->render();
  }
};

engine_manager::~engine_manager() {
  al_destroy_timer(_internal->timer);
  al_destroy_event_queue(_internal->queue);
  al_destroy_display(_internal->disp);
  al_destroy_font(_internal->font);
}

engine_manager::engine_manager() : _internal(std::make_unique<internal>()) {

  _internal->timer = al_create_timer(config::refresh_rate);
  _internal->queue = al_create_event_queue();
  _internal->disp = al_create_display(config::disp_w, config::disp_h);
  _internal->font = al_create_builtin_font();

  _internal->setup_context();

  _internal->hud = std::make_unique<hud::hud_manager>(_internal->queue);

  check_instantiation(_internal->timer, "create_timer");
  check_instantiation(_internal->queue, "create_event_queue");
  check_instantiation(_internal->disp, "create_display");
  check_instantiation(_internal->font, "create_builtin_font");

  al_register_event_source(_internal->queue, al_get_keyboard_event_source());
  al_register_event_source(_internal->queue, al_get_mouse_event_source());
  al_register_event_source(_internal->queue, al_get_display_event_source(_internal->disp));
  al_register_event_source(_internal->queue, al_get_timer_event_source(_internal->timer));

  if (al_install_touch_input())
    al_register_event_source(_internal->queue, al_get_touch_input_event_source());

  // temporary map creation
  _internal->world_instance = std::make_unique<world>();
}

void engine_manager::run(const std::string& user_name, std::shared_ptr<network_manager> net) {
  ALLEGRO_EVENT event;
  bool done = false;
  bool redraw = true;

  al_start_timer(_internal->timer);

  while (!done) {
    al_wait_for_event(_internal->queue, &event);

    switch (auto& key = game_context::get().key; event.type) {
    case ALLEGRO_EVENT_TIMER:
      for (unsigned char& i : key) {
        i &= KEY_SEEN;
      }
      redraw = true;
      _internal->execute_event(event, net);
      _internal->hud->execute_event(event);
      break;

    // handle keys pressure
    case ALLEGRO_EVENT_KEY_DOWN:
      key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
      _internal->hud->execute_event(event);
      break;
    case ALLEGRO_EVENT_KEY_UP:
      key[event.keyboard.keycode] &= KEY_RELEASED;
      break;
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
      done = true;
      break;
    default:
      if (is_mouse_event(event)) {
        event.mouse.x = int(float(event.mouse.x) / game_context::get()._ratio);
        event.mouse.y = int(float(event.mouse.y) / game_context::get()._ratio);
      }
      _internal->hud->execute_event(event);
      break;
    }

    // rendering
    if (redraw && al_is_event_queue_empty(_internal->queue)) {
      _internal->execute_rendering();
      al_flip_display();
      redraw = false;
    }
    al_rest(0);
  }
}

void engine_manager::run_launcher() {
}

}// namespace fys::st
