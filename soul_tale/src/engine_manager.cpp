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

#include <exception>

#include <spdlog/spdlog.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include <allegro_tmx/allegro_tmx.hh>
#include <widgetz/widgetz.h>

#include <common/option_config.hh>
#include <in-arena/arena.hh>
#include <in-world/world.hh>

#include <engine_manager.hh>

namespace {

void check_instantiation(void* ptr, const std::string& component) {
  if (ptr == nullptr) {
    SPDLOG_ERROR("An error occurred at {}\n", component);
    throw std::runtime_error(component);
  }
}

}// namespace

namespace fys::st {

struct engine_manager::internal {

  ALLEGRO_TIMER* timer = nullptr;
  ALLEGRO_EVENT_QUEUE* queue = nullptr;
  ALLEGRO_DISPLAY* disp = nullptr;
  ALLEGRO_FONT* font = nullptr;

  std::unique_ptr<world> world = nullptr;
  std::unique_ptr<arena> arena = nullptr;

  [[nodiscard("inf loop if not used")]] bool
  execute_event(ALLEGRO_EVENT event, std::shared_ptr<network_manager>& net) const {

    if (arena != nullptr) {
      return arena->execute_event(event, net);
    }
    if (world != nullptr) {
      return world->execute_event(event, net);
    }
    SPDLOG_ERROR("Neither arena nor world is set in the engine");
    return false;
  }

  void execute_rendering() const {
    if (arena != nullptr) {
      arena->render();
    } else if (world != nullptr) {
      world->render();
    }
  }
};

engine_manager::~engine_manager() {

  al_destroy_timer(_internal->timer);
  al_destroy_event_queue(_internal->queue);
  al_destroy_display(_internal->disp);
  al_destroy_font(_internal->font);

  al_uninstall_keyboard();
  al_shutdown_font_addon();
  al_shutdown_image_addon();
  al_shutdown_primitives_addon();

  //  wz_destroy_skin_theme(&skin_theme);
  //  wz_destroy(gui);
}

engine_manager::engine_manager() : _internal(std::make_unique<internal>()) {
  if (!al_init()) {
    SPDLOG_ERROR("An error occurred at init\n");
    throw std::runtime_error("al_init");
  }
  _internal->timer = al_create_timer(config::refresh_rate);
  _internal->queue = al_create_event_queue();
  _internal->disp = al_create_display(config::disp_w, config::disp_h);
  _internal->font = al_create_builtin_font();

  check_instantiation(_internal->timer, "create_timer");
  check_instantiation(_internal->queue, "create_event_queue");
  check_instantiation(_internal->disp, "create_display");
  check_instantiation(_internal->font, "create_builtin_font");

  al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);

  al_install_keyboard();
  al_init_font_addon();
  al_init_image_addon();
  al_init_primitives_addon();

  al_register_event_source(_internal->queue, al_get_keyboard_event_source());
  al_register_event_source(_internal->queue, al_get_display_event_source(_internal->disp));
  al_register_event_source(_internal->queue, al_get_timer_event_source(_internal->timer));

  al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
}

void engine_manager::run(const std::string& user_name, std::shared_ptr<network_manager> net) {
  ALLEGRO_EVENT event;
  bool done = false;
  bool redraw = true;

  al_start_timer(_internal->timer);

  while (!done) {
    al_wait_for_event(_internal->queue, &event);

    done = _internal->execute_event(event, net);

    if (redraw && al_is_event_queue_empty(_internal->queue)) {
      _internal->execute_rendering();
      al_flip_display();
      redraw = false;
    }
  }
}

void engine_manager::run_launcher() {
}

}// namespace fys::st
