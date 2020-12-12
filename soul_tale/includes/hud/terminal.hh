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

#ifndef SOUL_TALE_SOUL_TALE_SRC_HUD_TERMINAL_HH
#define SOUL_TALE_SOUL_TALE_SRC_HUD_TERMINAL_HH

#include <concepts>

#include <widgetz/widgetz.h>

#include <fil/cli/command_line_interface.hh>

namespace fys::st::hud {

class terminal {
  struct internal;

  inline static std::unique_ptr<terminal> _instance = nullptr;

public:
  ~terminal();

  static terminal& get_instance(ALLEGRO_EVENT_QUEUE* event_queue);
  static void reset();
  static void add_to_terminal(const std::string& to_add);

  template<std::invocable Handler>
  static void add_command(std::string name_command, Handler&& handler, std::string help = "") {
    assert(_instance != nullptr);
    _instance->_cli.add_sub_command(fil::sub_command(std::move(name_command), std::forward<Handler>(handler), std::move(help)));
  }

  void execute_event(ALLEGRO_EVENT event);
  void render();

private:
  explicit terminal(ALLEGRO_EVENT_QUEUE* event_queue);

private:
  std::unique_ptr<internal> _intern;
  fil::command_line_interface _cli;
};

}// namespace fys::st::hud

#endif//SOUL_TALE_SOUL_TALE_SRC_HUD_TERMINAL_HH
