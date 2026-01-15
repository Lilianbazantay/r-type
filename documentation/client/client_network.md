# Client Network Module

## Overview

This document describes how to use the **client-side network module** of the project.
It explains how packets are built, sent, received, and decoded, and how the game engine
is expected to interact with the client network layer.

This document **does not** describe game logic.
It only focuses on **client/server communication**.

---

## Architecture

The client network is composed of three main parts:

```
client/
├── Asio.hpp / Asio.cpp
├── Packet.hpp
├── PacketCodec.hpp / .cpp
├── client.hpp / client.cpp
```

### Responsibilities

| Component        | Responsibility |
|------------------|---------------|
| Asio_network     | Send / receive UDP packets |
| PacketCodec      | Encode & decode binary protocol |
| Client           | Expose simple API to the game engine |
| Game Engine      | Calls Client methods (inputs, start game, etc.) |

---

## Packet Format (Client → Server)

All packets follow this binary layout:

```
[ packetId (16 bits) ]
[ actionType (4 bits) | payloadSize (4 bits) ]
[ payload (N bytes) ]
```

### Action Types

| Action | Value | Payload |
|------|-------|---------|
| INPUT_PRESSED | 0x0 | 1 byte |
| INPUT_RELEASED | 0x1 | 1 byte |
| PLAYER_CONNECT | 0x2 | 6 bytes |
| START_GAME | 0x4 | 0 |
| NEW_CONNECTION | 0x8 | 6 bytes |
| PACKET_LOST | 0xC | 1 byte |
| OK | 0xF | 0 |

### Input Codes

| Input | Value |
|------|-------|
| FIRE | 0 |
| UP | 1 |
| DOWN | 2 |
| LEFT | 3 |
| RIGHT | 4 |

---

## Using the Client API

### Creating the Client

```cpp
Client client(serverIp, serverPort);
client.start();
```

This initializes the UDP socket and starts listening for server packets.

---

### Sending Inputs

The game engine **must not send text commands**.
It directly calls the client API.

```cpp
client.sendInput(true, INPUT_FIRE);   // Fire pressed (INPUT_FIRE = 0)
client.sendInput(false, INPUT_FIRE);  // Fire released (INPUT_FIRE = 0)
```

Internally this builds a binary packet and sends it to the server.

---

### Starting the Game

```cpp
client.sendStartGame();
```

Sends a START_GAME packet with no payload.

---

### Connection Request

```cpp
client.sendConnectionRequest(ipValue, portValue);
```

Payload format:

```
[ IP (4 bytes) ][ PORT (2 bytes) ]
```

---

## Receiving Server Packets

Packets from the server are received asynchronously using ASIO.
They are forwarded to a callback function:

```cpp
void on_receive_callback(const uint8_t* data,
                         size_t size,
                         const udp::endpoint& sender);
```

This callback decodes the packet using:

```cpp
ServerPacket pkt = decodeServerPacket(data, size);
```

The client never blocks while waiting for packets.

To see what will be received, follow the basic protocol in`server_network.md`.