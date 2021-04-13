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
find_package(Boost 1.75.0 REQUIRED)


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
presenter/cpp/Actors.cpp
presenter/header/Actors.h
presenter/cpp/Order.cpp
presenter/header/Order.h
presenter/header/SimulationWindow_Presenter.h
presenter/cpp/SimulationWindow_Presenter.cpp
presenter/header/OutlinerModel.h
presenter/cpp/OutlinerModel.cpp
presenter/header/MapItemPresenter.h
presenter/cpp/MapItemPresenter.cpp
presenter/header/WarehouseLayoutPresenter.h
presenter/cpp/WarehouseLayoutPresenter.cpp
presenter/header/ChargingStationPresenter.h
presenter/cpp/ChargingStationPresenter.cpp
presenter/header/PodDockPresenter.h
presenter/cpp/PodDockPresenter.cpp
presenter/header/RoadPresenter.h
presenter/cpp/RoadPresenter.cpp
presenter/header/OutlinerList.h
presenter/cpp/OutlinerList.cpp
presenter/cpp/OrderOutlinerModel.cpp
presenter/header/OrderOutlinerModel.h
presenter/header/OrderOutlinerList.h
presenter/cpp/OrderOutlinerList.cpp
presenter/header/MapTablePresenterModel.h
presenter/cpp/MapTablePresenterModel.cpp
presenter/header/MapTableList.h
presenter/cpp/MapTableList.cpp
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
