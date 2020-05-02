cmake_minimum_required(VERSION 3.15)

project(imuclient)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR};${CMAKE_MODULE_PATH}")

set(CMAKE_CXX_STANDARD 17)
set(BUILD_SHARED_LIBS ON)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE_CXX_17 ON)

find_package(OpenGL REQUIRED)
find_package(LibUSB REQUIRED)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)

set(SOURCES
        lib/glad/src/glad.c
        src/gfx/camera.cc
        src/gfx/cube.cc
        src/gfx/framebuffer.cc
        src/gfx/mesh.cc
        src/gfx/model.cc
        src/gfx/shader.cc
        src/gfx/texture.cc
        src/gfx/image.cc
        src/utils/fileloader.cc
        src/peripheral/peripheral.cc
        src/main.cc)

add_executable(imuclient ${SOURCES})
add_dependencies(imuclient glfw glm)
target_include_directories(imuclient PRIVATE
        ${LIBUSB_INCLUDE_DIR}
        lib/glfw/include
        lib/glm
        lib/glad/include
        lib/
        contrib
        include)
target_link_libraries(imuclient glfw OpenGL::GL ${LIBUSB_LIBRARIES})

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
