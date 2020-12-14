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

#include <algorithm>

#include <allegro5/allegro_native_dialog.h>

#include <common/game_context.hh>
#include <spdlog/spdlog.h>

#include <fil/algorithm/string.hh>

#include <hud/terminal.hh>

namespace {
constexpr std::uint32_t TERM_MAX_WIDTH = 74;
constexpr std::uint32_t TERM_MAX_HEIGHT = 22;
constexpr std::uint32_t TERM_MAX_HISTORY = 200;

std::uint32_t normalize(std::uint32_t index) {
  return index % TERM_MAX_HISTORY;
}

fil::command_line_interface make_cli() {
  fil::command_line_interface cli([] {}, "FyS Online Command line tool");

  cli.set_sub_command_only(true);
  return cli;
}

}// namespace

namespace fys::st::hud {

struct terminal::internal {
  WZ_WIDGET* widget = nullptr;
  WZ_TEXTBOX* terminal = nullptr;
  WZ_WIDGET* cml = nullptr;
  ALLEGRO_FONT* font = nullptr;

  WZ_DEF_THEME theme{};

  //! terminal history used to print
  std::vector<std::string> terminal_history{
      "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
      "Terminal Session : version 1.0.0-DEV"};

  //! Latest element seen, if set to 0, the most recent is shown as last, if 1 the previous last, and so on...
  std::uint32_t start_print_index = 0;

  //! index at which an overwrite of the terminal will occurs
  std::optional<std::uint32_t> overwrite_index;

  bool refresh_text = false;

  std::string make_terminal_print() {
    std::string result;
    std::uint32_t begin = normalize(overwrite_index.value_or(0) + start_print_index);

    for (std::uint32_t i = begin; i < TERM_MAX_HEIGHT; ++i) {
      if (i < terminal_history.size()) {
        result = result.append(terminal_history.at(i));
      }
      result = result.append("\n");
    }
    return result;
  }

  void print_in_terminal(const std::string& in_terminal) {
    fil::split_string(
        in_terminal, "\n", [this](std::string to_add) {
          if (terminal_history.size() <= TERM_MAX_HEIGHT) {
            terminal_history.emplace_back(std::move(to_add));
          } else {
            overwrite_index = overwrite_index.has_value() ? 0 : normalize(overwrite_index.value() + 1);
            assert(overwrite_index.value() < TERM_MAX_HEIGHT);
            terminal_history[overwrite_index.value()] = std::move(to_add);
          }
        },
        TERM_MAX_WIDTH);
    refresh_text = true;
  }
};

terminal::~terminal() {
  wz_destroy(_intern->cml);
  wz_destroy((WZ_WIDGET*)_intern->terminal);
  wz_destroy(_intern->widget);
  al_destroy_font(_intern->font);
};

terminal::terminal(ALLEGRO_EVENT_QUEUE* event_queue) : _intern(std::make_unique<internal>()), _cli(make_cli) {
  auto& ctx = game_context::get();

  _intern->font = al_load_font("/home/FyS/Project/Soul_Tale/asset/font/vera_mono.ttf", 30, 0);

  memset(&_intern->theme, 0, sizeof(_intern->theme));
  memcpy(&_intern->theme, &wz_def_theme, sizeof(_intern->theme));
  _intern->theme.font = _intern->font;
  _intern->theme.color1 = al_map_rgba_f(0.1, 0.1, 0.2, 0.9);
  _intern->theme.color2 = al_map_rgba_f(1.0, 1.0, 1.0, 1);

  _intern->widget = wz_create_widget(nullptr, 0, 0, -1);

  wz_set_theme(_intern->widget, (WZ_THEME*)&_intern->theme);

  wz_create_fill_layout(
      _intern->widget,
      0, 0, float(ctx._display_x), 100,
      0, 0, WZ_ALIGN_CENTRE, WZ_ALIGN_CENTRE, -1);

  _intern->cml = (WZ_WIDGET*)wz_create_editbox(
      _intern->widget,
      0, 0,
      float(ctx._display_x) - 120, 80,
      al_ustr_new(""), 1, 42);

  WZ_BUTTON* send = wz_create_button(_intern->widget, 0, 0, 80, 80, al_ustr_new("SEND"), 1, 666);
  wz_set_shortcut(reinterpret_cast<WZ_WIDGET*>(send), ALLEGRO_KEY_ENTER, 0);

  wz_create_fill_layout(
      _intern->widget,
      0, 100,
      float(ctx._display_x), float(ctx._display_y) + 130.f,
      20, 20, WZ_ALIGN_LEFT, WZ_ALIGN_TOP, -1);

  _intern->terminal = wz_create_textbox(
      _intern->widget,
      0, 0,
      float(ctx._display_x), float(ctx._display_y),
      WZ_ALIGN_LEFT,
      WZ_ALIGN_TOP,
      al_ustr_new(_intern->make_terminal_print().c_str()), 1, -1);

  wz_register_sources(_intern->widget, event_queue);
}

void terminal::render() {
  wz_update(_intern->widget, config::refresh_rate);
  if (_intern->refresh_text) {
    al_ustr_free(_intern->terminal->text);
    _intern->terminal->text = al_ustr_new(_intern->make_terminal_print().c_str());
  }
  wz_draw(_intern->widget);
}

void terminal::execute_event(ALLEGRO_EVENT event) {
  wz_send_event(_intern->widget, &event);
  switch (event.type) {
  case WZ_BUTTON_PRESSED: {
    switch ((int)event.user.data1) {
    case 666: {
      auto* edit_box_content = ((WZ_EDITBOX*)_intern->cml);
      std::string command = std::string("$> ").append(std::string((char*)edit_box_content->text->data, edit_box_content->text->slen));

      // execute command
      std::vector<std::string> ac_str{};
      fil::split_string(command, " ", [&ac_str](std::string s) { ac_str.emplace_back(std::move(s)); });
      std::vector<char*> ac;
      ac.reserve(ac_str.size());
      std::ranges::transform(ac_str, std::back_inserter(ac), [](const std::string& str) { return const_cast<char*>(str.c_str()); });

      _intern->print_in_terminal(command);
      if (!ac.empty()) {
        bool action = false;
        try {
          action = _cli.parse_command_line(ac.size(), &ac[0]);
        } catch (...) {}
        if (!action) {
          _intern->print_in_terminal("... : wrong command");
        }
      }

      // refresh command line
      al_ustr_free(edit_box_content->text);
      edit_box_content->text = al_ustr_new("");
    }
    }
    break;
  }
  }
}

terminal& terminal::get_instance(ALLEGRO_EVENT_QUEUE* event_queue) {
  if (_instance == nullptr) {
    _instance.reset(new terminal(event_queue));
  }
  return *_instance;
}

void terminal::reset() {
  _instance.reset(nullptr);
}

void terminal::add_to_terminal(const std::string& to_add) {
  _instance->_intern->print_in_terminal(to_add);
}

}// namespace fys::st::hud
