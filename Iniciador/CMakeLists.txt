cmake_minimum_required(VERSION 3.17)
project(Iniciador C)

set(CMAKE_C_STANDARD 99)



add_executable(Iniciador main.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Iniciador Threads::Threads)