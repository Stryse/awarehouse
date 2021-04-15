import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import ActorList           1.0
import ChargingStationList 1.0
import PodDockList         1.0
import DeliveryStationList 1.0

Item {
    id: root

    readonly property real  aspectRatio: 16/9

    readonly property int rows:    SimPresenter.layout.rows
    readonly property int columns: SimPresenter.layout.columns

    property int   cellSpacing: 1
    property real  cellScale:   zoomArea.zoomScale
    property real  cellSize:    Math.round(Math.min(height / rows    - cellSpacing,
                                                    width  / columns - cellSpacing) * cellScale)

    property int mapWidth:  columns * cellSize + (columns-1) * cellSpacing
    property int mapHeight: rows    * cellSize + (rows   -1) * cellSpacing

    anchors.centerIn: parent
    height:           parent.height * 0.7
    width:            height * aspectRatio

    Material.background: Material.primary
    Material.elevation:  6

    ListModel {
        id: placeholderModel

        ListElement { rowIdx: 1; columnIdx: 1; imgSource: "qrc:/placeholder_amogus.png"; rotationAngle: 35}
        ListElement { rowIdx: 2; columnIdx: 0; imgSource: "qrc:/placeholder_amogus.png"; rotationAngle: 170}
    }

    Component {
        id: baseComponent

//        Image {
//            id: tileImg

//            Layout.preferredHeight: root.cellSize
//            Layout.preferredWidth:  root.cellSize

//            source: imgSource //EDIT
//        }
        Rectangle {
            id: baseRectangle

            Layout.alignment: Qt.AlignCenter

            Layout.preferredHeight: root.cellSize
            Layout.preferredWidth:  root.cellSize

            color: Material.accent
        }
    }

    Component {
        id: actorComponent

        Image {
            id: actorImg

            x: model.column * (root.cellSize + root.cellSpacing)
            y: model.row    * (root.cellSize + root.cellSpacing)

            width:  root.cellSize
            height: root.cellSize

            source: model.image

            rotation: model.rotation
        }
    }

    Component {
        id: chargingStationComponent

        Image {
            id: chargingStationImg

            x: model.column * (root.cellSize + root.cellSpacing)
            y: model.row    * (root.cellSize + root.cellSpacing)

            width:  root.cellSize
            height: root.cellSize

            source: model.image
        }
    }

    Component {
        id: podComponent

        Image {
            id: podImg

            x: model.column * (root.cellSize + root.cellSpacing)
            y: model.row    * (root.cellSize + root.cellSpacing)

            width:  root.cellSize
            height: root.cellSize

            source: model.image
        }
    }

    Component {
        id: deliveryStationComponent

        Image {
            id: deliveryStationImg

            x: model.column * (root.cellSize + root.cellSpacing)
            y: model.row    * (root.cellSize + root.cellSpacing)

            width:  root.cellSize
            height: root.cellSize

            source: model.image
        }
    }

    MouseArea {
        id: zoomArea
        z:  100

        property real zoomScale: 1
        property real zoomX
        property real zoomY

        anchors.fill: mapFlickable

        acceptedButtons: Qt.MiddleButton

        onWheel: {
            zoomScale = Math.max(zoomScale + wheel.angleDelta.y / (120*2), 1)
            zoomX     = wheel.x
            zoomY     = wheel.y
        }
    }

    Flickable {
        id: mapFlickable

        anchors.fill: parent

        clip:true

        contentWidth:  map.width
        contentHeight: map.height

        contentX: root.cellScale == 1 ? (contentWidth  - width)  / 2 : contentWidth/2  - width  + zoomArea.zoomX
        contentY: root.cellScale == 1 ? (contentHeight - height) / 2 : contentHeight/2 - height + zoomArea.zoomY

        boundsBehavior: Flickable.StopAtBounds

        Item {
            id: map

            anchors.centerIn: parent

            width:  Math.max(mapFlickable.width  * 1.5, root.mapWidth  * 1.1)
            height: Math.max(mapFlickable.height * 1.5, root.mapHeight * 1.1)

            GridLayout {
                id: base

                anchors.centerIn: parent

                rows:    root.rows
                columns: root.columns

                rowSpacing:    root.cellSpacing
                columnSpacing: root.cellSpacing

                Repeater {
                    id: baseRepeater

                    model:    root.rows * root.columns
                    delegate: baseComponent
                }
            }
            Item {
                id: actors

                anchors.fill: base

                Repeater {
                    id: actorRepeater

                    model: ActorListModel {
                       actors: SimPresenter.layout.actors
                    }
                    delegate: actorComponent
                }
            }
            Item {
                id: chargingStations

                anchors.fill: base

                Repeater {
                    id: chargingStationRepeater

                    model: ChargingStationListModel {
                        chargingStations: SimPresenter.layout.chargingStations
                    }
                    delegate: chargingStationComponent
                }
            }
            Item {
                id: pods

                anchors.fill: base

                Repeater {
                    id: podRepeater

                    model: PodDockListModel {
                        podDocks: SimPresenter.layout.podDocks
                    }
                    delegate: podComponent
                }
            }
            Item {
                id: deliveryStations

                anchors.fill: base

                Repeater {
                    id: deliveryStationRepeater

                    model: DeliveryStationListModel {
                        deliveryStations: SimPresenter.layout.deliveryStations
                    }
                    delegate: deliveryStationComponent
                }
            }
        }
    }
}
