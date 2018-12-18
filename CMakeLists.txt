cmake_minimum_required(VERSION 3.3)

project(FreeSouls_World CXX)

set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lprotobuf -lpthread -lboost_system ")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(PROJECT_STATIC_RUNTIME FALSE CACHE BOOL "Use statically linked standard/runtime libraries?")

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)
find_package(TMXLITE REQUIRED)

find_package(SFML 2 REQUIRED system window graphics network audio)

enable_testing()

add_subdirectory(Protobuf)

include_directories(
        ${SFML2_INCLUDE_DIR}
        ${TMXLITE_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/cmake-build-debug/Protobuf
        ${CMAKE_SOURCE_DIR}/Protobuf
        ${CMAKE_SOURCE_DIR}/FySMQ/include/queue
        ${CMAKE_SOURCE_DIR}/FySMQ/include/bus
        ${CMAKE_SOURCE_DIR}/FySMemoryManager/include
        ${CMAKE_SOURCE_DIR}/Client/include/cl
        ${CMAKE_SOURCE_DIR}/Client/include/cl/listener
        ${CMAKE_SOURCE_DIR}/Client/include/cl/tmx
        ${CMAKE_SOURCE_DIR}/Client/include/network
        ${CMAKE_SOURCE_DIR}/Client/include/game
        ${CMAKE_SOURCE_DIR}/Utils/src
        ${CMAKE_SOURCE_DIR}/FySMemoryPool/include/
        )

set(SOURCE_FILES_MM FySMemoryPool/include/MemoryPool.hpp)

set(SOURCE_FILES_MQ
        ${CMAKE_SOURCE_DIR}/FySMQ/include/queue/LockFreeQueue.hh
        ${CMAKE_SOURCE_DIR}/FySMQ/include/queue/QueueContainer.hh
        ${CMAKE_SOURCE_DIR}/FySMQ/include/queue/LockingQueue.hh
        ${CMAKE_SOURCE_DIR}/FySMQ/include/bus/BusListener.hh
        ${CMAKE_SOURCE_DIR}/FySMQ/include/bus/FysBus.hh FySMemoryPool/include/Sprite.hh FySMemoryPool/src/Sprite.cpp)

set(SOURCE_FILES_GAME
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
        ${CMAKE_SOURCE_DIR}/Client/include/cl/tmx/Map.hh
        ${CMAKE_SOURCE_DIR}/Client/include/cl/tmx/SFMLOrthogonalLayer.hpp
        ${CMAKE_SOURCE_DIR}/Client/src/cl/Game.cpp
        ${CMAKE_SOURCE_DIR}/Client/include/cl/Game.hh)

set(SOURCE_FILES_UTILS
        ${CMAKE_SOURCE_DIR}/Utils/src/PlayerDataType.hh)

set(SOURCE_FILES_ALL
        ${SOURCE_PROTOBUF}
        ${SOURCE_FILES_UTILS}
        ${SOURCE_FILES_MQ}
        ${SOURCE_FILES_MM}
        ${SOURCE_FILES_GAME})

set(SOURCE_FILES
        ${SOURCE_FILES_ALL}
        ${CMAKE_SOURCE_DIR}/Client/src/main.cpp Client/src/cl/tmx/GameRenderer.cpp Client/include/cl/tmx/GameRenderer.hh)

### Client ###
add_executable(FreeSouls_Client ${SOURCE_FILES})
target_compile_options(FreeSouls_Client PRIVATE -O2 -fstack-protector-all -g)
if ( UNIX )
    target_compile_options(FreeSouls_Client PRIVATE -Wall -Wpedantic -Wextra -Wno-deprecated-declarations)
    message( STATUS Set Warnings flag : ${CMAKE_CXX_FLAGS})
endif()
target_link_libraries(FreeSouls_Client
        proto
        ${SFML_LIBRARIES}
        ${TMXLITE_LIBRARIES})