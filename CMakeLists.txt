cmake_minimum_required(VERSION 3.14)

project(GB_Emulator
    VERSION 0.1
    LANGUAGES C CXX
)

# ===============================
# Compiler Settings
# ===============================

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are forbidden. Use build/ directory.")
endif()
# ===============================
# Output Directories
# ===============================

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ===============================
# Auto Reconfigure on File Change
# ===============================

set(CMAKE_CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests
)

# ===============================
# Collect Sources
# ===============================

file(GLOB_RECURSE EMU_SOURCES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.c
)

list(FILTER EMU_SOURCES EXCLUDE REGEX ".*/main\\.cpp$")

# ===============================
# Main Executable
# ===============================

add_executable(gb_emu
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)

target_include_directories(gb_emu
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(gb_emu PRIVATE cxx_std_23)

# ===============================
# Core Library (If Exists)
# ===============================

if(EMU_SOURCES)

    add_library(emu_lib STATIC ${EMU_SOURCES})

    target_include_directories(emu_lib
        PUBLIC ${PROJECT_SOURCE_DIR}/include
    )

    target_compile_features(emu_lib PUBLIC cxx_std_23)

    target_link_libraries(gb_emu PRIVATE emu_lib)

endif()

# ===============================
# Testing
# ===============================

include(CTest)
enable_testing()

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

if(EXISTS ${PROJECT_SOURCE_DIR}/tests/CMakeLists.txt)
    add_subdirectory(tests)
endif()