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

#include <functional>

#include <widgetz/widgetz.h>

#include <common/game_context.hh>
#include <hud/hud_manager.hh>
#include <hud/terminal.hh>

namespace fys::st::hud {

struct hud_manager::internal {

  explicit internal(ALLEGRO_EVENT_QUEUE* event_queue)
      : terminal_hud(event_queue) {

    registered_hud = {

        // terminal hud
        [this](std::optional<ALLEGRO_EVENT> event) mutable {
          if (!terminal_enabled) {
            if (event.has_value()) {
              terminal_hud.execute_event(event.value());
            } else {
              terminal_hud.render();
            }
          }
        },

    };
  }

  hud::terminal terminal_hud;

  std::vector<std::function<void(std::optional<ALLEGRO_EVENT>)>> registered_hud;

  bool terminal_enabled = false;
  void enable_dev_terminal() {
    terminal_enabled = !terminal_enabled;
  }
};

hud_manager::~hud_manager() = default;

hud_manager::hud_manager(ALLEGRO_EVENT_QUEUE* event_queue)
    : _intern(std::make_unique<internal>(event_queue)) {
}

void hud_manager::execute_event(ALLEGRO_EVENT event) {

  if (event.type == game_context::get().get_key_map().open_dev_terminal) {
    _intern->enable_dev_terminal();
  }
  for (auto& hud : _intern->registered_hud) {
    hud(event);
  }
}

void hud_manager::render() {
  for (auto& hud : _intern->registered_hud) {
    hud(std::nullopt);
  }
}

}// namespace fys::st::hud
