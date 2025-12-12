#pragma once
#include "../IComponent.hpp"
#include <vector>
#include <memory>
#include <string>

class IMediatorEntity;
using IMediatorEntityPtr = std::shared_ptr<IMediatorEntity>;

class IMediatorEntity {
public:
    using ComponentPtr = std::shared_ptr<IComponent>;
    using MediatorPtr  = IMediatorEntityPtr;

    virtual ~IMediatorEntity() = default;

    virtual void run() = 0;
    virtual int createEntity(const std::string &type) = 0;

    virtual std::vector<ComponentPtr> GetActuatorComponents() = 0;
    virtual void AddActuatorComponent(ComponentPtr component) = 0;

    virtual std::vector<ComponentPtr> GetUnderGoerComponents() = 0;
    virtual void AddUndergoerComponent(ComponentPtr component) = 0;

    virtual std::vector<MediatorPtr> GetAttachedEntities() = 0;
    virtual void AttachEntity(MediatorPtr ent) = 0;
};
