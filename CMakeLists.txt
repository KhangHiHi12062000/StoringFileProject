cmake_minimum_required(VERSION 3.22)
project(ServerProject C)

set(CMAKE_C_STANDARD 99)
SET(CMAKE_CXX_FLAGS_DEBUG "-g")

add_executable(server server.c file.c tfp.c)


add_executable(client1 client.c tfp.c file.c)
add_executable(client2 client.c tfp.c file.c)
add_executable(client3 client.c tfp.c file.c)
add_executable(client4 client.c)

add_executable(test tfp.c file.c)
