#############################################################################
# Author: Doki                                                              #
########################### SETUP UNIT TESTING ##############################
cmake_minimum_required(VERSION 3.16.0)
include(GoogleTest)
include(Dart)
############################# SOURCE FILES ##################################
set(T_SOURCES
# Add files here
test_main.cpp
Example_test.cpp
Tile_test.cpp 
)
#############################################################################
add_executable(awh-test ${T_SOURCES})
###################### INCLUDE FILES ########################################
target_include_directories(awh-test PUBLIC ${header_dir_list} )
#############################################################################
target_link_libraries(awh-test PUBLIC gtest gmock OBJ_AWSOURCE_FILES)
gtest_discover_tests(awh-test)
#############################################################################