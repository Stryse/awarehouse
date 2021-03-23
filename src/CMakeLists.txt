#########################################################################
# @Author: Doki                                                         #
#########################################################################
cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

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
########################### Source Files ################################

set(QRC_RESOURCES
# Add qrc here
view/viewsource.qrc
../resources/resources.qrc
)

set(QT_QOBJECT_FILES
# Add CPP and Header files here - those which !!ARE!! Q_OBJECT
model/cpp/Simulator/Simulator.cpp
model/header/Simulator/Simulator.h
model/cpp/Warehouse/WarehouseManager.cpp
model/header/Warehouse/WarehouseManager.h
)

add_library(OBJ_AWSOURCE_FILES STATIC
# Add CPP files here - those which are !!NOT!! Q_OBJECT
model/cpp/Tile/Tile.cpp
#presenter/cpp/Presenter.cpp
presenter/cpp/Actors.cpp
#presenter/cpp/OutlinerListModel.cpp
#presenter/cpp/Order.cpp
#presenter/cpp/OrderListModel.cpp
model/cpp/Warehouse/Warehouse.cpp
model/cpp/Warehouse/State.cpp
model/cpp/Warehouse/Pod/OrderModel.cpp
## QOBJECT FILES ##
${QT_QOBJECT_FILES}
)

target_include_directories(OBJ_AWSOURCE_FILES PUBLIC ${header_dir_list})
target_link_libraries(OBJ_AWSOURCE_FILES PRIVATE
Qt${QT_VERSION_MAJOR}::Core
Qt${QT_VERSION_MAJOR}::Quick
Qt${QT_VERSION_MAJOR}::QuickControls2
)

#########################################################################
############################## BINARY ###################################

qt_add_executable(awarehouse main.cpp ${QRC_RESOURCES})

#########################################################################
######################### Include files (.h) ############################

target_include_directories(awarehouse PUBLIC ${header_dir_list})

######################### Compile Definitons ############################

target_compile_definitions(awarehouse
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

#########################################################################
############################# Link library ##############################

target_link_libraries(awarehouse PRIVATE
Qt${QT_VERSION_MAJOR}::Core
Qt${QT_VERSION_MAJOR}::Quick
Qt${QT_VERSION_MAJOR}::QuickControls2
OBJ_AWSOURCE_FILES
)
#########################################################################
