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

#include <allegro_tmx/allegro_tmx.hh>
#include <widgetz/widgetz.h>

static constexpr std::uint32_t DISP_W = 620;
static constexpr std::uint32_t DISP_H = 480;

static constexpr int KEY_SEEN = 1;
static constexpr int KEY_RELEASED = 2;

int main(int ac, char** av) {

  if (!al_init()) {
    std::cerr << "An error occurred at init\n";
    return 1;
  }
  al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);

  al_install_keyboard();
  al_init_image_addon();
  al_init_primitives_addon();

  double refresh_rate = 1.0 / 60.0;
  ALLEGRO_TIMER* timer = al_create_timer(refresh_rate);
  ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
  ALLEGRO_DISPLAY* disp = al_create_display(DISP_W, DISP_H);
  ALLEGRO_FONT* font = al_create_builtin_font();

  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_timer_event_source(timer));

  al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

  ALLEGRO_EVENT event;

  al_start_timer(timer);

  // widget HUD
  WZ_WIDGET* gui;
  WZ_DEF_THEME theme;
  WZ_SKIN_THEME skin_theme;
  WZ_WIDGET* wgt;

  //al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
  /*
  Define a custom skin theme
  wz_skin_theme is a global vtable defined by the header
  */
  memset(&skin_theme, 0, sizeof(skin_theme));
  memcpy(&skin_theme, &wz_skin_theme, sizeof(skin_theme));
  skin_theme.theme.font = font;
  skin_theme.theme.color1 = al_map_rgba_f(0.1, 0.1, 0.9, 1);
  skin_theme.theme.color2 = al_map_rgba_f(1, 0.6, 0.1, 1);
  skin_theme.button_up_bitmap = al_load_bitmap("data/button_up.png");
  skin_theme.button_down_bitmap = al_load_bitmap("data/button_down.png");
  skin_theme.box_bitmap = al_load_bitmap("data/box.png");
  skin_theme.editbox_bitmap = al_load_bitmap("data/editbox.png");
  skin_theme.scroll_track_bitmap = al_load_bitmap("data/scroll_track.png");
  skin_theme.slider_bitmap = al_load_bitmap("data/slider.png");

  gui = wz_create_widget(nullptr, 0, 0, -1);

  wz_set_theme(gui, (WZ_THEME*)&skin_theme);
  /*
  Define all other gui elements, fill_layout is an automatic layout container
  */
  wz_create_fill_layout(gui, 50, 50, 300, 450, 50, 20, WZ_ALIGN_CENTRE, WZ_ALIGN_TOP, -1);
  wz_create_textbox(gui, 0, 0, 200, 50, WZ_ALIGN_CENTRE, WZ_ALIGN_CENTRE, al_ustr_new("Welcome to WidgetZ!"), 1, -1);
  wz_create_toggle_button(gui, 0, 0, 200, 50, al_ustr_new("Toggle 1"), 1, 1, 5);
  wz_create_toggle_button(gui, 0, 0, 200, 50, al_ustr_new("Toggle 2"), 1, 1, 6);
  wz_create_toggle_button(gui, 0, 0, 200, 50, al_ustr_new("Toggle 3"), 1, 1, 7);
  wz_create_button(gui, 0, 0, 200, 50, al_ustr_new("Switch Themes"), 1, 666);
  wgt = (WZ_WIDGET*)wz_create_button(gui, 0, 0, 200, 50, al_ustr_new("Quit"), 1, 1);
  wz_set_shortcut(wgt, ALLEGRO_KEY_Q, ALLEGRO_KEYMOD_CTRL);
  wz_create_fill_layout(gui, 350, 50, 300, 450, 50, 20, WZ_ALIGN_CENTRE, WZ_ALIGN_TOP, -1);
  wz_create_textbox(gui, 0, 0, 200, 50, WZ_ALIGN_CENTRE, WZ_ALIGN_CENTRE, al_ustr_new("Scroll Bars:"), 1, -1);
  wz_create_scroll(gui, 0, 0, 200, 20, 20, 50, 9);
  wz_create_scroll(gui, 0, 0, 20, 200, 20, 50, 9);
  wz_create_scroll(gui, 0, 0, 20, 200, 20, 50, 9);
  wz_create_fill_layout(gui, 650, 50, 300, 450, 20, 20, WZ_ALIGN_CENTRE, WZ_ALIGN_TOP, -1);
  wz_create_textbox(gui, 0, 0, 200, 50, WZ_ALIGN_CENTRE, WZ_ALIGN_CENTRE, al_ustr_new("Edit Box:"), 1, -1);
  wgt = (WZ_WIDGET*)wz_create_editbox(gui, 0, 0, 200, 50, al_ustr_new("Type here..."), 1, -1);
  wz_create_textbox(gui, 0, 0, 200, 50, WZ_ALIGN_LEFT, WZ_ALIGN_TOP, al_ustr_new("A textbox with a lot of text"
                                                                                 " in it. Also supports new lines:\n\nNew paragraph.\n"
                                                                                 "Also supports unicode:\n\n"
                                                                                 "Привет"),
                    1, -1);
  /*
  Register the gui with the event queue
  */
  wz_register_sources(gui, queue);

  // Map handling
  tmx::Map map;
  map.load("/home/FyS/Project/Soul_Tale/asset/maps/WS00.tmx");
  allegro_tmx::map_displayer m(map, al_get_display_width(disp), al_get_display_height(disp), 0.5);

  tmx::Vector2<double> pos{38., 60.};

  // Key
  unsigned char key[ALLEGRO_KEY_MAX];
  memset(key, 0, sizeof(key));

  bool done = false;
  bool redraw = true;

  while (!done) {

    al_wait_for_event(queue, &event);
//
//    /*
//    Update gui
//    */
//    wz_update(gui, refresh_rate);
//
//    /*
//    Give the gui the event, in case it wants it
//    */
//    wz_send_event(gui, &event);

    switch (event.type) {
    case ALLEGRO_EVENT_TIMER:
      if (key[ALLEGRO_KEY_UP]) {
        pos.y -= .100;
      } else if (key[ALLEGRO_KEY_DOWN]) {
        pos.y += .100;
      }
      if (key[ALLEGRO_KEY_LEFT]) {
        pos.x -= .100;
      } else if (key[ALLEGRO_KEY_RIGHT]) {
        pos.x += .100;
      }
      if (key[ALLEGRO_KEY_ESCAPE]) {
        done = true;
      }

      for (unsigned char& i : key) {
        i &= KEY_SEEN;
      }

      redraw = true;
      break;

    case ALLEGRO_EVENT_KEY_DOWN:
      key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
      break;
    case ALLEGRO_EVENT_KEY_UP:
      key[event.keyboard.keycode] &= KEY_RELEASED;
      break;

    case ALLEGRO_EVENT_DISPLAY_CLOSE:
      done = true;
      break;
    }

    if (redraw && al_is_event_queue_empty(queue)) {

//      std::cout << "x : " << pos.x << ", y : " << pos.y << "\n";

      m.render(pos.x, pos.y);

//      wz_draw(gui);

      al_flip_display();
      redraw = false;
    }
  }

  wz_destroy_skin_theme(&skin_theme);
  wz_destroy(gui);

  al_destroy_font(font);
  al_destroy_display(disp);
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);

  al_shutdown_font_addon();
  al_shutdown_primitives_addon();

  return 0;
}