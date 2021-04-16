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

######################## Locate Boost Dependency ########################
set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.71.0 REQUIRED)


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

presenter/cpp/ActorPresenter.cpp
presenter/header/ActorPresenter.h

presenter/cpp/TaskPresenter.cpp
presenter/header/TaskPresenter.h

presenter/cpp/SimulationWindowPresenter.cpp
presenter/header/SimulationWindowPresenter.h

presenter/cpp/MapItemPresenter.cpp
presenter/header/MapItemPresenter.h

presenter/cpp/WarehouseLayoutPresenter.cpp
presenter/header/WarehouseLayoutPresenter.h

presenter/cpp/ChargingStationPresenter.cpp
presenter/header/ChargingStationPresenter.h

presenter/cpp/PodDockPresenter.cpp
presenter/header/PodDockPresenter.h

presenter/cpp/RoadPresenter.cpp
presenter/header/RoadPresenter.h

presenter/cpp/ActorList.cpp
presenter/header/ActorList.h

presenter/cpp/ActorListModel.cpp
presenter/header/ActorListModel.h

presenter/cpp/TaskList.cpp
presenter/header/TaskList.h

presenter/cpp/TaskListModel.cpp
presenter/header/TaskListModel.h

presenter/cpp/ActorListModel.cpp
presenter/header/ActorListModel.h

presenter/cpp/ChargingStationList.cpp
presenter/header/ChargingStationList.h

presenter/cpp/ChargingStationListModel.cpp
presenter/header/ChargingStationListModel.h

presenter/cpp/PodDockList.cpp
presenter/header/PodDockList.h

presenter/cpp/PodDockListModel.cpp
presenter/header/PodDockListModel.h

presenter/cpp/DeliveryStationPresenter.cpp
presenter/header/DeliveryStationPresenter.h

presenter/cpp/DeliveryStationList.cpp
presenter/header/DeliveryStationList.h

presenter/cpp/DeliveryStationListModel.cpp
presenter/header/DeliveryStationListModel.h

presenter/cpp/WarehouseEditorPresenter.cpp
presenter/header/WarehouseEditorPresenter.h

#presenter/cpp/PodList.cpp
#presenter/header/PodList.h

#presenter/cpp/PodListModel.cpp
#presenter/header/PodListModel.h

#presenter/cpp/PodPresenter.cpp
#presenter/header/PodPresenter.h
)

presenter/cpp/WarehouseEditorPresenter.cpp
presenter/header/WarehouseEditorPresenter.h

#presenter/cpp/PodList.cpp
#presenter/header/PodList.h

#presenter/cpp/PodListModel.cpp
#presenter/header/PodListModel.h

#presenter/cpp/PodPresenter.cpp
#presenter/header/PodPresenter.h
)

add_library(OBJ_AWSOURCE_FILES STATIC
# Add CPP files here - those which are !!NOT!! Q_OBJECT
model/cpp/Tile/Tile.cpp
model/cpp/Tile/DeliveryStation.cpp
model/cpp/Warehouse/Warehouse.cpp
model/cpp/Warehouse/State.cpp
model/cpp/Warehouse/Pod/OrderModel.cpp
persistence/cpp/WarehouseFilePersistence.cpp
persistence/cpp/ChargingStationLoader.cpp
persistence/cpp/DeliveryStationLoader.cpp
persistence/cpp/PodDockLoader.cpp
persistence/cpp/OrderLoader.cpp
persistence/cpp/RobotLoader.cpp
model/cpp/Warehouse/Controller/ControllerImpl.cpp
model/cpp/Warehouse/Scheduler/SchedulerImpl.cpp
model/cpp/Warehouse/Network/Network.cpp
model/cpp/Warehouse/Network/NetworkMessage.cpp
model/cpp/Warehouse/Network/NetworkAdapter.cpp
## QOBJECT FILES ##
${QT_QOBJECT_FILES}
)

target_include_directories(OBJ_AWSOURCE_FILES PUBLIC ${header_dir_list} ${Boost_INCLUDE_DIRS})
target_link_libraries(OBJ_AWSOURCE_FILES PRIVATE
Qt${QT_VERSION_MAJOR}::Core
Qt${QT_VERSION_MAJOR}::Quick
Qt${QT_VERSION_MAJOR}::QuickControls2
)

#########################################################################
############################## BINARY ###################################

add_executable(awarehouse main.cpp ${QRC_RESOURCES})

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
