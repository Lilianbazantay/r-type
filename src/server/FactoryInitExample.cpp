#include "ConfigParser.hpp"
#include "EntityFactory.hpp"
#include <iostream>

class DummyMediator : public IMediatorEntity {
public:
    int nextId = 0;
    std::vector<ComponentPtr> actuators;

    int createEntity(const std::string &type) override {
        std::cout << "DummyMediator::createEntity -> type: " << type << "\n";
        return nextId++;
    }

    std::vector<ComponentPtr> GetActuatorComponents() override { return actuators; }
    void AddActuatorComponent(ComponentPtr component) override {
        actuators.push_back(component);
        std::cout << "  Added actuator component type: " << static_cast<int>(component->GetType()) << "\n";
    }

    std::vector<ComponentPtr> GetUnderGoerComponents() override { return {}; }
    void AddUndergoerComponent(ComponentPtr) override {}
    std::vector<MediatorPtr> GetAttachedEntities() override { return {}; }
    void AttachEntity(MediatorPtr) override {}
    void run() override {}
};

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <config.json> <prototype_name>\n";
        std::cout << "Example: ./factory_test player_prototypes.json Player\n";
        return 0;
    }

    std::string cfg = argv[1];
    std::string protoName = argv[2];

    ConfigParser parser;
    if (!parser.loadFile(cfg)) {
        std::cerr << "Failed to load config: " << cfg << "\n";
        return 1;
    }

    EntityFactory factory;
    factory.registerPrototypes(parser.getPrototypes());
    factory.registerComponentConstructors();

    DummyMediator mediator;
    int id = factory.createEntityFromPrototype(protoName, mediator);
    std::cout << "Created entity id: " << id << ", components = " << mediator.actuators.size() << "\n";

    return 0;
}
