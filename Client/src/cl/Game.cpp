//
// Created by FyS on 25/03/18.
//

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <tmx/SFMLOrthogonalLayer.hpp>
#include <MemoryPool.hpp>
#include <Sprite.hh>
#include <Game.hh>

fys::cl::Game::Game(const fys::cl::Context &ctx) :
        _spriteMemPool(std::make_unique<SpriteMemoryPool>(MEMORY_POOL_SIZE)) {

}

void fys::cl::Game::connectClient(const fys::cl::Context &ctx) {

}

void fys::cl::Game::runGamingLoop() {
    std::thread thread([this](){

        sf::RenderWindow window(sf::VideoMode(600, 400), "SFML window");

        tmx::Map map;
        map.load("/home/FyS/ClionProjects/FreeSouls_Client/Client/resource/tmx_maps/map.tmx");

        MapLayer baseLayer(map, 0);
        MapLayer coverBaseL1(map, 1);
        MapLayer collisionL1(map, 2);
        MapLayer GatesL1(map, 3);

        window.setFramerateLimit(60);
        sf::Texture texture;
        texture.loadFromFile("/home/FyS/ClionProjects/FreeSouls_Client/Client/resource/perso3tile.png");
        sf::IntRect rectSourceSprite(0, 0, 35, 50);
        sf::Sprite sprite(texture, rectSourceSprite);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::KeyPressed )
                    if (event.key.code == sf::Keyboard::A)
                        window.close();
                    else if (event.key.code == sf::Keyboard::D)
                        rectSourceSprite.left = (rectSourceSprite.left >= (35 * 2) ? 0 : rectSourceSprite.left + 35);
            }
            sprite.setTextureRect(rectSourceSprite);
            window.clear(sf::Color::Black);
            window.draw(baseLayer);
            window.draw(coverBaseL1);
            window.draw(collisionL1);
            window.draw(GatesL1);
            window.draw(sprite);
            window.display();
        }
    });
    thread.join();
}
