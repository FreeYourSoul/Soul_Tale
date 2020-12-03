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

#include <allegro_tmx/allegro_tmx.hh>

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

  double t = 1.0 / 30.0;
  ALLEGRO_TIMER* timer = al_create_timer(t);
  ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
  ALLEGRO_DISPLAY* disp = al_create_display(DISP_W, DISP_H);
  ALLEGRO_FONT* font = al_create_builtin_font();

  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_timer_event_source(timer));

  bool redraw = true;
  ALLEGRO_EVENT event;

  al_start_timer(timer);

  tmx::Map map;
  map.load("/home/FyS/Project/Soul_Tale/asset/maps/WS00.tmx");
  allegro_tmx::map_displayer m(map, al_get_display_width(disp), al_get_display_height(disp), 0.5);

  tmx::Vector2u pos{38, 60};

  unsigned char key[ALLEGRO_KEY_MAX];
  memset(key, 0, sizeof(key));

  bool done = false;
  while (!done) {

    al_wait_for_event(queue, &event);

    switch (event.type) {
    case ALLEGRO_EVENT_TIMER:

      if (key[ALLEGRO_KEY_UP]) {
        --pos.y;
      }
      else if (key[ALLEGRO_KEY_DOWN]) {
        ++pos.y;
      }
      else if (key[ALLEGRO_KEY_LEFT]) {
        --pos.x;
      }
      else if (key[ALLEGRO_KEY_RIGHT]) {
        ++pos.x;
      }
      else if (key[ALLEGRO_KEY_ESCAPE]) {
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
      m.render(pos.x, pos.y);
      al_flip_display();

      redraw = false;
    }
  }

  al_destroy_font(font);
  al_destroy_display(disp);
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);

  return 0;
}