cmake_minimum_required(VERSION 3.15)
project(imuclient)
set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

set(GLFW_DIR        ${PROJECT_SOURCE_DIR}/lib/glfw)
set(GLFW_BINARY_DIR ${GLFW_DIR}/bin)
set(GLM_DIR         ${PROJECT_SOURCE_DIR}/lib/glm)
set(GLM_BINARY_DIR  ${GLM_DIR}/bin)

ExternalProject_Add(
        glfw
        PREFIX         ${GLFW_DIR}
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.3
        INSTALL_DIR    ${GLFW_BINARY_DIR}
        CMAKE_ARGS     -DCMAKE_INSTALL_PREFIX=${GLFW_BINARY_DIR}
        -DBUILD_SHARED_LIBS=ON -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF
        -DGLFW_BUILD_DOCS=OFF)
ExternalProject_Add(
        glm
        PREFIX         ${GLM_DIR}
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG        0.9.9.3
        INSTALL_DIR    ${GLM_BINARY_DIR}
        CMAKE_ARGS     -DCMAKE_INSTALL_PREFIX=${GLM_BINARY_DIR}
        -DGLM_TEST_ENABLE=OFF)

include_directories(
        ${GLFW_BINARY_DIR}/include
        ${GLM_BINARY_DIR}/include
        contrib/glad/include
        contrib
        include)
link_directories(${GLM_BINARY_DIR}/lib)
link_libraries(${GLFW_BINARY_DIR}/lib/libglfw.3.dylib)

set(SOURCES
        contrib/glad/src/glad.c
        src/gfx/shader.cc
        src/utils/fileloader.cc
        src/main.cc)

add_executable(imuclient ${SOURCES})
add_dependencies(imuclient glfw glm)