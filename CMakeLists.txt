cmake_minimum_required(VERSION 3.14)
project(FlowMeter)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules")

include(CodeCoverage)
append_coverage_compiler_flags()

enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/test
        ${CMAKE_CURRENT_SOURCE_DIR}/FlowMeter)

add_executable(
        test_flowmeter
        FlowMeter/FlowMeter.cpp
        test/test_flowmeter.cc
        test/test_flagAfterDelta.cc
        test/test_flagEvery.cc
        test/test_flagEvery.cc
        test/test_flagAt.cc
        test/test_getTicksInPast.cc
)

target_link_libraries(
        test_flowmeter
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_flowmeter)

setup_target_for_coverage_gcovr_html(
        NAME coverage
        EXECUTABLE ctest -j ${n_cores}
        DEPENDENCIES
        test_flowmeter
        EXCLUDE "build/_deps/*" "test/*")