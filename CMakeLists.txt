cmake_minimum_required(VERSION 3.3)

project(FreeSouls_World CXX)

set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lprotobuf -lboost_system -pthread")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

SET(PROJECT_STATIC_RUNTIME FALSE CACHE BOOL "Use statically linked standard/runtime libraries?")

set(CMAKE_CXX_STANDARD 17)

if ( UNIX )
    set(CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -Wno-deprecated-declarations")
    message( STATUS Set Warnings flag : ${CMAKE_CXX_FLAGS})
endif()

enable_testing()

add_subdirectory(Protobuf)

include_directories(
        ${TMXLITE_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/cmake-build-debug/Protobuf
        ${CMAKE_SOURCE_DIR}/FySMQ/include/queue
        ${CMAKE_SOURCE_DIR}/FySMQ/include/bus
        ${CMAKE_SOURCE_DIR}/FySMemoryManager/include
        ${CMAKE_SOURCE_DIR}/Client/include/cl
        ${CMAKE_SOURCE_DIR}/Client/include/cl/listener
        ${CMAKE_SOURCE_DIR}/Client/include/cl/tmx
        ${CMAKE_SOURCE_DIR}/Client/include/network
        ${CMAKE_SOURCE_DIR}/Client/include/game
        ${CMAKE_SOURCE_DIR}/Utils/src)

set(SOURCE_FILES_MM)

set(SOURCE_FILES_MQ
        ${CMAKE_SOURCE_DIR}/FySMQ/include/queue/LockFreeQueue.hh
        ${CMAKE_SOURCE_DIR}/FySMQ/include/queue/QueueContainer.hh
        ${CMAKE_SOURCE_DIR}/FySMQ/include/queue/LockingQueue.hh
        ${CMAKE_SOURCE_DIR}/FySMQ/include/bus/BusListener.hh
        ${CMAKE_SOURCE_DIR}/FySMQ/include/bus/FysBus.hh)

set(SOURCE_FILES_SRV
        ${CMAKE_SOURCE_DIR}/Client/src/cl/listener/GamingListener.cpp
        ${CMAKE_SOURCE_DIR}/Client/include/cl/listener/GamingListener.hh
        ${CMAKE_SOURCE_DIR}/Client/src/cl/Context.cpp
        ${CMAKE_SOURCE_DIR}/Client/include/cl/Context.hh
        ${CMAKE_SOURCE_DIR}/Client/src/network/SessionManager.cpp
        ${CMAKE_SOURCE_DIR}/Client/include/network/SessionManager.hh
        ${CMAKE_SOURCE_DIR}/Client/src/network/TcpConnection.cpp
        ${CMAKE_SOURCE_DIR}/Client/include/network/TcpConnection.hh
        ${CMAKE_SOURCE_DIR}/Client/src/cl/tmx/MapBuilder.cpp
        ${CMAKE_SOURCE_DIR}/Client/include/cl/tmx/MapBuilder.hh
        ${CMAKE_SOURCE_DIR}/Client/src/cl/tmx/Map.cpp
        ${CMAKE_SOURCE_DIR}/Client/include/cl/tmx/Map.hh)

set(SOURCE_FILES_UTILS
        ${CMAKE_SOURCE_DIR}/Utils/src/PlayerDataType.hh)

set(SOURCE_FILES_ALL
        ${SOURCE_PROTOBUF}
        ${SOURCE_FILES_UTILS}
        ${SOURCE_FILES_MQ}
        ${SOURCE_FILES_MM}
        ${SOURCE_FILES_SRV})

set(SOURCE_FILES
        ${SOURCE_FILES_ALL}
        ${CMAKE_SOURCE_DIR}/Client/src/main.cpp
        )

### Client ###
add_executable(FreeSouls_Client ${SOURCE_FILES})
target_link_libraries(FreeSouls_Client proto ${TMXLITE_LIBRARIES})