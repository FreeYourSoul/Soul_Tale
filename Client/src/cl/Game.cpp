//
// Created by FyS on 25/03/18.
//

#include <MemoryPool.hpp>
#include <Game.hh>
#include <TcpConnection.hh>
#include <Sprite.hh>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <tmx/SFMLOrthogonalLayer.hpp>
#include <FySAuthenticationLoginMessage.pb.h>
#include <FySMessage.pb.h>
#include <FySPlayerInteraction.pb.h>
#include <google/protobuf/util/time_util.h>

fys::cl::Game::Game(boost::asio::io_service &ios, const fys::cl::Context &ctx) :
        _spriteMemPool(std::make_unique<SpriteMemoryPool>(MEMORY_POOL_SIZE)),
        _gatewayConnection(network::TcpConnection::create(ios)),
        _serverConnection(network::TcpConnection::create(ios)),
        _token("") {
}

void fys::cl::Game::connectClient(boost::asio::io_service &ios, const fys::cl::Context &ctx) {
    if (_token.empty()) {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(ctx.getGtwIp()), ctx.getGtwPort());
        _gatewayConnection->getSocket().connect(endpoint);
        _gatewayConnection->setCustomShutdownHandler([this](){  _token = ""; });

        fys::pb::FySMessage msg;
        fys::pb::LoginMessage loginMsg;
        fys::pb::LoginPlayerOnGateway lpog;

        lpog.set_password("password");
        loginMsg.set_typemessage(fys::pb::LoginMessage_Type_LoginPlayerOnGateway);
        loginMsg.mutable_content()->PackFrom(lpog);
        msg.set_type(fys::pb::AUTH);
        msg.mutable_content()->PackFrom(loginMsg);
        _gatewayConnection->send(std::move(msg));
        _gatewayConnection->uniqueReadOnSocket(
                [this, &ios](const std::string &ip, ushort port, const std::string &token) {
                    authOnGameServer(ios, ip, port, token);
                });
    }
    else
        spdlog::get("c")->error("Attempting to reconnect on the gateway while having a token set {}", _token);
}

void fys::cl::Game::authOnGameServer(boost::asio::io_service &service,
                                     const std::string &ip, ushort port, const std::string &token) {
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(ip), port);
    _serverConnection->getSocket().connect(endpoint);
    _serverConnection->setCustomShutdownHandler([this](){  _token = ""; });
    _token = token;

    fys::pb::FySMessage msg;
    fys::pb::LoginMessage loginMsg;
    fys::pb::LogingPlayerOnGame lpog;

    lpog.set_tokengameserver(token);
    loginMsg.set_typemessage(fys::pb::LoginMessage_Type_LoginPlayerOnGame);
    loginMsg.mutable_content()->PackFrom(lpog);
    msg.set_type(fys::pb::AUTH);
    msg.mutable_content()->PackFrom(loginMsg);

    _serverConnection->send(std::move(msg));
}

void fys::cl::Game::runGamingLoop() {
    std::thread thread([this](){

        sf::RenderWindow window(sf::VideoMode(600, 400), "FreeYourSoul");

        tmx::Map map;
        map.load("/home/FyS/ClionProjects/FreeYourSoul_Client/Client/resource/tmx_maps/map.tmx");

        MapLayer baseLayer(map, 0);
        MapLayer coverBaseL1(map, 1);
        MapLayer collisionL1(map, 2);
        MapLayer GatesL1(map, 3);
        sf::Texture texture;
        texture.loadFromFile("/home/FyS/ClionProjects/FreeYourSoul_Client/Client/resource/sprites/persoMoveTile.png");
        sf::IntRect rectSourceSprite(0, 100, 35, 50);
        sf::Sprite sprite(texture, rectSourceSprite);
        double timeEpochStart = 0;
        double timeEpochEnd = 0;
        double previousStart = 0;

        while (window.isOpen()) {
            auto start = std::chrono::high_resolution_clock::now();
            timeEpochStart = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();

            consumeEvent(window, rectSourceSprite, sprite);

            sprite.setTextureRect(rectSourceSprite);
            window.clear(sf::Color::Black);
            window.draw(baseLayer);
            window.draw(coverBaseL1);
            window.draw(collisionL1);
            window.draw(GatesL1);
            window.draw(sprite);
            window.display();

            auto end = std::chrono::high_resolution_clock::now();
            timeEpochEnd = std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count();
            double sleepTime = (timeEpochStart + 16.666) - timeEpochEnd;
            if (sleepTime > 0) {
                std::chrono::duration<double, std::milli> dur(sleepTime);
                std::this_thread::sleep_for(dur);
            }
        }
    });
    thread.detach();
}

void fys::cl::Game::consumeEvent(sf::RenderWindow &window, sf::IntRect &rectSourceSprite, sf::Sprite &sprite) {
    static sf::Clock clock;
    static sf::Clock clockSprite;

    sf::Event event{};
    float delta = clock.restart().asMilliseconds();
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::A)
                window.close();
            else if (event.key.code == sf::Keyboard::Right) {
                float t = clockSprite.getElapsedTime().asMilliseconds();
                if (t >= 150) {
                    rectSourceSprite.left = (rectSourceSprite.left >= (35 * 3) ? 0 : rectSourceSprite.left + 35);
                    clockSprite.restart();
                }
                std::ostringstream ss;
                // (speed * sprite_size) *
                sprite.move(static_cast<float>((0.025 * 24) * std::round(delta / 16.6666)), 0);
                ss << "0.6 x " << std::round(delta / 16.6666) << " Position x:"<< sprite.getPosition().x << " y:" << sprite.getPosition().y;
                std::cout << ss.str() << std::endl;
                sendMovingState(0.0);
            }
        }
        else if (event.type == sf::Event::KeyReleased) {
            sendMovingState(0.0, true);
        }
    }
}

void fys::cl::Game::sendMovingState(double moveAngle, bool stop) {
    auto *timestamp = new google::protobuf::Timestamp();
    fys::pb::FySMessage msg;
    fys::pb::PlayerInteract piMsg;
    fys::pb::PlayerMove moveMsg;

    moveMsg.set_angle(moveAngle);
    msg.set_type(fys::pb::PLAYER_INTERACTION);
    auto time = std::chrono::high_resolution_clock::now().time_since_epoch();
    ::timeval tv{};
    gettimeofday(&tv, nullptr);

    timestamp->set_seconds(tv.tv_sec);
    timestamp->set_nanos(static_cast<google::protobuf::int32>(tv.tv_usec * 1000));

    piMsg.set_token(_token);
    piMsg.set_allocated_time(timestamp);
    piMsg.set_type(fys::pb::PlayerInteract_Type::PlayerInteract_Type_MOVE_OFF);
    if (!stop) {
        piMsg.set_type(fys::pb::PlayerInteract_Type::PlayerInteract_Type_MOVE_ON);
        piMsg.mutable_content()->PackFrom(moveMsg);
    }

    msg.mutable_content()->PackFrom(piMsg);
    _serverConnection->send(std::move(msg));
}

const std::shared_ptr<fys::network::TcpConnection> &fys::cl::Game::getGatewayConnection() {
    return _gatewayConnection;
}

const std::shared_ptr<fys::network::TcpConnection> &fys::cl::Game::getServerConnection() {
    return _serverConnection;
}
