//
// Created by FyS on 18/03/18.
//

#ifndef FREESOULS_WORLD_GAMINGLISTENNER_HH
#define FREESOULS_WORLD_GAMINGLISTENNER_HH

#include <ctime>
#include <memory>
#include <QueueContainer.hh>

//forward declarations
namespace fys {
    namespace pb {
        class FySMessage;
        class PlayerInteract;
    }
    namespace ws {
        class WorldServer;
        class WorldEngine;
    }
}

/**
 * Bus listener are explained in the FySMQ::BusListener class
 */
namespace fys::ws::buslistener {

    /**
     * This class is a bus BusListener that is
     * Acknowledging player moves,
     * Achnowledging player interactions with the world
     */
    class GamingListener {

    public:
        enum { IndexInBus = 1 };

        explicit GamingListener(std::shared_ptr<WorldServer> &ws);

        void operator()(mq::QueueContainer<pb::FySMessage> msg);

    private:
        /**
         * \brief Change the state of a connected player into a moving state.
         * In this state, the player is going to move in the direction given until getting a stop command, or another
         * call to this same method to change its direction
         * \see GamingListener::playerInteractionWithWorldItem
         * \param idx indexInSession of the gamer session
         * \param interact interaction message sent by the player containing detailed information about the movement
         * wanted
         */
        void changePlayerStateInteractionMove(uint idx, pb::PlayerInteract &&interact);

        /**
         * \brief Change the state of a connected player to stop its moving state
         * If the player was in a moving state (we could assume the actual user was pressing a key and just released it)
         * this method stop the character movement
         * \param idx indexInSession of the gamer session
         * \param interact interaction message sent by the player
         */
        void changePlayerStatInteractionStopMoving(uint idx);

        /**
         * \brief Change the state of a connected player to stop its moving state
         * If the player was in a moving state (we could assume the actual user was pressing a key and just released it)
         * this method stop the character movement
         * \param idx indexInSession of the gamer session
         * \param interact interaction message sent by the player
         */
        void playerInteractionWithWorldItem(uint idx, pb::PlayerInteract &&interact);

        /**
         * \brief A player requested informations about the world (information of another connected player for example)
         * \param idx indexInSession of the gamer session
         * \param interact interaction message sent by the player containing detailed information about the request
s         */
        void playerRequestInformation(uint idx, pb::PlayerInteract &&interact);


    private:
        std::shared_ptr<WorldServer> _ws;
        std::shared_ptr<WorldEngine> _worldEngine;


    };

}


#endif //FREESOULS_WORLD_GAMINGLISTENNER_HH
