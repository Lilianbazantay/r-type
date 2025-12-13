//#include "Game.hpp"
//#include <iostream>
//
//Game::Game()
//: window("SFML - Fenetre encapsulée", 800, 600),
//  client("127.0.0.1", 4242, &netBuffer)
//{
//    player.setRadius(30.f);
//    player.setFillColor(sf::Color::Green);
//    player.setOrigin(player.getRadius(), player.getRadius());
//    player.setPosition(400.f, 300.f);
//
//    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
//        std::cerr << "Text Font not found\n";
//        useText = false;
//    } else {
//        useText = true;
//        info.setFont(font);
//        info.setCharacterSize(14);
//        info.setFillColor(sf::Color::White);
//        info.setPosition(5.f, 5.f);
//    }
//
//    client.start();
//    input.setClient(&client);
//}
//
//void Game::run()
//{
//    sf::Clock clock;
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.get().pollEvent(event)) {
//            if (event.type == sf::Event::Closed)
//                window.close();
//            else if (event.type == sf::Event::Resized)
//                window.handleResize(event.size.width, event.size.height);
//
//            input.processEvent(event);
//        }
//
//        float time = clock.restart().asSeconds();
//        update(time);
//        processNetworkPackets();
//
//        window.get().clear(sf::Color(50, 50, 50));
//        window.get().draw(player);
//        if (useText)
//            window.get().draw(info);
//        window.get().display();
//    }
//}
//
//void Game::update(float time)
//{
//    const float speed = 200.f;
//    sf::Vector2f move{0.f, 0.f};
//
//    if (input.isKeyPressed(sf::Keyboard::Left) || input.isKeyPressed(sf::Keyboard::Q))
//        move.x -= speed * time;
//    if (input.isKeyPressed(sf::Keyboard::Right) || input.isKeyPressed(sf::Keyboard::D))
//        move.x += speed * time;
//    if (input.isKeyPressed(sf::Keyboard::Up) || input.isKeyPressed(sf::Keyboard::Z))
//        move.y -= speed * time;
//    if (input.isKeyPressed(sf::Keyboard::Down) || input.isKeyPressed(sf::Keyboard::S))
//        move.y += speed * time;
//
//    player.move(move);
//
//    std::string typed = input.consumeTextBuffer();
//    if (!typed.empty()) {
//        typedText += typed;
//        if (typedText.size() > 128)
//            typedText.erase(0, typedText.size() - 128);
//    }
//
//    if (useText) {
//        info.setString(
//            "Position: (" + std::to_string((int)player.getPosition().x) + ", " +
//            std::to_string((int)player.getPosition().y) + ")\n" +
//            "Texte enter: " + typedText + "\n" +
//            "Touch pressed: " + pressedKeysToString() + "\n"
//        );
//    }
//}
//
//void Game::processNetworkPackets()
//{
//    auto packets = netBuffer.popAllPackets();
//    for (const auto& pkt : packets) {
//        // PUT ENDPOINT ECS
//        std::cout << "[GAME] Received packet id=" << pkt.packetId
//                  << " action=" << (int)pkt.actionType
//                  << " entity=" << (int)pkt.entityType
//                  << " entityId=" << pkt.entityId
//                  << " pos=(" << pkt.posX << "," << pkt.posY << ")\n";
//    }
//}
//
//std::string Game::pressedKeysToString() const
//{
//    std::string out;
//    if (input.isKeyPressed(sf::Keyboard::Left) || input.isKeyPressed(sf::Keyboard::Q))
//        out += "Left ";
//    if (input.isKeyPressed(sf::Keyboard::Right) || input.isKeyPressed(sf::Keyboard::D))
//        out += "Right ";
//    if (input.isKeyPressed(sf::Keyboard::Up) || input.isKeyPressed(sf::Keyboard::Z))
//        out += "Up ";
//    if (input.isKeyPressed(sf::Keyboard::Down) || input.isKeyPressed(sf::Keyboard::S))
//        out += "Down ";
//    return out.empty() ? "-" : out;
//}
//