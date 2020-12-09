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
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>

#include <allegro_tmx/allegro_tmx.hh>
#include <engine_manager.hh>

int main() {

  if (!al_init()) {
    std::cerr << "An error occurred at init\n";
    return 1;
  }

  al_install_keyboard();
  al_install_mouse();
  al_init_font_addon();
  al_init_ttf_addon();
  al_init_image_addon();
  al_init_primitives_addon();
  al_init_native_dialog_addon();

  al_set_app_name("Free Tales Online");

  al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);

  fys::st::engine_manager engine_manager{};
  engine_manager.run("test", nullptr);

  al_uninstall_keyboard();
  al_uninstall_mouse();
  al_shutdown_font_addon();
  al_shutdown_image_addon();
  al_shutdown_primitives_addon();
  al_shutdown_native_dialog_addon();
  return 0;
}