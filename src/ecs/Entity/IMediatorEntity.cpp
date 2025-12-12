#pragma once
#include "IMediatorEntity.hpp"

class MediatorEntity : public IMediatorEntity {
public:
    int createEntity(const std::string &type) override {
        return nextId++;
    }

    std::vector<ComponentPtr> GetActuatorComponents() override { return actuatorComponents; }
    void AddActuatorComponent(ComponentPtr component) override { actuatorComponents.push_back(component); }

    std::vector<ComponentPtr> GetUnderGoerComponents() override { return undergoerComponents; }
    void AddUndergoerComponent(ComponentPtr component) override { undergoerComponents.push_back(component); }

    std::vector<MediatorPtr> GetAttachedEntities() override { return attachedEntities; }
    void AttachEntity(MediatorPtr ent) override { attachedEntities.push_back(ent); }

    void run() override {
    }

private:
    int nextId = 0;
    std::vector<ComponentPtr> actuatorComponents;
    std::vector<ComponentPtr> undergoerComponents;
    std::vector<MediatorPtr> attachedEntities;
};
