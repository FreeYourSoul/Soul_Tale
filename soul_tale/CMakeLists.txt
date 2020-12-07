cmake_minimum_required(VERSION 3.6...3.15)
if (${CMAKE_VERSION} VERSION_LESS 3.15)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else ()
    cmake_policy(VERSION 3.15)
endif ()

project(Soul_Tale CXX)

add_executable(Soul_Tale
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hud/chat_ui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hud/perf_ui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hud/terminal.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hud/hud_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hud/user_info_ui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/in-arena/arena.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/in-world/world.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/in-world/character.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/in-world/entity_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/launcher/launcher.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/engine_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common/key_map.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common/game_context.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common/option_config.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/common/option_config.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/common/game_context.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/common/key_map.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/engine_manager.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/hud/chat_ui.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/hud/perf_ui.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/hud/terminal.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/hud/hud_manager.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/hud/user_info_ui.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/in-arena/arena.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/in-world/world.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/in-world/character.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/in-world/entity_manager.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/launcher/launcher.hh
        )

target_include_directories(Soul_Tale PRIVATE
        includes
        ${ALLEGRO_INCLUDE_DIRS}
        ${ALLEGRO_IMG_INCLUDE_DIRS}
        ${ALLEGRO_TTF_INCLUDE_DIRS}
        ${ALLEGRO_PRIMITIVES_INCLUDE_DIRS}
        ${ALLEGRO_FONT_INCLUDE_DIRS})
target_link_libraries(Soul_Tale PRIVATE
        ${ALLEGRO_LIBRARIES}
        ${ALLEGRO_IMG_LIBRARIES}
        ${ALLEGRO_TTF_LIBRARIES}
        ${ALLEGRO_FONT_LIBRARIES}
        ${ALLEGRO_PRIMITIVES_LIBRARIES}
        fmt::fmt
        widgetz
        fys::allegro_tmx)
target_compile_features(Soul_Tale PRIVATE cxx_std_20)

