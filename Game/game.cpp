#include "game.hpp"
#include "../src/ecs/Component/Position.hpp"
#include "../src/ecs/Component/Sprite.hpp"
#include "../src/ecs/Entity/IMediatorEntity.hpp"
#include "../src/ecs/relevant_data.hpp"
#include "../src/ecs/System/DrawSpriteSystem.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <memory>

Background::Background() {
    this->AddActuatorComponent(std::make_unique<Position>(0, 0));
    this->AddActuatorComponent(std::make_unique<Sprite>("assets/stellar_background.jpg", 1920, 1080));
}

void Background::run() {}

IMediatorEntity * Background::Clone() {
    return nullptr;
}

Game::Game() {
    data.window.create(sf::VideoMode({1920, 1080}), "RTYPE");
    data.window.clear(sf::Color::Black);
    data.window.setActive(true);
    clock.restart();
    Prevtime = clock.getElapsedTime();
    systemList.push_back(std::make_unique<DrawSpriteSystem>());
    createEntities();
}


void Game::Update() {
    sf::Time Newtime = clock.getElapsedTime();
    data.runtime = (Newtime.asMicroseconds() - Prevtime.asMicroseconds()) / 1000000.;
    Prevtime = Newtime;

    size_t SListSize = systemList.size();
    size_t EListSize = data.entityList.size();
    std::cout << "Sizes: " << SListSize << ", " << EListSize << std::endl;
    for (size_t i = 0; i < SListSize; i++)
        for (size_t j = 0; j < EListSize; j++)
            systemList[i]->checkEntity(*data.entityList[j].get(), data);
}

void Game::getInputs(sf::Event evt) {
    if (evt.type == sf::Event::Closed) {
        Running = false;
        return;
    }
    if (sf::Keyboard::isKeyPressed(_client.getUp()))
        return;
    if (sf::Keyboard::isKeyPressed(_client.getLeft()))
        return;
    if (sf::Keyboard::isKeyPressed(_client.getDown()))
        return;
    if (sf::Keyboard::isKeyPressed(_client.getRight()))
        return;
};


void Game::Loop() {
    Running = true;
    std::cout << '1' << std::endl;
    sf::Event evt;
    while(data.window.isOpen()) {
        if (!Running)
            return;
        data.window.clear(sf::Color::Green);
        Update();
        data.window.display();
        data.window.pollEvent(evt);
        getInputs(evt);
    }
    std::cout << '4' << std::endl;
}

void Game::createEntities() {
    data.entityList.push_back(std::make_unique<Background>());
    data.entityList.erase(data.entityList.begin());
}

void Game::Network() {};

int main(void) {
    try {
        Game rtype;
        rtype.Loop();
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Truc qui fail: " << e.what() << std::endl;
    }
    return 0;
}
