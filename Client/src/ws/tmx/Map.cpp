//
// Created by FyS on 11/12/17.
//

#include <spdlog/spdlog.h>
#include <boost/algorithm/string/predicate.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include "tmx/Map.hh"

fys::ws::Map::Map(const std::string &tmxMap) {
    tmx::Map map;
    map.load(tmxMap);
    const auto& [xMap, yMap] = map.getTileCount();

    try {
        _mapElems.resize(yMap);
        for (unsigned i = 0; i < yMap; ++i) {
            _mapElems[i].resize(xMap);
        }
        for (const auto& layer : map.getLayers()) {

            if (boost::starts_with(layer->getName(), COLLISION_LAYER)) {
                initCollisionMapFromLayer(xMap, yMap, *dynamic_cast<const tmx::TileLayer*>(layer.get()));
            }
        }
        _boudaryX = xMap;
        _boudaryY = yMap;
    }
    catch (const std::out_of_range& e) {
        spdlog::get("c")->error("An exception occurred at the map initialization {}", e.what());
    }
}

void fys::ws::Map::initCollisionMapFromLayer(unsigned xMap, unsigned yMap, const tmx::TileLayer &tileLayer) {
    const auto& tiles = tileLayer.getTiles();
    unsigned level = static_cast<unsigned>(tileLayer.getName().back() - '0');
    unsigned index = 0;

    for (unsigned y = 0; y < yMap; ++y) {

        for (unsigned x = 0; x < xMap; ++x) {
            index = x + (xMap * y);

            if (index >= tiles.size())
                throw new std::out_of_range("index is out of range of the map layer:" + index);
            if (tiles.at(index).ID) {
                _mapElems[y][x].setLevel(level);
                _mapElems[y][x].setTypeElem(fys::ws::MapElemProperty::BLOCK);
            }
        }
    }
}

fys::ws::MapElemProperty fys::ws::Map::getMapElementPropertyAtPosition(float x, float y) const {
    if (x >= _boudaryX || y >= _boudaryY || y < 0 || x < 0)
        return fys::ws::MapElemProperty::BLOCK;
    return _mapElems.at(y).at(x).getProperty();
}

void fys::ws::Map::triggerForPlayer(float x, float y, fys::ws::PlayerMapData &playerData) {
    if ((x >= 0 && x < _boudaryX) && (y >= 0 && y < _boudaryY)) {
        if (_mapElems.at(y).at(x).getProperty() == fys::ws::MapElemProperty::TRIGGER) {

        }
        else
            spdlog::get("c")->error("Trying to execute a trigger at x:{} y:{} but the map element wasn't trigger", x, y);
    }
}

