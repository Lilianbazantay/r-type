// #include "../../server/EntityFactory.hpp"
// #include "IMediatorEntity.hpp"
// #include "../IComponent.hpp"
// #include "../Component/Position.hpp"
// #include "../Component/Velocity.hpp"
// #include "../Component/Direction.hpp"
// #include "../Component/Hp.hpp"
// #include "../Component/Hitbox.hpp"
// #include "../Component/AnimatedSprite.hpp"
// #include "../Component/Clock.hpp"
// #include "../Component/Cooldown.hpp"
// #include "../Component/EntitySpawner.hpp"
// #include "../Component/Gravity.hpp"
// #include "../Component/Sprite.hpp"
// #include "../Component/Sound.hpp"
// #include "../Component/Strategy.hpp"

// #include <iostream>
// #include <fstream>
// #include <memory>
// #include <nlohmann/json.hpp>

// using json = nlohmann::json;

// // Simple entity pour le test
// class TestEntity : public IMediatorEntity {
// public:
//     void run() override {}

//     // 🚀 Correction : on ne copie pas l'objet
//     std::unique_ptr<IMediatorEntity> Clone() override {
//         return std::make_unique<TestEntity>(); // nouvel objet
//     }
// };

// void printComponent(IComponent* comp) {
//     if (!comp) return;

//     switch (comp->GetType()) {
//         case ComponentType::POSITION: {
//             auto p = dynamic_cast<Position*>(comp);
//             auto pos = p->GetPosition();
//             std::cout << "Position: x=" << pos.first << ", y=" << pos.second << "\n";
//             break;
//         }
//         case ComponentType::VELOCITY: {
//             auto v = dynamic_cast<Velocity*>(comp);
//             std::cout << "Velocity: " << v->GetVelocity() << "\n";
//             break;
//         }
//         case ComponentType::DIRECTION: {
//             auto d = dynamic_cast<Direction*>(comp);
//             auto dir = d->GetDirection();
//             std::cout << "Direction: x=" << dir.first << ", y=" << dir.second << "\n";
//             break;
//         }
//         case ComponentType::HP: {
//             auto h = dynamic_cast<Hp*>(comp);
//             std::cout << "HP: " << h->GetHp() << "\n";
//             break;
//         }
//         case ComponentType::HITBOX: {
//             auto hb = dynamic_cast<Hitbox*>(comp);
//             auto size = hb->GetHitboxSize();
//             std::cout << "Hitbox: size=(" << size.first << "," << size.second << "), damage=" << hb->GetDamage() << "\n";
//             break;
//         }
//         case ComponentType::ANIMATED_SPRITE: {
//             auto a = dynamic_cast<AnimatedSprite*>(comp);
//             auto sz = a->getSize();
//             std::cout << "AnimatedSprite: size=(" << sz.first << "," << sz.second << ")\n";
//             break;
//         }
//         case ComponentType::COOLDOWN: {
//             auto c = dynamic_cast<Cooldown*>(comp);
//             std::cout << "Cooldown: length=" << c->GetLenth() << "\n";
//             break;
//         }
//         case ComponentType::ENTITY_SPAWNER: {
//             auto s = dynamic_cast<EntitySpawner*>(comp);
//             std::cout << "EntitySpawner: cooldown=" << s->GetCooldownLenth() 
//                       << ", activated=" << s->IsActivated() << "\n";
//             break;
//         }
//         case ComponentType::STRATEGY: {
//             std::cout << "Strategy component loaded\n";
//             break;
//         }
//         default:
//             std::cout << "Other component type\n";
//             break;
//     }
// }

// int main() {
//     std::ifstream f("../configuration/entity_test.json");
//     if (!f.is_open()) {
//         std::cerr << "Failed to open JSON file\n";
//         return 1;
//     }
//     json j;
//     f >> j;

//     Prototype proto;
//     proto.data = j;

//     std::unordered_map<std::string, Prototype> protos;
//     protos["test_entity"] = proto;

//     EntityFactory factory;
//     factory.registerPrototypes(protos);
//     factory.registerComponentConstructors();

//     TestEntity entity;

//     if (!factory.applyPrototypeToEntity("test_entity", entity)) {
//         std::cerr << "Failed to apply prototype\n";
//         return 1;
//     }

//     // Afficher tous les composants et leurs valeurs
//     std::cout << "=== Components Loaded ===\n";
//     for (auto& type : entity.GetActuatorComponents()) {
//         auto comp = entity.FindComponent(type);
//         printComponent(comp);
//     }

//     return 0;
// }
