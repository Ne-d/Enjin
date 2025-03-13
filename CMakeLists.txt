cmake_minimum_required(VERSION 3.31)
project(Enjin)

set(CMAKE_CXX_STANDARD 20)

set(DEPENDENCIES_ROOT deps)
set(SOURCE_ROOT src)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Create an option to switch between a system sdl library and a vendored SDL library
option(SDL_VENDORED "Use vendored libraries" ON)

if (SDL_VENDORED)
    # This assumes you have added SDL as a submodule in vendored/SDL
    add_subdirectory(${DEPENDENCIES_ROOT}/SDL EXCLUDE_FROM_ALL)
else ()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif ()

add_executable(Naito
        ${SOURCE_ROOT}/Naito/main.cpp
        ${SOURCE_ROOT}/Engine/World.cpp
        ${SOURCE_ROOT}/Engine/World.h
        ${SOURCE_ROOT}/Engine/Cell.cpp
        ${SOURCE_ROOT}/Engine/Cell.h
        ${SOURCE_ROOT}/Engine/Random.cpp
        ${SOURCE_ROOT}/Engine/Random.h
        ${SOURCE_ROOT}/Engine/CellGrid.cpp
        ${SOURCE_ROOT}/Engine/CellGrid.h
)

target_include_directories(Naito PRIVATE
        ${SOURCE_ROOT}/Engine
        ${SOURCE_ROOT}/Naito
        deps/SDL/include/SDL3
)

target_link_libraries(Naito PRIVATE
        SDL3::SDL3
)