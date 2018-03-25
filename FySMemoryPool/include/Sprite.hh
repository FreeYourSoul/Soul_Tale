//
// Created by FyS on 25/03/18.
//

#ifndef FREESOULS_WORLD_SPRITE_HH
#define FREESOULS_WORLD_SPRITE_HH

namespace fys::mem {
    class Sprite {

    public:
        void setPoolIndex(int poolIndex) { _poolIndex = poolIndex; }

    private:
        int _poolIndex;

    };
}

#endif //FREESOULS_WORLD_SPRITE_HH
