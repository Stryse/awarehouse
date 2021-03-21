########################### SETUP QT Unit testing ###########################
cmake_minimum_required(VERSION 3.16.0)
#############################################################################
find_package(Qt6 COMPONENTS Test REQUIRED)

#############################################################################
######################### QT Compiler Config ################################
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON) # Run Meta-Object Compiler (MOC) automatically
enable_testing(true)
############################### Test sources ################################
set(QT_AWH_T_SOURCES
SimulatorTest.cpp
)

set(QOBJECT_HEADER_FILES
../../src/model/header/Simulator/Simulator.h
)
################################### Binary####################################
qt_add_executable(awh-qt-test ${QT_AWH_T_SOURCES} ${QOBJECT_HEADER_FILES})
add_test(NAME awh-qt-test COMMAND awh-qt-test)
target_include_directories(awh-test PUBLIC ${header_dir_list} )
############################## Link libraries ################################
target_link_libraries(awh-qt-test PRIVATE Qt::Test OBJ_AWSOURCE_FILES) 
# ***************************************************************************#