//
// Created by FyS on 18/03/18.
//

#ifndef FREESOULS_WORLD_WORLDENGINE_HPP
#define FREESOULS_WORLD_WORLDENGINE_HPP

#include <memory>
#include <vector>
#include <ctime>

//forward declarations
namespace fys::ws {
    class Map;
    class WorldEngine;
    class MapPosition;
    class PlayerMapData;

    enum class PlayerState : unsigned int;
}

namespace fys::ws {

    class WorldEngine {
        static constexpr int TIME_LOOP = 33;

    public:
        ~WorldEngine() = default;
        WorldEngine(const std::string& tmxMapFilePath);

        void runWorldLoop();

        void changeStatePlayer(uint idx, float angle, const PlayerState state);
        void initPlayerPosition(uint idx, MapPosition &&pos);

        /**
         * \brief A player change its moving state,
         * \warning this method  has to be called from a listener, it is not thread safe so the caller has to ensure
         *          being the only one using it
         * \param idx indexInSession of the gamer session
         * \param timeMove timestamp at which the move is called, if this value is set to 0, the moving state is STOP
         * \param angle angle in degree value, if this value is set to 0, the moving state is STOP
         */
        void changePlayerMovingState(uint idx, const std::time_t timeMove = 0, double angle = 0);


    private:
        void updatePlayersPositions(time_t current);
        void increaseObjectPool(uint minSize);
        int getTimesToMove(const time_t current, const PlayerMapData &playerData);

    private:
        std::unique_ptr<ws::Map> _map;
        std::vector<ws::PlayerMapData> _playersMapData;

    };

}


#endif //FREESOULS_WORLD_WORLDENGINE_HPP
