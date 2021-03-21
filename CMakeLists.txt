##############################################################################
# @Author: Doki                                                              #
##############################################################################
cmake_minimum_required(VERSION 3.16.0)
project(awarehouse VERSION 1.0.0 LANGUAGES CXX)
######################### Include file macro #################################
include(${CMAKE_SOURCE_DIR}/dev-tools/cmake/include_macro.cmake)
########################## Awarehouse Source #################################
add_subdirectory(src)
############################### Unit Tests ###################################
enable_testing()
include(${CMAKE_SOURCE_DIR}/dev-tools/cmake/google_test_fetch.cmake)
add_subdirectory(test/GTest)
add_subdirectory(test/QtTest)
##############################################################################