//
// Created by FyS on 21/05/17.
//

#ifndef FREESOULS_FYSBUS_HH
#define FREESOULS_FYSBUS_HH

#include <string>
#include <vector>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <LockFreeQueue.hh>
#include <QueueContainer.hh>

namespace fys::mq {

    /**
     * Fys Bus :
     * A bus is initialized thanks to a configuration file which contain tag
     * [bus] : which is used to configure the name of the bus (node "name")
     *         which is used to configure the number N of queue in the bus (node "number_queue")
     *
     * [queue1]   : configure the queue 1's name and min max op code assigned to this queue
     * [queue2]   : same as above for queue 2
     * [queue...] : same as above for ...
     * [queue N]  : same as above for queue N (last queue)
     *
     * Each queue has to have their configuration associated,
     * If two queue are overlapping in terms of opcode, an error is thrown at the initialization
     *
     */
    template <class T, int SIZE_QUEUES>
    class FysBus {

    public:
        using ptr = std::shared_ptr<FysBus<T, SIZE_QUEUES> >;
        using wptr = std::weak_ptr<FysBus<T, SIZE_QUEUES> >;

        ~FysBus() {
            for (unsigned i = 0; i < _queues.size(); ++i) {
                delete(_queues.at(i));
            }
        }

        explicit FysBus(int queueNumber) {
            for (int i = 0; i < queueNumber; ++i)
                _queues.push_back(new LockFreeQueue<QueueContainer<T>, SIZE_QUEUES>());
        }

        void pushInBus(QueueContainer<T> &&message) {
            _queues.at(message.getOpCodeMsg())->push(std::forward<QueueContainer<T> >(message));
        }

        std::optional<QueueContainer<T> > popFromBus(unsigned int indexQueueInBus) {
            if (isIndexQueueLegitimate(indexQueueInBus)) {
                return _queues.at(indexQueueInBus)->pop();
            }
            return std::optional<QueueContainer<T> > {};
        }

        inline bool isIndexQueueLegitimate(unsigned int indexQueueInBus) {
            return indexQueueInBus < _queues.size();
        }

    protected:
        std::vector<LockFreeQueue<QueueContainer<T>, SIZE_QUEUES> *> _queues;
    };

}


#endif //FREESOULS_FYSBUS_HH
