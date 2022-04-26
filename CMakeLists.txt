cmake_minimum_required(VERSION 3.7)
project(bloomProject)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp GLAD/src/glad.c)
set(HEADER_FILES shader.h mesh.h)

set(dir ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${dir}/build)

include_directories(include)

#Include library files
include_directories(GLAD/include)
include_directories(glm/include)

FIND_PACKAGE(ASSIMP REQUIRED)
LINK_DIRECTORIES(/usr/local/include)

if(NOT ASSIMP_FOUND)
    message (FATAL_ERROR "ASSIMP not found!" )
endif(NOT ASSIMP_FOUND)
if(ASSIMP_FOUND)
    include_directories(${ASSIMP_INCLUDE_DIR})
endif(ASSIMP_FOUND)

#set(ASSIMP_DIR /usr/local/include/assimp)
#include_directories(${ASSIMP_DIR})
include_directories(${dir}/assimp)
set(ASSIMP_LINK /usr/local/lib/libassimp.5.0.0.dylib)
link_libraries(${ASSIMP_LINK})

set(BUILD_ASSIMP_TOOLS  ON)
set(ASSIMP_BUILD_STATIC_LIB ON)

file(COPY resources DESTINATION ${dir}/build)

#Add deps
add_subdirectory(dependencies)

#add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} glfw )
