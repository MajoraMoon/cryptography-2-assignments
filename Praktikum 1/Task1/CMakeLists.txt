cmake_minimum_required(VERSION 3.10)
project(praktikum_1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Suche alle Quellen
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Executable erstellen
add_executable(praktikum_1 ${SOURCES})

# Botan hinzufügen
find_package(PkgConfig REQUIRED)
pkg_check_modules(BOTAN REQUIRED botan-2)

# Botan-Header und Linker-Flags hinzufügen
target_include_directories(praktikum_1 PRIVATE ${BOTAN_INCLUDE_DIRS})
target_link_libraries(praktikum_1 PRIVATE ${BOTAN_LIBRARIES})
