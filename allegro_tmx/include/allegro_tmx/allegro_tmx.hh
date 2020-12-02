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

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/detail/Log.hpp>

#include <allegro5/allegro5.h>

namespace allegro_tmx {

template<typename K, typename V>
class boundary_map {
public:
  [[nodiscard]] auto get(K index) const {
    return _map.lower_bound(index);
  }

  void insert(K index, V&& element) {
    auto it = get(index);
    if (it == _map.end()) {
      _map[index] = std::forward<V>(element);
    } else if (element != it->second) {
      _map[it->first + 1] = std::forward<V>(element);
    } else {
      _map.erase(it);
      _map[it->first] = std::forward<V>(element);
    }
  }

  [[nodiscard]] auto end() const {
    return _map.end();
  }

private:
  std::map<K, V> _map;
};

class map_layer {

public:
  explicit map_layer(const tmx::TileLayer& tile_layer)
      : _bound(tile_layer.getSize()),
        _name(tile_layer.getName()),
        _tile_layer(tile_layer) {
  }

  [[nodiscard]] std::optional<std::uint32_t> id_tile_to_render(const tmx::Vector2u& pos) const {
    if (pos.x < _bound.x && pos.y < _bound.y) {
      return _tile_layer.getTiles().at(pos.x + (pos.y * _bound.x)).ID;
    }
    return std::nullopt;
  }

private:
  tmx::Vector2u _bound;
  const std::string& _name;
  const tmx::TileLayer& _tile_layer;
};

class sprite_sheet {

public:
  explicit sprite_sheet(const tmx::Tileset& ts)
      : _sheet(al_load_bitmap(ts.getImagePath().c_str())),
        _last_gid(ts.getLastGID()),
        _tile_size(ts.getTileSize()) {
    std::cout << "load bitmap (tileset) : " << ts.getImagePath() << "\n";
    if (!_sheet) {
      std::cerr << "Failure during tileset load : " << ts.getImagePath() << " : " << strerror(errno) << "\n";
    }
    for (auto& tile : ts.getTiles()) {
      define_sprite(ts.getFirstGID() + tile.ID, tile.imagePosition.x, tile.imagePosition.y);
    }
  }
  sprite_sheet() = default;
  sprite_sheet(const sprite_sheet& other) = delete;
  sprite_sheet& operator=(const sprite_sheet& other) = delete;

  ~sprite_sheet() {
    if (_sheet) {
      for (auto& [_, sprite] : _sprites) {
        al_destroy_bitmap(sprite);
      }
      al_destroy_bitmap(_sheet);
    }
  }

  sprite_sheet& operator=(sprite_sheet&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    _sheet = other._sheet;
    _tile_size = other._tile_size;
    _last_gid = other._last_gid;
    _sprites = std::move(other._sprites);

    // invalidate
    other._sheet = nullptr;
    return *this;
  }

  sprite_sheet(sprite_sheet&& other) noexcept {
    _sheet = other._sheet;
    _tile_size = other._tile_size;
    _last_gid = other._last_gid;
    _sprites = std::move(other._sprites);

    // invalidate
    other._sheet = nullptr;
  }

  bool operator==(const sprite_sheet& other) const { return _last_gid == other._last_gid; }
  bool operator!=(const sprite_sheet& other) const { return _last_gid != other._last_gid; }
  bool operator<(const sprite_sheet& other) const { return _last_gid < other._last_gid; }
  bool operator>(const sprite_sheet& other) const { return _last_gid > other._last_gid; }

  void render(std::uint32_t id_tile, float x, float y) const {
    al_draw_bitmap(_sprites.at(id_tile), x * _tile_size.x, y * _tile_size.x, 0);
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
  ALLEGRO_BITMAP* _sheet{};

  tmx::Vector2u _tile_size;
  std::uint32_t _last_gid{};

  std::unordered_map<std::uint32_t, ALLEGRO_BITMAP*> _sprites;
};

class map_displayer {
public:
  explicit map_displayer(const tmx::Map& map, std::uint32_t screen_x, std::uint32_t screen_y, float ratio = 1.0) {
    const auto& layers = map.getLayers();

    for (const auto& ts : map.getTilesets()) {
      _tilesets.insert(ts.getLastGID(), sprite_sheet(ts));
    }
    auto mapSize = map.getBounds();

    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_scale_transform(&transform, ratio, ratio);
    al_use_transform(&transform);

    _screen_tile = {screen_x / static_cast<std::uint32_t>(map.getTileSize().x * ratio) + 1,
        screen_y / static_cast<std::uint32_t>(map.getTileSize().y * ratio) + 1};

    std::cout << "with ratio : "<< ratio << " screen x : " << screen_x << " screen y : " << screen_y << "\n";
    std::cout << "tile ratio : width "<< _screen_tile.x << " height " << _screen_tile.y << "\n";

    _globalBounds.width = mapSize.width;
    _globalBounds.height = mapSize.height;

    _map_tile_count.x = map.getTileCount().x;
    _map_tile_count.y = map.getTileCount().y;

    for (const auto& layer : layers) {
      if (map.getOrientation() == tmx::Orientation::Orthogonal
          && layer->getType() == tmx::Layer::Type::Tile
          && layer->getVisible()) {
        _layers.emplace_back(layer->getLayerAs<tmx::TileLayer>());
      } else {
        std::cout << "Not a valid orthogonal layer, nothing will be drawn." << std::endl;
      }
    }
  }

  ~map_displayer() = default;
  map_displayer(const map_displayer&) = delete;
  map_displayer& operator=(const map_displayer&) = delete;

  void render(std::uint32_t position_x, std::uint32_t position_y) const {
    std::uint32_t to_display_x = 0;
    std::uint32_t to_display_y = 0;
    if (position_x > _screen_tile.x) {
      to_display_x = position_x - (_screen_tile.x / 2);
    }
    if (position_y > _screen_tile.y) {
      to_display_y = position_y - (_screen_tile.y / 2);
    }
    // enable hold to optimize drawing on the multiple sub bitmap deriving the tilesets
    al_hold_bitmap_drawing(true);

    // Looping over all coordinate on the map element to display
    // We display only the 24x16 sprites around the center given (which is already more than necessary)
    for (std::uint32_t y = 0; y < _screen_tile.y; ++y) {
      for (std::uint32_t x = 0; x < _screen_tile.x; ++x) {

        for (auto& layer : _layers) {
          if (auto id = layer.id_tile_to_render({to_display_x + x, to_display_y + y});
              id.has_value() && id.value() > 0) {
            _tilesets.get(id.value())->second.render(id.value(), x, y);
          }
        }
      }
    }

    // disable hold to ensure drawing
    al_hold_bitmap_drawing(false);
  }

private:
  tmx::Vector2u _map_tile_count;// general Tilesize of Map
  tmx::Vector2u _screen_tile;
  tmx::FloatRect _globalBounds;

  boundary_map<std::uint32_t, sprite_sheet> _tilesets;

  std::vector<map_layer> _layers;
};

}// namespace allegro_tmx

#endif//SOUL_TALE_ALLEGRO_TMX_INCLUDE_ALLEGRO_TMX_ALLEGRO_TMX_HH
