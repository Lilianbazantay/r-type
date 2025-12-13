//#pragma once
//
//#include <SFML/Graphics.hpp>
//#include <string>
//
//#include "Window.hpp"
//#include "InputManager.hpp"
//#include "../client.hpp"
//#include "../NetworkBuffer.hpp"
//
//class Game
//{
//private:
//    WindowManager window;
//    InputManager input;
//
//    sf::CircleShape player;
//
//    bool useText = false;
//    sf::Font font;
//    sf::Text info;
//    std::string typedText;
//
//    NetworkBuffer netBuffer;
//    Client client;
//
//    void update(float dt);
//    void processNetworkPackets();
//    std::string pressedKeysToString() const;
//
//public:
//    Game();
//    void run();
//};
//