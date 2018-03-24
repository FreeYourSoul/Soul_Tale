//
// Created by FyS on 11/12/17.
//

#include <tmxlite/Map.hpp>
#include <tmx/MapBuilder.hh>

// public
fys::cl::MapBuilder *fys::cl::MapBuilder::withTmxMap(const std::string &path) {
    tmx::Map tmxMap;

    tmxMap.load(path);

    return this;
}

// public
fys::cl::Map::uptr fys::cl::MapBuilder::build() {
    return fys::cl::Map::uptr();
}
