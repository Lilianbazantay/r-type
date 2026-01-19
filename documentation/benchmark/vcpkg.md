# vcpkg — Technology Benchmark & Justification
[Back to README](../../README.md)

## Overview
vcpkg is the package manager selected to handle all external dependencies of our project.
This choice aligns with the requirements of the project:
- CMake-only build system
- Self-contained dependency management
- Cross-platform support Linux + Windows
- No reliance on system-wide libraries

vcpkg provides a clean, reproducible, and maintainable workflow for our project.

---

## Why We Chose vcpkg

### • Fully compliant with Epitech requirements
vcpkg integrates seamlessly into our CMake toolchain, providing:
- isolated dependency installation
- no system pollution
- simple setup for everyone (prevents the usual “works on my machine” issues)

This ensures that the project can be compiled and tested on fresh machines with zero manual setup.

### • Cross-platform support
The project must run on:
- Linux
- Windows

vcpkg supports both environments uniformly.
Dependencies (SFML, Asio, ...) work identically across platforms using the same command set.

### • Easy onboarding for new contributors
An external only needs to run:
```
./vcpkg install
```
or
```
vcpkg install sfml asio openssl
```
to obtain a fully operational environment.

### • Strong integration with CI/CD
Because vcpkg supports binary caching and deterministic manifests, it fits perfectly with:
- GitHub Actions
- GitLab CI
- self-hosted pipelines

---

## Comparison With Alternatives

### Conan
+ very powerful
- steeper learning curve
- requires profile management
- overkill for our project

### CPM.cmake
+ lightweight and CMake-native
- no real binary management
- manual configuration per dependency
- harder to guarantee reproducibility

vcpkg offers the best balance of simplicity, reproducibility, and tooling quality.

---

## Strengths
- Perfect CMake integration
- Manifest mode (`vcpkg.json`) ensures consistent environments
- Large library catalog (SFML, Asio, OpenSSL, ...)
- Transparent cross-platform workflow
- Easy configuration for CI pipelines
- Clear documentation and active community

---

## Weaknesses
- Initial builds can take time
- Less flexible than Conan for complex enterprise-level setups
- Requires internet on first install
- Some libraries may require manual triplet configuration

These drawbacks are negligible compared to the benefits for our project.

---

## Security Considerations
Using a package manager reduces supply-chain risk by ensuring:
- versions are pinned in `vcpkg.json`
- dependencies are fetched from trusted sources
- CI builds remain reproducible
- no hidden or system-wide libraries are used

---

## Integration in Our Build Pipeline
vcpkg is integrated at the root of our CMake configuration using the provided toolchain file.
It is responsible for installing and configuring:
- **SFML** (rendering, audio, input)
- **Asio** (networking)

---

## Accessibility Considerations
Although a package manager does not directly influence accessibility, vcpkg **indirectly improves accessibility** by:
- eliminating environment-related crashes
- ensuring consistent framerate behavior
- avoiding platform-specific inconsistencies

Stable builds reduce user discomfort and improve cognitive accessibility.
