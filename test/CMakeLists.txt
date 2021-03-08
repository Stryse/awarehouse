cmake_minimum_required(VERSION 3.16.0)

# Author: Doki
##################### SETUP UNIT TESTING #######################
include(GoogleTest)
include(Dart)
######################### SOURCE FILES #########################
set(T_SOURCES 
# Add files here
)
################################################################
add_executable(awh-test ${T_SOURCES})
target_link_libraries(awh-test PUBLIC gtest gmock)
gtest_discover_tests(awh-test)
################################################################