cmake_minimum_required(VERSION 3.13)
project(Stressor C)

set(CMAKE_C_STANDARD 99)


include_directories(include)

set(headers include/log.h include/http_get.h include/thread.h)
set(sources src/main.c src/log.c src/http_get.c src/thread.c)

add_executable(${PROJECT_NAME} ${headers} ${sources})


target_link_libraries(${PROJECT_NAME} pthread curl)