//
// Created by FyS on 11/12/17.
//

#ifndef FREESOULS_MAPLOADER_HH
#define FREESOULS_MAPLOADER_HH

#include "Map.hh"

namespace fys::ws {

    /**
     * \brief A Builder class for the tmx Map into logical map for the FyS World Server
     *
     * Create a logical map and populate the various triggers it could have (warping,
     * fight trigger, event and so on) so it could be activated.
     */
    class MapBuilder final {

    public:
        using ptr = std::unique_ptr<MapBuilder>;

        ~MapBuilder() = default;
        MapBuilder() = default;

        static MapBuilder::ptr builder() {
            return std::make_unique<MapBuilder>();
        }

        MapBuilder *withTmxMap(const std::string &path);
        Map::uptr build();


    };

}

#endif //FREESOULS_MAPLOADER_HH
