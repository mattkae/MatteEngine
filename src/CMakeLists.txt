cmake_minimum_required (VERSION 3.0)
project(Matte)
add_executable(Matte Vector2f.h App.cpp Bone.h Vector4f.cpp Vector3f.cpp MyString.cpp Mesh.cpp Randomizable.h main.cpp Randomizable.cpp Mesh.h App.h Camera.cpp LoadModel.cpp Input.cpp Matrix3x3f.h Logger.h SystemEngine.h Vector3f.h OpenGLUtil.h ModelLoader.cpp Water.h Plane.h TextureInfo.cpp OpenSimplexNoise.cpp Constants.h ImageUtil.h Editor.cpp DirectoryUtil.h Vertex.h OpenSimplexNoise.h Light.h ImageUtil.cpp Light.cpp SceneLoader.h Rectangle.cpp Sphere.cpp Vector2f.cpp ObjectPool.h Bone.cpp SceneLoader.cpp ParticleEmitter.h TextureLoader.cpp TextureInfo.h MeshTexture.h Logger.cpp ModelLoader.h Material.h MeshTexture.cpp GlobalLoaders.h Scene.h Model.cpp StringUtil.h Water.cpp DebugRender.cpp FrameBuffer.cpp Ray.cpp BinarySerializer.cpp Matrix4x4f.cpp Skybox.h MathHelper.h AnimationController.cpp FixedArray.h Vector4f.h GeneratedTexture.h Model.h Matrix3x3f.cpp Terrain.cpp DebugRender.h Shader.cpp AnimationController.h Types.h Ray.h Shader.h Scene.cpp Editor.h ShaderUniformMapping.h Functional.cpp Camera.h FileHelper.h Material.cpp Quad.h Input.h List.h TextureLoader.h BinarySerializer.h Terrain.h GradientSky.cpp GridHelper.h Rectangle.h Skybox.cpp Box.cpp ShaderUniformMapping.cpp Quaternion.h LoadModel.h GridHelper.cpp Matrix4x4f.h GlobalLoaders.cpp FrameBuffer.h Sphere.h Quaternion.cpp ParticleEmitter.cpp TextureUniformConstants.h GradientSky.h Functional.h MyString.h SystemEngine.cpp Box.h UI/UI.cpp UI/UIRenderer.h UI/UIRenderer.cpp UI/UI.h UI/Elements.h System/RenderableSystem.h System/LightSystem.cpp System/MouseInteractable.h System/LightSystem.h System/PhysicsSystem.h System/EntitySystemConstants.h System/MouseInteractable.cpp System/PhysicsSystem.cpp System/RenderableSystem.cpp shaders/Light.shared.cpp)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
endif()
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(freetype REQUIRED)
target_link_libraries(Matte PRIVATE assimp::assimp SOIL glfw GLEW::GLEW freetype)