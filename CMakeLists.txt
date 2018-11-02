project(FileStorage)

cmake_minimum_required(VERSION 3.09)
set(CMAKE_CXX_STANDARD 17)

set(OUT ${CMAKE_SOURCE_DIR}/out)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}${CMAKE_SOURCE_DIR}/cmake_modules)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -pthread -lstdc++fs")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

add_subdirectory(lib)
add_subdirectory(client)
add_subdirectory(server)