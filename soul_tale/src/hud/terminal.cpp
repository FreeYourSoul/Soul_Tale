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

#include <fmt/format.h>

#include <common/game_context.hh>

#include <hud/terminal.hh>

namespace fys::st::hud {

struct terminal::internal {
  WZ_WIDGET* widget = nullptr;
  WZ_DEF_THEME theme;
  ALLEGRO_FONT* font;

  std::string terminal_content = fmt::format("----------------------------------------------\nTerminal Session : version 1.0.0-DEV\n");
};

terminal::~terminal() {
  wz_destroy(_intern->widget);
  al_destroy_font(_intern->font);
};

terminal::terminal(ALLEGRO_EVENT_QUEUE* event_queue) : _intern(std::make_unique<internal>()) {
  auto& ctx = game_context::get();

  _intern->font = al_load_font("/home/FyS/Project/Soul_Tale/asset/font/vera_mono.ttf", 30, 0);

  memset(&_intern->theme, 0, sizeof(_intern->theme));
  memcpy(&_intern->theme, &wz_def_theme, sizeof(_intern->theme));
  _intern->theme.font = _intern->font;
  _intern->theme.color1 = al_map_rgba_f(0.1, 0.1, 0.2, 0.9);
  _intern->theme.color2 = al_map_rgba_f(1.0, 1.0, 1.0, 1);

  _intern->widget = wz_create_widget(nullptr, 0, 0, -1);

  wz_set_theme(_intern->widget, (WZ_THEME*)&_intern->theme);

  wz_create_fill_layout(_intern->widget,
                        0, 0,
                        float(ctx._display_x), float(ctx._display_y) + 130.f,
                        20, 20, WZ_ALIGN_LEFT, WZ_ALIGN_TOP, -1);

  wz_create_textbox(_intern->widget,
                    0, 0,
                    float(ctx._display_x), float(ctx._display_y),
                    WZ_ALIGN_LEFT,
                    WZ_ALIGN_TOP,
                    al_ustr_new(_intern->terminal_content.c_str()), 1, -1);

  (WZ_WIDGET*)wz_create_editbox(_intern->widget,
                                0, 0,
                                float(ctx._display_x) - 40, 70,
                                al_ustr_new("Type here.."), 1, -1);

  wz_register_sources(_intern->widget, event_queue);
}

void terminal::render() {
  wz_draw(_intern->widget);
}

void terminal::execute_event(ALLEGRO_EVENT event) {
  wz_update(_intern->widget, 1.0f / 60.f);
  wz_send_event(_intern->widget, &event);
}

}// namespace fys::st::hud
