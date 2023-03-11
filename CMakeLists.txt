cmake_minimum_required(VERSION 3.25)
project(OpenDraw)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

file(GLOB CORE_SOURCE ${SRC_DIR}/libs/*.cpp)
file(GLOB CORE_HEADER ${SRC_DIR}/include/*.hpp)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(assimp REQUIRED)
find_package(OpenCV REQUIRED)

add_subdirectory(${SRC_DIR}/glad)
add_subdirectory(${SRC_DIR}/glm)
add_subdirectory(${SRC_DIR}/imgui)

add_executable(${PROJECT_NAME}
    ${SRC_DIR}/main.cpp
    ${CORE_SOURCE} ${CORE_HEADER})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${SRC_DIR}/include/
    ${OpenCV_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
    SDL2::Main SDL2::Image
    glad glm imgui assimp
    ${OpenCV_LIBS})
