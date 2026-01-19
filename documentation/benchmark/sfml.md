# SFML - Benchmark
[Back to README](../../README.md)

## Overview
SFML was chosen as the **client-side multimedia library** for rendering, input, audio, and windowing.
It provides all features necessary for a 2D arcade shooter while remaining lightweight, accessible, and highly maintainable.

This fits the R-Type client constraints:
- real-time rendering
- starfield, sprites, animations
- sound effects
- cross-platform execution
- clear separation between rendering and logic

---

## Why We Chose SFML

### • Ideal for 2D arcade gameplay
R-Type is fundamentally a **2D horizontal shooter**, and SFML handles:
- textures & sprites
- frame-based animations
- view handling
- timed updates via `sf::Clock`

### • Very simple, readable C++ API
In the project, SFML’s API reduces:
- onboarding time
- friction between subsystems
- errors caused by complex rendering code

### • Mature, stable, documented library
SFML has well-written and comprehensive documentation that allows us to understand and implement features quickly and efficiently.

### • No engine-level constraints
Unlike game engines, SFML:
- does not impose scene graphs
- does not impose physics
- does not impose UI frameworks

This gives us full freedom to build our **own game engine architecture**.

---

## Comparison With Alternatives

### SDL2
+ powerful, widespread
- lower-level C API
- more boilerplate
- less readable documentations and outdated tutorials.
SFML is more suited to a student team environment.

### Raylib
+ modern and elegant
- still younger / smaller community
- less C++-oriented

### SFML’s own Network module
We explicitly **do not** use it for gameplay networking, because:
- the project requires deep UDP control
- SFML network is high-level
- Asio is better suited for high-frequency updates

---

## Strengths
- Simple and intuitive API
- Perfect match for 2D rendering
- Great multimedia support (textures, audio, fonts)
- Large community and stable documentation
- Cross-platform support Linux + Windows
- Clean integration with vcpkg and CMake
- We allready have a lot of experiances with sfml

---

## Weaknesses
- Not optimized for very large GPU workloads
- Does not provide advanced GUI tools
- Network module too limited for real-time games

None of these weaknesses affect our project.

---

## Security Considerations
SFML does not expose significant attack surface except through:
- textures
- audio files
- window events

Mitigations:
- never load untrusted external files over the network
- validate asset formats
- isolate rendering from networking logic

All authoritative logic runs on the server, reducing client-side risk.

---

## Integration in Our Game Engine
SFML belongs to the **Rendering & Input Subsystem** of our engine:
- retrieves player inputs locally
- displays world state provided by the server
- plays sounds
- renders maps, entities, and HUD

SFML is strictly decoupled from:
- networking (Asio)
- ECS game logic
- physics
- protocol handling

This matches the “visible and decoupled subsystems” requirement.

---

## Accessibility Considerations
SFML allows us to integrate:
- scalable UI
- configurable key bindings
- color variations for color blindness
- volume controls and audio balancing
- frame-rate capping to avoid visual discomfort

This directly supports the accessibility of our project.
