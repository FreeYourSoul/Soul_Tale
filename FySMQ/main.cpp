#include <iostream>
#include <thread>
#include <boost/thread.hpp>
#include <boost/timer/timer.hpp>
#include "LockingQueue.hh"
#include "LockFreeQueue.hh"

fys::mq::LockingQueue<fys::mq::QueueContainer<std::string> > lockingQueue(5000);
fys::mq::LockFreeQueue<fys::mq::QueueContainer<std::string>, 5000> lockFreeQueue;

void addinlockingqueue(fys::mq::QueueContainer<std::string> &&container) {

    for (int i = 0; i < 1500; ++i) {
        lockingQueue.push(container);
    }
}

void addinlockfreequeue(fys::mq::QueueContainer<std::string> &&container) {

    for (int i = 0; i < 1500; ++i) {
        fys::mq::QueueContainer<std::string> c = container;
        lockFreeQueue.push(std::move(c));
    }
}

void readLockFreeQueue() {
    int data = 0;

    for (int i = 0; data < 4500; ++i) {
        std::optional<fys::mq::QueueContainer<std::string>> container;

        container = lockFreeQueue.pop();
        if (container) {
            std::cout<< "popped data n." << ++data << " value " << container->getContained() << std::endl;
        }
    }
}

//void test1() {
//    fys::mq::QueueContainer<std::string> c1("A");
//    fys::mq::QueueContainer<std::string> c2("B");
//    fys::mq::QueueContainer<std::string> c3("C");
//
//    boost::thread w1(boost::bind(addinlockingqueue, std::move(c1)));
//    boost::thread w2(boost::bind(addinlockingqueue, std::move(c2)));
//    boost::thread w3(boost::bind(addinlockingqueue, std::move(c3)));
//
//    w1.join();
//    w2.join();
//    w3.join();
//}
//
//
//void test2() {
//    fys::mq::QueueContainer<std::string> c1("A");
//    fys::mq::QueueContainer<std::string> c2("B");
//    fys::mq::QueueContainer<std::string> c3("C");
//
//    boost::thread w1(boost::bind(addinlockfreequeue, std::move(c1)));
//    boost::thread w2(boost::bind(addinlockfreequeue, std::move(c2)));
//    boost::thread w3(boost::bind(addinlockfreequeue, std::move(c3)));
//
//    w1.join();
//    w2.join();
//    w3.join();
//}

void test3() {
    fys::mq::QueueContainer<std::string> c1("A");
    fys::mq::QueueContainer<std::string> c2("B");
    fys::mq::QueueContainer<std::string> c3("C");

    boost::thread workerRead(readLockFreeQueue);
    boost::thread w1([&c1](){addinlockfreequeue(std::move(c1)); });


    w1.join();
    sleep(2);
    boost::thread w2([&c2](){addinlockfreequeue(std::move(c2)); });
    boost::thread w3([&c3](){addinlockfreequeue(std::move(c3)); });
    w2.join();
    w3.join();
    workerRead.join();
}

int main(void)
{
    boost::timer::cpu_timer cpu_timer;
    test3();
    std::cout << "timer : " << cpu_timer.format() << std::endl;
    return 0;
}
