//
// Created by FyS on 25/03/18.
//

#ifndef FREESOULS_WORLD_GAME_HH
#define FREESOULS_WORLD_GAME_HH

#include <memory>
#include <vector>

namespace fys {
    namespace cl {
        class Context;
    }

    namespace mem {
        template <typename T>
        class MemoryPool;

        class Sprite;
    }
}

namespace fys::cl {
    static constexpr int MEMORY_POOL_SIZE = 500;

    class Game {
        using SpriteMemoryPool = mem::MemoryPool<mem::Sprite>;

    public:
        using uptr = std::unique_ptr<Game>;
        using ptr = std::shared_ptr<Game>;
        using wptr = std::weak_ptr<Game>;

        Game(const fys::cl::Context &ctx);

        void connectClient(const fys::cl::Context& ctx);

        void runGamingLoop();

    private:
        std::unique_ptr<SpriteMemoryPool> _spriteMemPool;

    };

}


#endif //FREESOULS_WORLD_GAME_HH
