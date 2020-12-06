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

#include <allegro5/allegro5.h>

#include <common/key_map.hh>

namespace fys::st {

key_map key_map::from_config(const std::filesystem::path& path_to_config) {
  key_map m {};

  m.open_map = ALLEGRO_KEY_M;
  m.open_status = ALLEGRO_KEY_S;
  m.open_options = ALLEGRO_KEY_ESCAPE;

  m.open_dev_terminal = ALLEGRO_KEY_T;
  m.open_perf_monitor = ALLEGRO_KEY_P;

  m.move_down = ALLEGRO_KEY_DOWN;
  m.move_up = ALLEGRO_KEY_UP;
  m.move_left = ALLEGRO_KEY_LEFT;
  m.move_right = ALLEGRO_KEY_RIGHT;

  return m;
}

void key_map::save_to_config(const key_map& to_save, const std::filesystem::path& path_to_config) {
}

}