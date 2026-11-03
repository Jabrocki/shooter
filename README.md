# Shooter

An experimental 3D rendering project written in **C++17** and **OpenGL 3.3 Core**. The project focuses on building the graphics pipeline and reusable rendering abstractions rather than on a finished game.

## Highlights

- OpenGL wrappers for shader programs, vertex/index buffers, vertex arrays, textures, framebuffers and uniform buffers.
- Scene, entity, transform, camera and renderer layers with keyboard and mouse camera controls.
- Shader-based lighting, depth testing, alpha blending, face culling and correct back-to-front rendering of transparent objects.
- Model import through Assimp and texture loading through stb_image.
- Cubemap skybox rendering.
- Off-screen framebuffer rendering and a screen-quad post-processing pipeline with ping-pong framebuffers.
- CMake-based build with project resources copied to the build directory.

## Tech stack

- C++17
- OpenGL 3.3 Core, GLFW and GLAD
- GLM, Assimp, stb_image and nlohmann/json
- CMake

## Build

### Prerequisites

Install CMake, a C++17-capable compiler, OpenGL development libraries, GLFW, GLM, Assimp and nlohmann/json. The project includes source or headers for several dependencies under `external/`, while the current CMake configuration resolves GLFW, GLM, Assimp and nlohmann/json as CMake packages.

On macOS, the required packages can be installed with Homebrew:

```bash
brew install cmake glfw glm assimp nlohmann-json
```

### Configure and run

```bash
cmake -S . -B build
cmake --build build
./build/shooter
```

The executable opens an 800×600 OpenGL window. Use `W`, `A`, `S` and `D` to move the camera, move the mouse to look around, and press `Esc` to exit.

## Project structure

```text
src/
├── Core/       # window, game loop, scene management, camera and renderer
├── OpenGL/     # OpenGL resource and shader abstractions
└── Game/       # project-specific scene setup
res/            # shaders, models and textures
external/       # bundled third-party source and headers
```
