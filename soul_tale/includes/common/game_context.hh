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

#include "key_map.hh"
#include "option_config.hh"

namespace fys::st {

class game_context {
  friend ui::terminal;

public:
  static game_context& get() {
    static game_context ctx;
    return ctx;
  }

  [[nodiscard]] const std::string& user_name() const { return _user_name; }
  [[nodiscard]] const key_map& get_key_map() const { return _key_map; }

private:
  game_context() = default;

private:
  std::string _user_name;

  key_map _key_map;
  option_config _config;

};

}

#endif//SOUL_TALE_SOUL_TALE_SRC_PLAYER_CONTEXT_HH
