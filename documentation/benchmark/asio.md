# Asio - Benchmark

## Overview
Asio is the networking library chosen for the R-Type Online project to implement our **UDP-based multiplayer server** and the **client network layer**.
It provides a **high-performance asynchronous I/O model**, full control on UDP/TCP sockets, and clean integration with modern C++.

This choice aligns with the project requirements:
- UDP-only binary protocol
- Non-blocking server
- Real-time entity updates
- Strong decoupling between engine systems

---

## Why We Chose Asio
Asio fits perfectly with the constraints of a **shoot-them-up**:

### • Asynchronous, event-driven architecture
The server must run *frame after frame* regardless of client delays.
Asio’s **io_context**, asynchronous handlers, and timers allow:
- continuous game-loop execution
- concurrent network operations
- no blocking calls

### • Full control of UDP
Asio gives us:
- raw UDP access
- customizable reliability
- zero hidden buffering
- no “engine magic” masking packet behaviour

This is essential for implementing:
- prediction / reconciliation
- custom packet packing
- loss recovery or acknowledgment

### • Precise control of binary protocol
The subject requires:
- a complete custom binary protocol
- documentation allowing rewrites of clients
- robustness to malformed packets

Asio allows us to build our protocol **byte-perfect** without unnecessary abstraction layers.

---

## Comparison With Alternatives
### SFML Network
+ easy API
- too high-level, hides UDP internals
- no advanced async patterns
- not suitable for highly concurrent servers

### ENet
+ excellent reliable UDP
- hides protocol logic
- less flexible than Asio
- not ideal for custom engine-level behaviour

### Raw BSD Sockets
+ maximal control
- too low-level for a multi-platform CMake project
- requires significant boilerplate
Asio provides the same control with much better engineering quality.

### IPOAC (IP over Avian Carriers)
+ very long communication range
- latency incompatible with real-time gameplay
- extremely low bandwidth
- unpredictable delivery, high potential loss
- no asynchronous model suitable for a game engine

---

## Strengths
- Cross-platform Linux + Windows
- Header-only (simple packaging via vcpkg)
- High performance asynchronous model
- Clear threading model with strands
- Clean integration with modern C++
- Production-quality reliability

---

## Weaknesses
- More complex than SFML network module
- No built-in reliability
- Requires developer discipline to avoid race conditions

---

## Security Considerations
Our protocol must resist:
- malformed packets
- spoofing
- replay
- overflow
- floods

Asio supports these by offering:
- precise packet size control
- explicit error handling
- no hidden buffering
- easy rate-limiting + token bucket implementation
- TLS integration if needed for lobby/authentication

---

## Integration in Our Game Engine
Asio is used inside the **Networking Subsystem** of our custom engine.
Its responsibilities:
- manage UDP sockets
- dispatch incoming messages
- encode/decode binary packets
- communicate with ECS systems

This fits the engine decoupling required:
- Rendering ← SFML
- Game Logic ← server side
- Networking ← Asio

Asio never interacts with graphics or gameplay directly.

---

## Accessibility Considerations
Networking has limited direct accessibility impact, but Asio helps us:
- keep stable framerates even under load (less motion-induced discomfort)
- reduce stutter and desync (better for cognitive disabilities)
- ensure deterministic behavior
