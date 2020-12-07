cmake_minimum_required(VERSION 3.6...3.15)
if (${CMAKE_VERSION} VERSION_LESS 3.15)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else ()
    cmake_policy(VERSION 3.15)
endif ()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(Soul_Tale CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

find_package(fmt REQUIRED)
find_package(tmxlite REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Catch2 REQUIRED)

enable_testing()

pkg_check_modules(ALLEGRO REQUIRED allegro-5)
pkg_check_modules(ALLEGRO_IMG REQUIRED allegro_image-5)
pkg_check_modules(ALLEGRO_TTF REQUIRED allegro_ttf-5)
pkg_check_modules(ALLEGRO_FONT REQUIRED allegro_font-5)
pkg_check_modules(ALLEGRO_PRIMITIVES REQUIRED allegro_primitives-5)

add_subdirectory(allegro_tmx)
add_subdirectory(soul_tale)

