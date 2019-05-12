# Investigations into 3D engines
My goal is that, over time, I'll have something decent worth showing off in ways of a 3D engine. Everything here is implemented in OpenGL 4.3, with both Windows and Mac support. I will be using vcpkg for all of my 3rd party needs, but - this being a custom engine - I want to keep those needs to a minimum.

## Implemented Features
1. Phong Lighting
2. Shadows (Point, Spot, and Directional)
3. Terrain Generation using Simplex Noise
4. .obj and .mtl file support
5. Simple Particles
6. Skyboxes
7. Built-in screenshot mechanism
8. Scene loading from .json files
9. Movable camera

## Planned Features
1. Quicker terrain generation
2. More realistic lighting system with illuminance based off material color
3. Deferred rendering
4. Editor UI for loading models, editing models in worldspace, and saving out to .json
5. Web support using WASM, so anyone can play around with the code on the web
6. Some sort of simple cube physics to begin with
7. Animation of some sort (big investigation into best way to do this)