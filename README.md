# Building out a 3D engine in OpenGL/GLFW/C++ from Scratch
I've been working on this project for a while now in order to investigate how difficult it is to build a fairly verbose 3D engine in C++ with OpenGL and GLFW. My goal is to eventually strip out any need for third party libraries (other than GLFW and OpenGL). The third party libraries currently in use are:

- Freetype (for reading in .ttf files)
- GLM (although we have out own math library now, so we can begin stripping that out)
- SOIL (for imaging reading)

Over time, the goal is to remove these libraries and hand-roll the things that are specific to my use case.

## Capabilities of the Engine
### File Format
The engine reads in `.matte` files, exampels of which can be found in the `assets/scenes` directory. These files are in pseudo-json format, with a few exceptions, since I found JSON to be a little to bloated for my use case.

### Skyboxes and Terrain
Terrain is being generated using a variation of Simplex Noise, and Skyboxes are being rendered via a Cube Map
[alt text](readme-assets/terrain.png)

### Lights
The engine supports three different kinds of lights: directional, point, and spot, as well as shadow mapping for all except spot light. (At one point, spot light shadow mapping was supported, but it has since broken)

### .OBJ and .MTL file Loading
Instead of using assimp, I've hand-spun my own .obj file loader in C++. At the moment, texture-loading isn't working.
[alt text](readme-assets/models.png)

In the future, we will also be able to load a number of primitives straight out of the gate, and, at the moment, the user can specify a `sphere` object to be place in the world.

### Model Selection
Using ray-casting, we're able to select models in the scene, and show the bounding box for the model. In the future, we will be trying to also move the box using directional arrows and the mouse cursor.

### Particle Emitters
The engine also features a fairly nuanced particle system, in which users can specify how the color and speed of a particle changes over time. These particles are drawn via instancing, and I've found that 100,000 particles on screen at the same time causes very little slow down.
[alt text](readme-assets/particles.png)

### UI System
The UI system is currently in its infancy but, as the project progresses, it should become a lot more built-out. In its current state, users are able to edit the location of models in the scene via text input.
[alt text](readme-assets/ui.png)

### Deferred rendering
The user can turn deferred rendering on in the `.matte` configuration, but some things may break.

## Upcoming
1. Animation System, which will require a bunch of investigation into how .obj files can accomodate it.
2. Point Light Shadow Maps
3. Writing the current scene to `.matte` files
4. Removing remaining GLM code from the engine
5. Adding texture mapping back into the engine
6. More widgets for the UI