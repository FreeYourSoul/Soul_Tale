cmake_minimum_required(VERSION 3.6...3.15)
if (${CMAKE_VERSION} VERSION_LESS 3.15)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else ()
    cmake_policy(VERSION 3.15)
endif ()

project(Soul_Tale CXX)

add_executable(Soul_Tale
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_include_directories(Soul_Tale PRIVATE
        ${ALLEGRO_INCLUDE_DIRS}
        ${ALLEGRO_FONT_INCLUDE_DIRS})
target_link_libraries(Soul_Tale PRIVATE
        ${ALLEGRO_LIBRARIES}
        ${ALLEGRO_IMG_LIBRARIES}
        ${ALLEGRO_FONT_LIBRARIES}
        fys::allegro_tmx)
target_compile_features(Soul_Tale PRIVATE cxx_std_20)

