#set the minimum version requied
cmake_minimum_required(VERSION 3.5)

#set name
project(chat_system)

#set cpp standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-m32")
set(CMAKE_C_FLAGS "-m32")

add_definitions("-static")
set(CMAKE_EXE_LINKER_FLAGS "-static")

set(SOURCES
    src/main.cpp
    src/chatclient.cpp
)

add_executable(chat_system ${SOURCES})

target_include_directories(chat_system
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

if(WIN32)
  target_link_libraries(chat_system ws2_32)
endif()

# use to set compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)