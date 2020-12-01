cmake_minimum_required(VERSION 3.6...3.15)
if (${CMAKE_VERSION} VERSION_LESS 3.15)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else ()
    cmake_policy(VERSION 3.15)
endif ()

project(allegro_tmx CXX)

find_package(tmxlite REQUIRED)
find_package(PNG REQUIRED)

add_library(allegro_tmx INTERFACE)
add_library(fys::allegro_tmx ALIAS allegro_tmx)
target_link_libraries(allegro_tmx INTERFACE
        ${PNG_LIBRARIES}
        ${ALLEGRO_LIBRARIES}
        ${ALLEGRO_IMG_LIBRARIES}
        ${TMXLITE_LIBRARIES})
target_include_directories(allegro_tmx INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${TMXLITE_HEADERS})
target_compile_features(allegro_tmx INTERFACE cxx_std_20)

if (BUILD_TESTING)
    find_package(Catch2 REQUIRED)
    enable_testing()
    add_subdirectory(test)
endif()




