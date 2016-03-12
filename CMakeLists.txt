cmake_minimum_required(VERSION 2.6)

project(many_examples_opengl)

# OpenGL
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
	include_directories(${OPENGL_INCLUDE_DIRS})
	link_libraries(${OPENGL_LIBRARIES})
endif()

# try to use C++ 11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# SDL2
add_subdirectory("externals/SDL2")

#ASSIMP
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory("externals/assimp")

#Bullet
option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)
add_subdirectory("externals/bullet")

#jsoncpp
option(BUILD_STATIC_LIBS ON)
add_subdirectory("externals/jsoncpp")


include_directories(
	"externals/glew/include"
	"externals/SDL2/include"
	"externals/glm"
	"externals/stb"
        "externals/assimp/include"
	"externals/bullet/src"
	"externals/jsoncpp/include"
	"externals/rapidxml"
        "common"
)

add_library("common_utils" STATIC
        "common/utils.hpp"
        "common/utils.cpp"
        "common/camera.hpp"
        "common/camera.cpp"
)

link_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/externals/glew/lib"
)

link_libraries(
	"SDL2" "SDL2main"
	"GLEW"
	"assimp"
	"BulletDynamics" "BulletCollision" "LinearMath"
	"jsoncpp_lib"
        "common_utils"
)

add_executable(ex0
	"examples/ex0/main.cpp"
)

add_executable(ex1
	"examples/ex1/main.cpp"
        "examples/ex1/shaders/vert.glsl"
        "examples/ex1/shaders/frag.glsl"
)

add_executable(ex2
        "examples/ex2/main.cpp"
        "examples/ex2/shaders/vert.glsl"
        "examples/ex2/shaders/frag.glsl"
)

add_executable(ex3
        "examples/ex3/main.cpp"
)

add_executable(ex4
        "examples/ex4/main.cpp"
        "examples/ex4/shaders/vert.glsl"
        "examples/ex4/shaders/frag.glsl"
)

add_executable(ex5
        "examples/ex5/main.cpp"
        "examples/ex5/shaders/vert.glsl"
        "examples/ex5/shaders/frag.glsl"
)

add_executable(ex6
        "examples/ex6/main.cpp"
        "examples/ex6/shaders/vert.glsl"
        "examples/ex6/shaders/frag.glsl"
)

add_executable(ex7
        "examples/ex7/main.cpp"
        "examples/ex7/shaders/vert.glsl"
        "examples/ex7/shaders/frag.glsl"
)

add_executable(ex8
        "examples/ex8/main.cpp"
)
