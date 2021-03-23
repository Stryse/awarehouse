#############################################################################
# Author: Doki                                                              #
########################### SETUP UNIT TESTING ##############################
cmake_minimum_required(VERSION 3.16.0)
include(GoogleTest)
include(Dart)
# C++ 17 neccessary
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
############################# SOURCE FILES ##################################
set(T_SOURCES
# Add files here
test_main.cpp
Example_test.cpp
Tile_test.cpp 
Body_test.cpp
Agent_test.cpp
ObservableNavEnvironment_test.cpp
)
#############################################################################
add_executable(awh-test ${T_SOURCES})
###################### INCLUDE FILES ########################################
target_include_directories(awh-test PUBLIC ${header_dir_list} )
#############################################################################
target_link_libraries(awh-test PUBLIC 
gtest 
gmock 
OBJ_AWSOURCE_FILES 
)
gtest_discover_tests(awh-test)
# **************************************************************************#