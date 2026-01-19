# R-Type — Multiplayer Arcade Shooter

###### An <img src="https://newsroom.ionis-group.com/wp-content/uploads/2023/09/epitech-2023-logo-m.png" height=18/> project

[Project](#project) • [Installation](#installation) • [Usage](#usage) • [Features](#features) • [Architecture](#architecture) • [Team](#team)

---

<a name="project"></a>
## Project

<a name="project-overview"></a>
### Overview

This project is a full **R-Type multiplayer engine** with a **game engine**, implemented in **C++**, using:

- **ASIO** (UDP networking)
- **SFML** (graphics, input, audio)
- A custom **ECS architecture**
- A fully custom **binary client/server protocol**

The goal is to reproduce the behavior of the classic R-Type shooter while supporting multiplayer gameplay.

The project is separated into 2 main parts: **Software Architecture & Prototype** and **Advanced Topics**. For more information on the subject, you can download the subject in the [wiki](https://github.com/EpitechPGE3-2025/G-CPP-500-REN-5-2-rtype-4/wiki).

For our part we chose to work on the **game engine** and decided to add multiple features; including:
- an application to create and customise entities.
- a second game with its own server and client to prove the efficency of our engine.

The project produces fixe binaries:

- **`r-type_server`** — authoritative server handling the game world
- **`r-type_client`** — player client handling rendering + inputs
- **`r-type_app`** — application used to create new entities
- **`flappy_server`** — an authoritative server handling the world of the second game
- **`flappy_client`** — player client for the second game

---

<a name="installation"></a>
## Installation

To install the project, you can refer yourself to [the guide](https://github.com/EpitechPGE3-2025/G-CPP-500-REN-5-2-rtype-4/wiki/Deployement)

### 1. Clone the repository

```sh
git clone git@github.com:Lilianbazantay/r-type.git r-type
cd r-type
```

### 2. Run the build script

Linux:
```sh
sudo ./scripts/linux.sh
```

Windows:
```ps1
./scripts/msvc.ps1
./scripts/windows.ps1
```

This installs dependencies (via vcpkg) and generates:

```
./r-type_client
./r-type_server
./r-type_app
```
*<u>Note:</u>
The installation script will install **system-wide dependencies**. These includes:*
- basic developement tools: used to compile the program and run it
- `cmake`: our build system
- `ninja`: a build system paired with cmake to improve compilation speed
- `freetype`: a library that renders font
- `X11`: windowing system for Unix-based system
- `XCB`: library to enable connection with the X11 library
- `XRandR`: X11 extension to handle the screen's configuration dynamically
- `XI`: X11 extension to handle touchpads inputs
- `XCursor`: X11 mouse cursor manager
- `udev`: a linux device manager to detect and manage hardware dynamically.
- `OpenAL`: audio API used for spatial sound playback
- `OpenGL`: graphic API to render in both 2D and 3D. Creates a grpahical context.
- `libtool`: a build tool to abstract static library creation

The **freetype** library, **X11** library and its dependencies and **OpenAL**/**OPENGL** are all library used by the **SFML** library.

*Every library here is imported that way as to not make the project dependant on them and allow it run without being dependant on the user's system.*

---

<a name="features"></a>
## Features

### Engine
- Custom **Entity Component System (ECS)**
- Deterministic **authoritative server**
- Custom **binary UDP protocol**
- Hot-reloadable entity definitions
- Cross-platform architecture (Linux focused)

### Networking
- Client/server model over **UDP (ASIO)**
- Lag compensation & client-side prediction
- Entity state synchronization
- Server-side collision & game logic
- Support for up to **4 concurrent players**

### Games
- **R-Type** multiplayer arcade shooter
- **Flappy-like** second game demonstrating engine reusability

### Tools
- **vcpkg**
  - Ensures a consistent development environment across all machines
  - Automatically installs and manages all dependencies and submodules
- **r-type_app**
  - Create and customize entities
  - Edit components and behaviors
  - Export directly to the engine format

---

<a name="architecture"></a>
## Architecture

The engine follows a **data-oriented ECS design**:

- **[Factory](documentation/ecs/Factory.md)** is a [design pattern](https://refactoring.guru/design-patterns) that create all entities
- **Entities** are unique IDs
- **[Components](documentation/ecs/Component.md)** are plain data structures
- **[Systems](documentation/ecs/System.md)** operate on component sets

### [Server](documentation/developer/server_network.md)
- Owns the authoritative game state
- Runs physics, collisions, AI and game rules
- Sends snapshots to clients

### [Client](documentation/developer/client_network.md)
- Handles rendering, inputs, audio
- Applies prediction & interpolation
- Synchronizes entities with the server

### Protocol
- Fully custom binary protocol
- Optimized packet size
- Explicit serialization/deserialization
- Stateless messages when possible

---

<a name="usage"></a>
## Usage

### Start the server

```sh
./r-type_server -p <free port on your machine>
```

### Start a client

```sh
./r-type_client -i <IP of the server machine> -p <same port as server>
```

Multiple clients can connect simultaneously. (max 4)

---

<a name="team"></a>
## Team

| <img src="https://avatars.githubusercontent.com/u/146708420?v=4" width=92> | <img src="https://avatars.githubusercontent.com/u/146721664?v=4" width=92> | <img src="https://avatars.githubusercontent.com/u/146714535?v=4" width=92> | <img src="https://avatars.githubusercontent.com/u/114913834?v=4" width=92> | <img src="https://avatars.githubusercontent.com/u/146707823?v=4" width=92> |
|---|---|---|---|---|
| [**Lilian BAZANTAY**](https://github.com/Lilianbazantay)<br/>lilian.bazantay@epitech.eu | [**Valentin ROQUEJOFRE**](https://github.com/Valentin22r)<br/>valentin.roquejofre@epitech.eu | [**Antoine QUILLET**](https://github.com/Touf272)<br/>antoine.quillet@epitech.eu | [**Thibaud LE CREURER**](https://github.com/leTLC)<br/>thibaud.le-creurer@epitech.eu | [**Arthur DRAHY**](https://github.com/tuturicide)<br/>arthur.drahy@epitech.eu |

---

## License

This project is developed as an **Epitech educational project** between the 01/09/2025 and the 25/01/2026.
It is not intended for commercial use.
All rights reserved.
