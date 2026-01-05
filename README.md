# R-Type — Multiplayer Arcade Shooter

###### An <img src="https://newsroom.ionis-group.com/wp-content/uploads/2023/09/epitech-2023-logo-m.png" height=18/> project

[Project](#project) • [Installation](#installation) • [Usage](#usage) • [Team](#team)

---

<a name="project"></a>
## Project

<a name="project-overview"></a>
### Overview

This project is a full **R-Type multiplayer engine**, implemented in **C++**, using:

- **ASIO** (UDP networking)
- **SFML** (graphics, input, audio)
- A custom **ECS architecture**
- A fully custom **binary client/server protocol**

The goal is to reproduce the behavior of the classic R-Type shooter while supporting multiplayer gameplay.

The project produces three binaries:

- **`r-type_server`** — authoritative server handling the game world
- **`r-type_client`** — player client handling rendering + inputs
- **`r-type_app`** — application used to create new entities

---

<a name="installation"></a>
## Installation

### 1. Clone the repository

```sh
git clone git@github.com:Lilianbazantay/r-type.git r-type
cd r-type
```

### 2. Run the build script

```sh
./installer.sh
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

The **freetype** library, **X11** library and its dependencies and **OpanAL**/**OPENGL** are all library used by the **SFML** library.

*Every library here is imported that way as to not make the project dependant on them and allow it run without being dependant on the user's system.*


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

Multiple clients can connect simultaneously. (max 5)

---

<a name="team"></a>
## Team

| <img src="https://avatars.githubusercontent.com/u/146708420?v=4" width=92>                                                       | <img src="https://avatars.githubusercontent.com/u/146721664?v=4" width=92>                                                         | <img src="https://avatars.githubusercontent.com/u/146714535?v=4" width=92>                                                                      | <img src="https://avatars.githubusercontent.com/u/114913834?v=4" width=92>                                           | <img src="https://avatars.githubusercontent.com/u/146707823?v=4" width=92>                                        |
|----------------------------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------|
| [**Lilian BAZANTAY**](https://github.com/Lilianbazantay)<br/>[lilian.bazantay@epitech.eu]("mailto:lilian.bazantay@epitech.eu") | [**Valentin ROQUEJOFRE**](https://github.com/Valentin22r)<br/>[valentin.roquejofre@epitech.eu]("mailto:valentin.roquejofre@epitech.eu") | [**Antoine QUILLET**](https://github.com/Touf272)<br/>[antoine.quillet@epitech.eu]("mailto:antoine.quillet@epitech.eu") | [**Thibaud LE CREURER**](https://github.com/leTLC)<br/>[thibaud.le-creurer@epitech.eu]("mailto:thibaud.le-creurer@epitech.eu") | [**Arthur DRAHY**](https://github.com/tuturicide)<br/>[arthur.drahy@epitech.eu]("mailto:arthur.drahy@epitech.eu") |