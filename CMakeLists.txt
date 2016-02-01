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
)

link_directories(
	"externals/glew/lib/"
)

link_libraries(
	"SDL2" "SDL2main"
	"GLEW"
	"assimp"
	"BulletDynamics" "BulletCollision" "LinearMath"
	"jsoncpp_lib"
)

add_executable(ex0
	"examples/ex0/main.cpp"
)

add_executable(ex1
	"examples/ex1/main.cpp"
)
