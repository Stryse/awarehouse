cmake_minimum_required(VERSION 3.16.0)

################### GOOGLE TEST DOWNLOAD AND COMPLILE ###################
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
                 "${CMAKE_BINARY_DIR}/googletest-build"
)
#########################################################################
#
# @Author: Doki
# ####################### PROJECT awarehouse ############################
project(awarehouse VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
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

find_package(QT NAMES Qt6 Qt5 COMPONENTS      Core 
                                              Quick
                                              QuickControls2 
                                              REQUIRED)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core 
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

MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list *.h)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES(header_dir_list)
list(LENGTH header_dir_list header_dir_list_count)
message(STATUS "[INFO] Found ${header_dir_list_count} header directories.")
########################### Source Files ################################
set(PROJECT_SOURCES

src/main/main.cpp
src/main/view/viewsource.qrc
resources/resources.qrc
)

#########################################################################
############################## BINARY ###################################
qt_add_executable(awarehouse ${PROJECT_SOURCES})
#########################################################################
######################### Include files (.h) ############################
target_include_directories(
    awarehouse
    PUBLIC
    ${header_dir_list} # Recursive
)
######################### Compile Definitons ############################
target_compile_definitions(awarehouse
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
#########################################################################
############################# Link library ##############################
target_link_libraries(awarehouse PRIVATE
Qt${QT_VERSION_MAJOR}::Core
Qt${QT_VERSION_MAJOR}::Quick
Qt${QT_VERSION_MAJOR}::QuickControls2
gtest 
gmock_main)
#########################################################################

######################## PROJECT awarehouseTests ########################
#########################################################################