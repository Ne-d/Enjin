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
option(MYGAME_VENDORED "Use vendored libraries" ON)

if(MYGAME_VENDORED)
    # This assumes you have added SDL as a submodule in vendored/SDL
    add_subdirectory(${DEPENDENCIES_ROOT}/SDL EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

add_executable(Enjin
        ${SOURCE_ROOT}/main.cpp
)

target_include_directories(Enjin PRIVATE
        deps/SDL/include/SDL3
)

target_link_libraries(Enjin PRIVATE
    SDL3::SDL3
)