cmake_minimum_required(VERSION 3.25.1 FATAL_ERROR)

project(mwfractal LANGUAGES CXX)

set_property(GLOBAL PROPERTY CMAKE_EXECUTE_TIMEOUT 90)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

set(VERSION_MAJOR 2)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(VERSION_PRE_RELEASE "-alpha")

configure_file(cmake/config.h.in ${PROJECT_BINARY_DIR}/config.h @ONLY)

# Boost
find_package(Boost REQUIRED COMPONENTS program_options)

# ImageMagick
find_package(ImageMagick REQUIRED COMPONENTS Magick++)
add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16)
add_definitions(-DMAGICKCORE_HDRI_ENABLE=0)

include_directories(
    PRIVATE
        ${PROJECT_BINARY_DIR}
        ${Boost_INCLUDE_DIRS}
        ${ImageMagick_INCLUDE_DIRS}
)

add_executable(
    mwfractal
    src/main.cpp
    src/utils.cpp
    src/ProgramOptions.cpp
    src/colourizers/Colourizer.cpp
    src/colourizers/MW_Darts_Violet_to_Red.cpp
    src/colourizers/NEM_Violet_to_Red.cpp
    src/colourizers/NEM_Violet_to_Red_Lightness.cpp
    src/colourizers/NEM_White_to_Black.cpp
    src/generators/Generator.cpp
    src/generators/Julia.cpp
    src/generators/Julia3.cpp
    src/generators/JuliaPowExp.cpp
    src/generators/Mandelbrot.cpp
)

target_link_libraries(
    mwfractal
    PRIVATE
        Boost::program_options
        ${ImageMagick_LIBRARIES}
)

string(TIMESTAMP ts)
message(STATUS "Build complete on ${CMAKE_HOST_SYSTEM} at ${ts}")
