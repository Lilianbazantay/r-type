# Client Network Module

## Overview

This document describes how to use the **server-side network module** of the project.
It explains how packets are built, sent, received, and decoded, and how the game engine
is expected to interact with the client network layer.

This document **does not** describe game logic.
It only focuses on **client/server communication**.

---

## Architecture

The client network is composed of three main parts:

```
server/
├── server.hpp / server.cpp
├── encoder.hpp
├── protocol.hpp / .cpp
├── ServerGame.hpp / .cpp
```

### Responsibilities

| Component        | Responsibility |
|------------------|---------------|
| server     | Send / receive UDP packets |
| encoder      | Encode binary protocol  for client|
| protocol           | decode client binary protocol |
| serverGame      | Calls game methods (entity creation, deletion, etc.) |

---

## Packet Format (Client → Server)

All packets follow this binary layout:

```
[ packetId (16 bits) ]
[ payloadSize (4 bits) | ActionType (4 bits) ]
[ payload (N bytes) ]
```

### Action Types

| Action | Value | Payload |
|------|-------|---------|
| ENTITY_CREATED | 0x0 | 6 byte |
| ENTITY_MOVED | 0x1 | 6 byte |
| ENTITY_DELETED | 0x2 | 3 bytes |
| START_GAME | 0x4 | 0 |
| PACKET_LOST | 0xC | 0 bytes |
| SERVER_SHUTDOWN | 0xD | 0 byte |
| OK | 0xF | 0 |

### Payload Bytes

| Value | bytes |
|------|-------|
| ENTITY_TYPE | 1 |
| ENTITY_ID | 2 |
| POSITION | 3 |

6 bytes = take all 3 values;
3 bytes = take entity_type + entity_id
0 bytes = take none
---

## Using the Client API

### Sending Inputs

The server **must not receive text commands and will only send binary command**.

To see what is expected to be received follow the protocol in `client_network.md`.