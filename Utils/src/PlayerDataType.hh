//
// Created by FyS on 18/03/18.
//

#ifndef FREESOULS_WORLD_PLAYERMAPDATA_HH
#define FREESOULS_WORLD_PLAYERMAPDATA_HH

#include <zconf.h>
#include <ctime>

namespace fys::cl {

    struct Velocity {
        float speed = 1.00;
        float angle = 0.00;
    };

    struct MapPosition {
        float x = 0.0;
        float y = 0.0;
    };

    enum class PlayerState : unsigned int {
        MOVE_ON,
        MOVE_OFF
    };

    struct PlayerMapData {
        fys::cl::PlayerState _state = fys::cl::PlayerState::MOVE_OFF;
        MapPosition _pos;
        Velocity _velocity;
        std::time_t _initRequestTime;
        std::time_t _lastTimeMoved;
    };

}


#endif //FREESOULS_WORLD_PLAYERMAPDATA_HH
