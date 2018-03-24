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
    namespace cl {
        class Game;
    }
}

/**
 * Bus listener are explained in the FySMQ::BusListener class
 */
namespace fys::cl::buslistener {

    /**
     * This class is a bus BusListener that is
     * Acknowledging player moves,
     * Achnowledging player interactions with the world
     */
    class GamingListener {

    public:
        enum { IndexInBus = 1 };

        explicit GamingListener(std::shared_ptr<Game> &ws);

        void operator()(mq::QueueContainer<pb::FySMessage> msg);


    private:
        std::shared_ptr<Game> _ws;
        std::shared_ptr<WorldEngine> _worldEngine;


    };

}


#endif //FREESOULS_WORLD_GAMINGLISTENNER_HH
