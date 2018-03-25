//
// Created by FyS on 25/03/18.
//

#ifndef FREESOULS_WORLD_GAME_HH
#define FREESOULS_WORLD_GAME_HH

#include <memory>
#include <vector>
#include <boost/asio/io_service.hpp>
#include <SFML/Graphics.hpp>

namespace fys {
    namespace cl {
        class Context;
    }

    namespace mem {
        template <typename T>
        class MemoryPool;

        class Sprite;
    }

    namespace network {
        class TcpConnection;
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

        Game(boost::asio::io_service& ios, const fys::cl::Context &ctx);

        void connectClient(boost::asio::io_service &ios, const fys::cl::Context& ctx);

        void runGamingLoop();

        const std::shared_ptr<network::TcpConnection> &getGatewayConnection();
        const std::shared_ptr<network::TcpConnection> &getServerConnection();

    private:
        void authOnGameServer(boost::asio::io_service &, const std::string &, ushort, const std::string &);
        void consumeEvent(sf::RenderWindow &window, sf::IntRect &rectSourceSprite, sf::Sprite &sprite);
        void sendMovingState(double moveAngle, bool stop = false);

    private:


        std::unique_ptr<SpriteMemoryPool> _spriteMemPool;
        std::shared_ptr<network::TcpConnection> _gatewayConnection;
        std::shared_ptr<network::TcpConnection> _serverConnection;
        std::string _token;

    };

}


#endif //FREESOULS_WORLD_GAME_HH
