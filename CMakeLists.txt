cmake_minimum_required(VERSION 3.3.0)


project(Project VERSION 0.1.0)


include(ExternalProject)

include_directories(include)

# Set install directory
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist CACHE PATH ${CMAKE_SOURCE_DIR}/dist FORCE)
set(CMAKE_BUILD_TYPE "Debug")
if(WIN32)
    set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "Debug" FORCE)
endif()

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)

include(BuildGLEW)
include(BuildGLFW)
include(BuildGLM)

# Project
set(EXEC Project)

file(GLOB SRC src/*.cpp)

add_executable(${EXEC} ${SRC} "include/Cylinder.h")

target_link_libraries(${EXEC} OpenGL::GL glew_s glfw glm)

list(APPEND BIN ${EXEC})
# end Project

# install files to install location
install(TARGETS ${BIN} DESTINATION ${CMAKE_INSTALL_PREFIX})