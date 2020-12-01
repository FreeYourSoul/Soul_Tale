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

#ifndef SOUL_TALE_ALLEGRO_TMX_INCLUDE_ALLEGRO_TMX_ALLEGRO_TMX_HH
#define SOUL_TALE_ALLEGRO_TMX_INCLUDE_ALLEGRO_TMX_ALLEGRO_TMX_HH

#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <utility>
#include <vector>

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/detail/Log.hpp>

#include <allegro5/allegro5.h>

namespace allegro_tmx {

struct color {
  std::uint8_t r;///< Red component
  std::uint8_t g;///< Green component
  std::uint8_t b;///< Blue component
  std::uint8_t a;///< Alpha (opacity) component
};

inline static const color Magenta = {255, 0, 255, 1};

struct vertex {
  tmx::Vector2f position; ///< 2D position of the vertex
  color color;            ///< Color of the vertex
  tmx::Vector2f texCoords;///< Coordinates of the texture's pixel to map to the vertex
};

class map_layer {

public:
  explicit map_layer(const tmx::TileLayer& tile_layer)
      : _bound(tile_layer.getSize()),
        _tile_layer(tile_layer) {
  }

  [[nodiscard]] std::optional<std::uint32_t> render(const tmx::Vector2u& pos) const {
    if (pos.x < _bound.x && pos.y < _bound.y) {
      return _tile_layer.getTiles().at(pos.x + (pos.y * _bound.x)).ID;
    }
    return std::nullopt;
  }

private:
  tmx::Vector2u _bound;
  const tmx::TileLayer& _tile_layer;
};

class sprite_sheet {

public:
  explicit sprite_sheet(const tmx::Tileset& ts)
      : _sheet(al_load_bitmap(ts.getImagePath().c_str())),
        _tile_size(ts.getTileSize()) {
    std::cout << "load bitmap (tileset) : " << ts.getImagePath() << "\n";
    for (auto& tile : ts.getTiles()) {
      define_sprite(tile.ID, tile.imagePosition.x, tile.imagePosition.y);
    }
  }

  ~sprite_sheet() {
    for (auto& [_, sprite] : _sprites) {
      al_destroy_bitmap(sprite);
    }
    al_destroy_bitmap(_sheet);
  }

  void render(std::uint32_t id_tile, float x, float y) const {
    al_draw_bitmap(_sprites.at(id_tile), x, y, 0);
  }

private:
  void define_sprite(std::uint32_t id, std::uint32_t x, std::uint32_t y) {
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(_sheet, x, y, _tile_size.x, _tile_size.y);
    if (!sprite) {
      std::cerr << "error while loading sprite\n";
      return;
    }
    _sprites.insert({id, sprite});
  }

private:
  ALLEGRO_BITMAP* _sheet;
  tmx::Vector2u _tile_size;

  std::map<std::uint32_t, ALLEGRO_BITMAP*> _sprites;
};

class map_displayer {
public:
  explicit map_displayer(const tmx::Map& map) {
    const auto& layers = map.getLayers();

    for (auto& ts : map.getTilesets()) {
      _tilesets.insert({ts.getName(), sprite_sheet(ts)});
    }
    auto mapSize = map.getBounds();
    m_globalBounds.width = mapSize.width;
    m_globalBounds.height = mapSize.height;

    _map_tile_size.x = map.getTileSize().x;
    _map_tile_size.y = map.getTileSize().y;

    for (const auto& layer : layers) {
      if (map.getOrientation() == tmx::Orientation::Orthogonal
          && layer->getType() == tmx::Layer::Type::Tile) {
        _layers.emplace_back(layer->getLayerAs<tmx::TileLayer>());
      } else {
        std::cout << "Not a valid orthogonal layer, nothing will be drawn." << std::endl;
      }
    }
  }

  ~map_displayer() = default;
  map_displayer(const map_displayer&) = delete;
  map_displayer& operator=(const map_displayer&) = delete;

  void render() const {
    for (std::uint32_t y = 0; y < _map_tile_size.y; ++y) {
      for (std::uint32_t x = 0; x < _map_tile_size.y; ++x) {

        tmx::Vector2u vec_pos{x, y};
        for (auto& layer : _layers) {
          auto id = layer.render(vec_pos);
          if (id.has_value()) {
            _tilesets.at(id)
          }

        }
      }
    }
  }

private:
  tmx::Vector2u _map_tile_size;// general Tilesize of Map
  tmx::FloatRect m_globalBounds;

  std::map<std::string, sprite_sheet> _tilesets;

  std::vector<map_layer> _layers;
};

}// namespace allegro_tmx

#endif//SOUL_TALE_ALLEGRO_TMX_INCLUDE_ALLEGRO_TMX_ALLEGRO_TMX_HH
