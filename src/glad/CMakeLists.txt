cmake_minimum_required(VERSION 3.25.0)
project(glad)

set(INCLUDE_DIR include)
set(SOURCE_DIR src)
file(GLOB SOURCES ${SOURCE_DIR}/glad.c)
file(GLOB HEADERS ${INCLUDE_DIR}/glad/glad.h)

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})
