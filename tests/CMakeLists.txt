file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    *.cpp
)

# No tests yet → don't fail
if(NOT TEST_SOURCES)
    message(STATUS "No test files found")
    return()
endif()

add_executable(emu_tests ${TEST_SOURCES})

target_compile_features(emu_tests PRIVATE cxx_std_23)

target_include_directories(emu_tests
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

if(TARGET emu_lib)
    target_link_libraries(emu_tests PRIVATE emu_lib gtest_main SFML::Graphics SFML::Window SFML::System)
else()
    target_link_libraries(emu_tests PRIVATE gtest_main SFML::Graphics SFML::Window SFML::System)
endif()

include(GoogleTest)
gtest_discover_tests(emu_tests)