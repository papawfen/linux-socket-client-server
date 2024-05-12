cmake_minimum_required(VERSION 3.10)

project(TCP LANGUAGES C CXX)

include_directories(client start)

add_executable(server 
    server/server.cc
    server/main.cc
)

add_executable(client 
    client/client.cc
)