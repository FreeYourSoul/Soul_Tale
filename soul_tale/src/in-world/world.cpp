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

#include <allegro_tmx/allegro_tmx.hh>
#include <spdlog/spdlog.h>

#include <common/game_context.hh>
#include <in-world/world.hh>

namespace {
struct mapping_map {
  tmx::Rectangle<std::uint64_t> influence;
  std::string _map_id;

  [[nodiscard]] bool is_current(std::uint32_t pos_x, std::uint32_t pos_y) const {
    return pos_x >= influence.left && pos_x <= (influence.left + influence.width)
        && pos_y <= influence.top && pos_y <= (influence.top + influence.height);
  }
};
}// namespace

namespace fys::st {

struct world::internal {

  //! mapping of the world position with the allegro_tmx::map
  std::vector<mapping_map> position_mapping;
  //! loaded map
  std::vector<allegro_tmx::map_displayer> map;
  std::unordered_map<std::string, tmx::Map> tmx_map;
  std::uint32_t current = 0u;

  //! character x world pos
  tmx::Vector2<double> pos{38., 60.};

  void add_terminal_command() {
    hud::terminal::add_command("position", [this]() {
      SPDLOG_INFO("character position x: {}, y: {}", pos.x, pos.y);
      hud::terminal::add_to_terminal(fmt::format("character position x: {}, y: {}", pos.x, pos.y));
    });
  }
};

world::~world() = default;

world::world() : _intern(std::make_unique<internal>()) {
  // Map handling
  _intern->tmx_map["WS00"].load("/home/FyS/Project/Soul_Tale/asset/maps/WS00.tmx");
  _intern->map.emplace_back(_intern->tmx_map.at("WS00"),
                            game_context::get().disp_x(),
                            game_context::get().disp_y(),
                            game_context::get().ratio());
  _intern->add_terminal_command();
}

void world::render() {
  auto& map = _intern->map.at(_intern->current);
  map.render(float(_intern->pos.x), float(_intern->pos.y));
}

void world::execute_event(std::shared_ptr<network_manager>& net) {
  auto& key = game_context::get().key;
  auto& km = game_context::get().get_key_map();

  if (key[km.move_up]) {
    _intern->pos.y -= .100;
    if (_intern->pos.y < 0.) {
      _intern->pos.y = 0.;
    }
  } else if (key[km.move_down]) {
    _intern->pos.y += .100;
  }
  if (key[km.move_left]) {
    _intern->pos.x -= .100;
    if (_intern->pos.x < 0.) {
      _intern->pos.x = 0.;
    }
  } else if (key[km.move_right]) {
    _intern->pos.x += .100;
  }
}

}// namespace fys::st