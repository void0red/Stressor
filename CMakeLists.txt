cmake_minimum_required(VERSION 3.13)
project(Stressor C)

set(CMAKE_C_STANDARD 99)


include_directories(include)

set(headers include/thread_pool.h include/log.h include/http.h)
set(sources src/main.c src/thread_pool.c src/log.c src/http.c)

add_executable(${PROJECT_NAME} ${headers} ${sources})


target_link_libraries(${PROJECT_NAME} pthread curl)