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

The project produces two binaries:

- **`r-type_server`** — authoritative server handling the game world
- **`r-type_client`** — player client handling rendering + inputs

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
```

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