cmake_minimum_required(VERSION 3.15)

project(imuclient)

set(CMAKE_CXX_STANDARD 17)

set(BUILD_SHARED_LIBS ON)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE_CXX_17 ON)

find_package(OpenGL REQUIRED)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)

set(SOURCES
        contrib/glad/src/glad.c
        src/gfx/shader.cc
        src/utils/fileloader.cc
        src/main.cc)

add_executable(imuclient ${SOURCES})
add_dependencies(imuclient glfw glm)
target_include_directories(imuclient PUBLIC
        lib/glfw/include
        lib/glm
        contrib/glad/include
        contrib
        include)
target_link_libraries(imuclient glfw OpenGL::GL)