cmake_minimum_required(VERSION 3.10)

project(GBA_Emulator
        VERSION 0.1
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE LIB_FILES src/*.cpp)

# This collects all implementation files (excluding main.cpp files)
# This creates a library with shared code that both executables can use
list(FILTER LIB_FILES EXCLUDE REGEX ".*main\\.cpp$")
## Create a static library from implementation files
## This allows code reuse without duplicate main() functions
add_library(emu_lib STATIC ${LIB_FILES})

## Create the main executable
## Links only src/main.cpp + the library
add_executable(gb_emu src/main.cpp)
target_link_libraries(gb_emu emu_lib)