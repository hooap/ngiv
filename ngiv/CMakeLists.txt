cmake_minimum_required(VERSION 3.12.2)

project(ngiv)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release"
    CACHE STRING "Configuration types" FORCE)

add_executable(ngiv
  BasicLevelEditor.cpp
  BasicLevelEditor.h
  Camera2D.cpp
  Camera2D.h
  Camera3D.cpp
  Camera3D.h
  Collision_Box.h
  ColorRGBA8.h
  GameContainer.h
  glad.c
  GLSLProgram.cpp
  GLSLProgram.h
  InputManager.cpp
  InputManager.h
  IOManager.h
  IScreen.cpp
  IScreen.h
  IScreenManager.h
  Mesh.h
  Misc.h
  ModelLoader.cpp
  ModelLoader.h
  NGIV.h
  NGUI.cpp
  NGUI.h
  Noise.cpp
  Noise.h
  OBJ.h
  PhysicsWorld.cpp
  PhysicsWorld.h
  Renderer_2D.cpp
  Renderer_2D.h
  Renderer_3D.cpp
  Renderer_3D.h
  Renderer_Text.cpp
  Renderer_Text.h
  stb_image.h
  TerrainGenerator.cpp
  TerrainGenerator.h
  TextureLoader.cpp
  TextureLoader.h
  Window.cpp
  Window.h
)
# Preprocessor definitions
target_compile_definitions(ngiv PRIVATE
  $<$<CONFIG:Debug>:_MBCS;>
  $<$<CONFIG:Release>:_MBCS;>
)

# SDL check
target_compile_options(ngiv PRIVATE
  "$<$<CONFIG:Debug>:/sdl>"
  "$<$<CONFIG:Release>:/sdl>"
)

# Minimal rebuild
if (MSVC)
  target_compile_options(ngiv PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
    "$<$<CONFIG:Release>:/Gm->"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(ngiv PRIVATE
    ""
  )
endif ()
