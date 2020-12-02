cmake_minimum_required(VERSION 3.6...3.15)
if (${CMAKE_VERSION} VERSION_LESS 3.15)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else ()
    cmake_policy(VERSION 3.15)
endif ()

project(allegro_tmx_test CXX)

add_executable(ttt main.cpp test.cpp)
target_link_libraries(ttt PRIVATE Catch2::Catch2 fys::allegro_tmx)
target_include_directories(ttt PRIVATE include)
target_compile_features(ttt PRIVATE cxx_std_20)
