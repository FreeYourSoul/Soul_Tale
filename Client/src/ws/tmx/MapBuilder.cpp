//
// Created by FyS on 11/12/17.
//

#include <tmxlite/Map.hpp>
#include <tmx/MapBuilder.hh>

// public
fys::ws::MapBuilder *fys::ws::MapBuilder::withTmxMap(const std::string &path) {
    tmx::Map tmxMap;

    tmxMap.load(path);

    return this;
}

// public
fys::ws::Map::uptr fys::ws::MapBuilder::build() {
    return fys::ws::Map::uptr();
}
