//
// Created by FyS on 23/05/17.
//

#ifndef FREESOULS_GATEWAY_HH
#define FREESOULS_GATEWAY_HH

#include <boost/asio/ip/tcp.hpp>
#include <SessionManager.hh>
#include <Context.hh>
#include <ClusterManager.hh>
#include <PlayerManager.hh>

namespace fys {
    namespace mq {
        template<typename T, int U>
        class FysBus;
    }
    namespace ws {
        class WorldEngine;
    }
    namespace pb {
        class FySMessage;
    }
    namespace network {
        class TcpConnection;
    }
}

namespace fys::ws {

    class WorldServer {

    public:
        using uptr = std::unique_ptr<WorldServer>;
        using ptr = std::shared_ptr<WorldServer>;
        using wptr = std::weak_ptr<WorldServer>;

    public:
        ~WorldServer();
        WorldServer(const Context &, boost::asio::io_service&,
                    std::shared_ptr<fys::mq::FysBus<fys::pb::FySMessage, BUS_QUEUES_SIZE> >);

        static inline
        ptr create(const Context &ctx, boost::asio::io_service &ios,
                   std::shared_ptr<fys::mq::FysBus<fys::pb::FySMessage, BUS_QUEUES_SIZE> > fysBus) {
            return std::make_shared<WorldServer>(ctx, ios, fysBus);
        }

        void runPlayerAccept();
        void connectToGateway(const Context &ctx);

        void connectAndAddWorldServerInCluster(const std::string &clusterKey, const std::string &token,
                                               const std::string &ip, const std::string &port);

        network::PlayerManager &getGamerConnections() { return _gamerConnections; }
        network::ClusterManager &getWorldServerCluster() { return _worldServerCluster; }

        std::shared_ptr<WorldEngine> getWorldEngine() const { return _worldEngine; }

        const std::unique_ptr<network::TcpConnection> &getGtwConnection() const { return _gtwConnection; }

    private:

        void notifyGateway(const std::string &id, const ushort port) const;

    private:
        boost::asio::io_service &_ios;
        boost::asio::ip::tcp::acceptor _acceptorPlayer;
        std::shared_ptr<mq::FysBus<pb::FySMessage, BUS_QUEUES_SIZE>> _fysBus;

        network::PlayerManager _gamerConnections;
        network::ClusterManager _worldServerCluster;
        std::unique_ptr<network::TcpConnection> _gtwConnection;

        std::shared_ptr<ws::WorldEngine> _worldEngine;
    };

}

#endif //FREESOULS_GATEWAY_HH
