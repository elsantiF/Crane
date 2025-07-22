# Crane

Crane is a multi-platform 2D game engine written in C++ using ECS architecture. It is designed to be modular, simple and efficient. It aims to be developer-friendly.

## Features

- Entity Component System (ECS)
- 2D rendering
- Physics integration (Box2D)
- Editor
- Profiling tools (Tracy)

## Compiling

To compile Crane, you need to have CMake installed. You can build the engine using the following commands:

```bash
git clone --recurse-submodules https://github.com/elsantiF/Crane.git
cd Crane
mkdir build && cd build
cmake ..
cmake --build .
```

## License

Crane is licensed under the MIT License. See the [LICENSE](LICENSE.txt) file for more details.
