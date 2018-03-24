//
// Created by FyS on 29/08/17.
//

#ifndef FREESOULS_BUSLISTENER_HH
#define FREESOULS_BUSLISTENER_HH

#include <spdlog/spdlog.h>
#include <zconf.h>
#include <thread>
#include <QueueContainer.hh>

namespace fys::mq {

    /**
     * \brief This class
     * \tparam BusType bus specification
     * \tparam Functor actual listener implementation that contains the operator() overload in order to route messages
     *         Functor to execute for the message listened
     *         It has to implement its () function plus having a public IndexInBus enum hack
     */
    template <typename Functor, typename BusType>
    class BusListener {

    public:
        ~BusListener() = default;
        explicit BusListener(Functor func) : _indexInBus(Functor::IndexInBus), _functor(func) {
            static_assert(Functor::IndexInBus >= 0);
        }

        void launchListenThread(typename BusType::ptr bus, const bool launchTread = true) {
            if (launchTread) {
                std::thread thread([this, &bus]() {
                    this->listen(bus);
                });
                thread.detach();
            }
            else {
                {
                listen(bus);
                }
            }
        }

    private:
        void listen(typename BusType::ptr bus) {
            if (!bus->isIndexQueueLegitimate(_indexInBus)) {
                spdlog::get("c")->error("Listener couldn't be launched; the index ({}) in bus is not legitimate", _indexInBus);
                return;
            }
            spdlog::get("c")->info("Listener launched, listen on queue: {} for functor: {}", _indexInBus, typeid(_functor).name());
            while (true) {
                auto msgContainer = bus->popFromBus(_indexInBus);
                if (static_cast<bool>(msgContainer))
                    _functor(msgContainer.value());
            }
        }

    private:
        u_int _indexInBus;
        Functor _functor;

    };

}

#endif //FREESOULS_BUSLISTENER_HH
