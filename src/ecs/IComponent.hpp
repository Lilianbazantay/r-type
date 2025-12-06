#pragma once

enum class ComponentType {
    HP,
    POSITION,
    UNKNOWN
};

class IComponent
{
    protected:
        ComponentType _type = ComponentType::UNKNOWN;

    public:
        virtual ~IComponent() = default;

        // SET/GET
        virtual ComponentType GetType() const { return _type; }
        virtual void SetType(ComponentType type) { _type = type; }
};
