cmake_minimum_required(VERSION 3.17)
project(hash C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0)
pkg_check_modules(XXHASH REQUIRED IMPORTED_TARGET libxxhash)

add_executable(hash main.c vprint.c vprint.h source_handler.c source_handler.h copy_handler.c copy_handler.h)
target_link_libraries(hash PUBLIC PkgConfig::GLIB PkgConfig::XXHASH)