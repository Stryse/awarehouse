cmake_minimum_required(VERSION 3.16.0)

# @Author: Doki
################### GOOGLE TEST DOWNLOAD AND COMPLILE ###################
include(${CMAKE_SOURCE_DIR}/dev-tools/cmake/google_test_fetch.cmake)
#########################################################################
#
# ####################### PROJECT awarehouse ############################
project(awarehouse VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
enable_testing()
add_subdirectory(${CMAKE_SOURCE_DIR}/test)
#########################################################################
#
# ######################### C++ Standard ################################

# C++ 17 neccessary
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#########################################################################
#
######################### QT Compiler Config ############################
set(CMAKE_AUTOMOC ON) # Run Meta-Object Compiler (MOC) automatically
set(CMAKE_AUTORCC ON) # Run Resource Compiler (RCC) automatically
set(CMAKE_AUTOUIC ON) # Run User Interface Compiler (UIC) automatically
#########################################################################
#
######################## Locate Qt Dependency ###########################

find_package(QT NAMES Qt6 Qt5 COMPONENTS 
Core 
Quick
QuickControls2 
REQUIRED)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS 
Core 
Quick
QuickControls2 
REQUIRED)

#########################################################################
#
######################## Localization files #############################
# MISSING
#########################################################################
#
################### Include-file gathering macro ########################

include(${CMAKE_SOURCE_DIR}/dev-tools/cmake/include_macro.cmake)

########################### Source Files ################################

set(PROJECT_SOURCES
# Add files here
src/main/main.cpp
src/main/view/viewsource.qrc
resources/resources.qrc
)

#########################################################################
############################## BINARY ###################################

qt_add_executable(awarehouse ${PROJECT_SOURCES})

#########################################################################
######################### Include files (.h) ############################

target_include_directories(awarehouse PUBLIC ${header_dir_list}) # Recursive

######################### Compile Definitons ############################

target_compile_definitions(awarehouse
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

#########################################################################
############################# Link library ##############################

target_link_libraries(awarehouse PRIVATE
Qt${QT_VERSION_MAJOR}::Core
Qt${QT_VERSION_MAJOR}::Quick
Qt${QT_VERSION_MAJOR}::QuickControls2
)

#########################################################################