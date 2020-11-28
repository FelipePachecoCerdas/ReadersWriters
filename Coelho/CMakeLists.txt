cmake_minimum_required(VERSION 3.17)
project(Coelho C)

set(CMAKE_C_STANDARD 99)

add_executable(Coelho main.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Coelho Threads::Threads)