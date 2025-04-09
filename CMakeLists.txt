cmake_minimum_required(VERSION 3.31)
project(Enjin)

set(CMAKE_CXX_STANDARD 20)

set(DEPS_ROOT deps)
set(SOURCE_ROOT src)
set(TESTS_ROOT src/Tests)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Create an option to switch between a system sdl library and a vendored SDL library
option(SDL_VENDORED "Use vendored libraries" ON)

if (SDL_VENDORED)
    # This assumes you have added SDL as a submodule in vendored/SDL
    add_subdirectory(${DEPS_ROOT}/SDL EXCLUDE_FROM_ALL)
else ()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif ()


# ==================== Naito ====================

set(NAITO_SOURCE
        ${DEPS_ROOT}/imgui/imgui.cpp
        ${DEPS_ROOT}/imgui/imgui_demo.cpp
        ${DEPS_ROOT}/imgui/imgui_draw.cpp
        ${DEPS_ROOT}/imgui/imgui_tables.cpp
        ${DEPS_ROOT}/imgui/imgui_widgets.cpp

        ${DEPS_ROOT}/imgui/backends/imgui_impl_sdl3.cpp
        ${DEPS_ROOT}/imgui/backends/imgui_impl_sdlrenderer3.cpp

        ${SOURCE_ROOT}/Naito/main.cpp
        ${SOURCE_ROOT}/Engine/World.cpp
        ${SOURCE_ROOT}/Engine/World.h
        ${SOURCE_ROOT}/Engine/Cell.cpp
        ${SOURCE_ROOT}/Engine/Cell.h
        ${SOURCE_ROOT}/Engine/Random.cpp
        ${SOURCE_ROOT}/Engine/Random.h
        ${SOURCE_ROOT}/Engine/CellGrid.cpp
        ${SOURCE_ROOT}/Engine/CellGrid.h
        ${SOURCE_ROOT}/Naito/Elements.cpp
        ${SOURCE_ROOT}/Naito/Game.cpp
        ${SOURCE_ROOT}/Naito/Game.h
        ${SOURCE_ROOT}/Engine/Render.h
)

add_executable(Naito
        ${NAITO_SOURCE}
)

set(NAITO_INCLUDE
        ${SOURCE_ROOT}/Engine
        ${SOURCE_ROOT}/Naito

        ${DEPS_ROOT}/SDL/include/SDL3
        ${DEPS_ROOT}/imgui/
        ${DEPS_ROOT}/imgui/backends/
)

target_include_directories(Naito PRIVATE
        ${NAITO_INCLUDE}
)

target_link_libraries(Naito PRIVATE
        SDL3::SDL3
)


# ==================== Tests ====================

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${DEPS_ROOT}/googletest EXCLUDE_FROM_ALL)
enable_testing()


add_executable(Tests
        ${NAITO_SOURCE}

        ${TESTS_ROOT}/RandomTests.cpp
)

target_include_directories(Tests PRIVATE
        ${NAITO_INCLUDE}


        ${SOURCE_ROOT}/Tests

        deps/SDL/include/SDL3
        deps/googletest/googletest/include
)

target_link_libraries(Tests PRIVATE
        SDL3::SDL3
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(Tests)
