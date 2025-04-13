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

if(WIN32)

message(STATUS "Configuring Botan for Windows (MinGW)")
        # Pfade zu deiner Botan-Installation
        set(BOTAN_INCLUDE_DIR "C:/botan/build/include/public")
        set(BOTAN_LIB_DIR "C:/botan")
        set(BOTAN_LIB_NAME "botan-3")
    
        # Includes und Libpfade setzen
        include_directories(${BOTAN_INCLUDE_DIR})
        link_directories(${BOTAN_LIB_DIR})
    
        # Mit Botan linken
        target_link_libraries(praktikum_1 PRIVATE ${BOTAN_LIB_NAME})
else()

# Botan hinzufügen
find_package(PkgConfig REQUIRED)
pkg_check_modules(BOTAN REQUIRED botan-2)

# Botan-Header und Linker-Flags hinzufügen
target_include_directories(praktikum_1 PRIVATE ${BOTAN_INCLUDE_DIRS})
target_link_libraries(praktikum_1 PRIVATE ${BOTAN_LIBRARIES})

endif()



