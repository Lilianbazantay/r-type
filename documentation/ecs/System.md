# Systems creation, import and functionnalities
[Back to README](../../README.md)

The Entity–Component–System (ECS) architecture separates data (Components) from behavior (Systems) to achieve high modularity and flexible gameplay logic.
This document explains how to create Systems, how they interact with the ECS, and how to correctly import and use them.

## How to create a system
A System represents a processing unit that applies logic to entities containing specific sets of components.
All systems **must** inherit from the base class `ISystem` and override its core processing method.

### ISystem class
The `ISystem` interface class provides:
- A mechanism to declare required and optional component types.
- A method to check whether an entity meets the System’s requirements.
- A callback method (`executeEntity`) that derived Systems must implement.
- A public method (`checkEntity`) used by the ECS to evaluate entities and trigger System logic.

Here is the `ISystem` class, also avaliable in src/ecs/System/
```
class ISystem {
    private:
        bool checkRequirements(IMediatorEntity&);
    protected:
        virtual void executeEntity(IMediatorEntity&, relevant_data_t&) = 0;
        std::vector<ComponentType> requiedComponents;
        std::vector<ComponentType> optionnalComponents;
    public:
        ~ISystem() = default;

        void addRequiredComponentType(std::shared_ptr<IComponent>);
        void addRequiredComponentType(std::vector<ComponentType>);
        void addRequiredComponentType(ComponentType);

        void addOptionnalComponentType(ComponentType);

        void checkEntity(IMediatorEntity&, relevant_data_t&);
};
```

### Required & optionnal components
Systems operate only on entities that contain the correct set of components.
This set is split into:

#### Required components
**A required component is mandatory.**
If even one is missing, the System will ignore the entity.

You can add required component types using:
```
void addRequiredComponentType(ComponentType type);
void addRequiredComponentType(std::shared_ptr<IComponent> component);
void addRequiredComponentType(std::vector<ComponentType> types);
```
Internally, `checkRequirements()` compares the system’s `requiedComponents` with the list of components present in the entity:
```
bool ISystem::checkRequirements(IMediatorEntity& entity);
```
#### Optionnal components
Optional components are not required for an entity to be processed, but the System may use them if present.

Add them using:
```
void addOptionnalComponentType(ComponentType type);
```
Optional components are not checked in checkRequirements().
They serve as “extensions” that the System may use, if the entity possesses it, during execution.

## Importation into the ECS
Once a System is created, it must be registered with the ECS so it can begin processing entities.

ECS supports automatic System discovery. To ensure your system will be used, simply:
- Inherit from ISystem
- Provide a constructor
- Ensure the System is registered (e.g., via a factory or reflection mechanism)

The ECS will detect and instantiate your System automatically.


## System functions & callbacks

### ISystem inheritance
All Systems must inherit from `ISystem` and implement:
```
void executeEntity(IMediatorEntity& entity, relevant_data_t& data) override;
```
This method contains the **System’s processing logic**.
The ECS calls `checkEntity()` for every entity, and if requirements are satisfied, `executeEntity()` is executed.

Key inherited functions include:
- `addRequiredComponentType(...)`
    Declares components that must exist for the System to run on an entity.

- `addOptionnalComponentType(...)`
    Declares components that the System may use but does not require.

- `checkEntity(IMediatorEntity&, relevant_data_t&)`
    Called by ECS; internally runs checkRequirements() then executeEntity().

- `executeEntity(IMediatorEntity&, relevant_data_t&)` (**to override**)
    The main logic of the System.

Systems have two protected vectors:
- `requiedComponents` is checked automatically before system execution.
- `optionnalComponents` is available for your system to check manually inside executeEntity

### Custom functions
Derived systems may implement additional utility functions specific to their behavior.
For example, a collision system might implement:
```
bool checkCollison(...);
bool checkLayers(...);
```
These functions are fully custom and **not managed** by the ECS; they only support your internal logic during `executeEntity`.

## Example: Creating a custom system
Below is a minimal example showing how to implement a real System:
```
class CollisionSystem : public ISystem {
    private:
        void executeEntity(IMediatorEntity&, relevant_data_t &) override;
        bool checkCollison(std::pair<float, float>, std::pair<float, float>,
                           std::pair<float, float>, std::pair<float, float>);
        bool checkLayers(std::vector<int>, std::vector<int>);
    public:
        CollisionSystem();
};
```

### Typical usage
During ECS updates:
```
for (auto& entity : ecs.getEntities()) {
    collisionSystem.checkEntity(entity, data);
}
```
The ECS triggers the System only for entities containing both `Transform` and `Collider`, and optionally gives access to `RigidBody`.
