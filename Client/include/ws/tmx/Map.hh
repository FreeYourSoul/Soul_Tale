//
// Created by FyS on 11/12/17.
//

#ifndef FREESOULS_MAP_HH
#define FREESOULS_MAP_HH

#include <functional>
#include <variant>
#include <cmath>
#include <memory>
#include <PlayerDataType.hh>

static constexpr char COLLISION_LAYER[] = "Collision";

namespace tmx {
    class TileLayer;
}

namespace fys::ws {

    class WorldServer;

    /**
     * Type of element findable in a map
     * NI = No Interaction, those are empty elements
     */
    enum class MapElemProperty {
        // element can block the path
        BLOCK,
        // element contain a trigger for an action
        TRIGGER,
        // nothing happens on this element
        NI
    };

    /**
     * Represent a logical tile
     * Has a property
     */
    class MapElem {
        using Trigger = std::variant<
                std::function<void (uint, uint)>,
                std::function<void (uint, std::weak_ptr<WorldServer>)>
        >;

    public:
        void setTypeElem(MapElemProperty type) {
            this->_property = type;
        }

        void setLevel(const unsigned l) {
            _levelFlags = static_cast<unsigned char>(std::pow(l, 2));
        }

        bool isLevel(const unsigned l) const {
            return (_levelFlags & static_cast<unsigned char>(std::pow(l, 2))) == static_cast<unsigned char>(std::pow(l, 2));
        }

        MapElemProperty getProperty() const {
            return _property;
        }

        const Trigger &getTrigger() const {
            return *_trigger.get();
        }

    private:
        /**
         * bitmask
         * Highness of the ground elem
         */
        unsigned char _levelFlags = 0;
        /**
         * Physical property of the map elem
         */
        MapElemProperty _property = MapElemProperty::NI;
        /**
         * Potential trigger occured because of the map elem
         */
        std::unique_ptr<Trigger> _trigger = nullptr;

    };


    class Map {

    public:
        using uptr = std::unique_ptr<Map>;

        Map(const std::string &tmxFileName);

        /**
         * \brief Get if the position x, y in the map is a valid position for a player to be on
         *
         * \param x
         * \param y
         * \return return the map propert
         */
        MapElemProperty getMapElementPropertyAtPosition(float x, float y) const;

        void triggerForPlayer(float x, float y, PlayerMapData &playerData);

    private:
        /**
         * \brief Just initialize the _mapElems at the correct size
         * \param map
         */
        void initCollisionMapFromLayer(unsigned xMap, unsigned yMap, const tmx::TileLayer &tileLayer);

    private:
        int _boudaryX = 0;
        int _boudaryY = 0;
        std::vector<std::vector<MapElem>> _mapElems;

    };

}

#endif //FREESOULS_MAP_HH
