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

#ifndef SOUL_TALE_SOUL_TALE_SRC_PLAYER_CONTEXT_HH
#define SOUL_TALE_SOUL_TALE_SRC_PLAYER_CONTEXT_HH

#include <memory>

#include <allegro5/allegro5.h>
#include <engine_manager.hh>
#include <hud/terminal.hh>

#include "key_map.hh"
#include "option_config.hh"

namespace fys::st {

class game_context {
  friend hud::terminal;
  friend engine_manager;

public:
  static game_context& get() {
    static game_context ctx;
    return ctx;
  }

  //! key pressed (used for to detect which keys has been pressed
  unsigned char key[ALLEGRO_KEY_MAX]{};

  [[nodiscard]] const std::string& user_name() const { return _user_name; }
  [[nodiscard]] const key_map& get_key_map() const { return _key_map; }

  [[nodiscard]] int disp_x() const { return _display_x; }
  [[nodiscard]] int disp_y() const { return _display_y; }
  [[nodiscard]] float ratio() const { return _ratio; }

private:
  game_context() = default;

private:
  std::string _user_name;

  key_map _key_map{};

  option_config _config;

  int _display_x{0};
  int _display_y{0};
  float _ratio{0.5};
};

}// namespace fys::st

#endif//SOUL_TALE_SOUL_TALE_SRC_PLAYER_CONTEXT_HH
