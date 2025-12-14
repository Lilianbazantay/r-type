# Entity Factory & Entity Creation Documentation

This documentation explains the functionality of the **Entity Factory** and entities within the ECS (Entity–Component–System) architecture used in the project. It covers creation, configuration via JSON prototypes, and interaction with components.

---

## Overview

The **Entity Factory** is responsible for creating and configuring game entities. It works using:

- **Prototypes** (defined in JSON files) that describe the components and data of an entity.
- **Component constructors** that dynamically assemble components for each entity.

Entities follow the **Mediator Entity** pattern and have two main types of components:

1. **Actuator components**: interact with the world or respond to inputs (e.g., Position, Sprite, Velocity).
2. **Undergoer components**: represent internal state or properties of the entity (e.g., Hp, Shield).

Each entity can also have **attached entities** (e.g., weapons attached to a player).

---

## IMediatorEntity

`IMediatorEntity` is the base interface for all entities. Its main responsibilities are:

- Managing components (actuator and undergoer)
- Providing cloning functionality (`Clone`)
- Handling entity state (alive, changed, just created)
- Providing thread-safe locking

### Key Methods

- `AddActuatorComponent(std::unique_ptr<IComponent>)`
  Adds an actuator component to the entity.

- `AddUndergoerComponent(std::unique_ptr<IComponent>)`
  Adds an undergoer component to the entity.

- `FindComponent(ComponentType type)`
  Returns a pointer to the component of the given type, or `nullptr` if not found.

- `justCreated()`
  Returns true if the entity was just created; automatically resets to false.

- `hasChanged()`
  Checks if the entity has changed since the last check; automatically resets to false.

- `Alive(bool)` / `is_Alive()`
  Sets or gets the alive state of the entity.

- `lock()` / `unlock()`
  Provides thread-safe locking for entity manipulation.

### Entity Identifiers

Each entity has:

- `id`: unique identifier
- `Entitytype`: integer representing entity type (background, player, enemy, bullet)

The method `is_wanted_entity(int type, int id)` checks if an entity matches a specific type and ID.

---

## Components

Components are the modular data units of entities. They implement the `IComponent` interface and have a type defined in `ComponentType`:

- `HP`, `POSITION`, `VELOCITY`, `DIRECTION`, `HITBOX`, `ATTACK`, etc.

Entities are composed of **unique pointers** to components, which are either actuator or undergoer type.

---

## Entity Types

### Background
- Only actuator components: Position and Sprite.
- Represents the game background.
- Does not require additional logic in `run()`.

### Player
- Actuator: Sprite, Direction, Velocity, Position, Hitbox
- Undergoer: Hp
- Represents the main controllable character.
- `run()` is currently empty, intended for future logic.

### Enemy
- Actuator: Sprite, Direction, Velocity, Position, Hitbox
- Undergoer: Hp
- Represents an BOT-controlled opponent.
- `run()` is currently empty.

### PlayerBullet
- Actuator: Sprite, Direction, Velocity, Position, Hitbox
- Undergoer: Hp
- Represents projectiles fired by the player.
- `run()` is currently empty.

---

## Prototypes and JSON Configuration

Entities can be configured using **JSON prototype files**. Example: `player.json`

- Defines entity type, name, components, and additional data like input mapping and sprites.
- Components are dynamically instantiated in the factory using registered constructors.

### Example JSON Structure

- `entity`: basic entity info (`type`, `name`)
- `components`: defines actuator components with initial values
- `data`: additional configuration like input keys and sprite paths

---

## EntityFactory

The **Entity Factory** manages creation and configuration of entities using prototypes.

### Key Responsibilities

- `registerPrototypes(map<string, Prototype>)`
  Stores JSON prototypes to be applied later.

- `registerComponentConstructors()`
  Registers default constructors for supported component types.

- `registerConstructor(ComponentType type, ConstructorFn fn)`
  Registers a custom constructor for a specific component type.

- `applyPrototypeToEntity(string protoName, IMediatorEntity &entity)`
  Applies a prototype to an entity by adding all its actuator components.

### Component Mapping

`stringToComponentType(string key)` converts a string from JSON to `ComponentType`:

- Example: `"position"` → `ComponentType::POSITION`
- Unknown strings default to `ComponentType::UNKNOWN`.

---

## Workflow Example

1. Load prototypes using `ConfigParser::loadFile("player.json")`.
2. Register prototypes in `EntityFactory`.
3. Register component constructors.
4. Create a new entity (e.g., `Player` object).
5. Apply prototype to the entity using `applyPrototypeToEntity("player_basic", playerEntity)`.
6. The entity now has all components defined in the JSON and is ready to be used in the ECS.

---

## Notes

- Currently, `Clone()` is not implemented for any entity. It is intended to create deep copies of entities.
- All `run()` methods in entities are empty but reserved for entity-specific logic during game updates.
- Actuator vs. Undergoer distinction allows flexible ECS systems to operate on entities based on their role and state.
