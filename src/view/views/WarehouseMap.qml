import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Editor 1.0
import ActorList           1.0
import ChargingStationList 1.0
import PodDockList         1.0
import DeliveryStationList 1.0

Item {
    id: root

    property int rows:    EditorPresenter.layout.rows
    property int columns: EditorPresenter.layout.columns

    property int   cellSpacing: 1
    property real  cellScale:   zoomArea.zoomScale
    property real  cellSize:    Math.round(Math.min(height / rows    - cellSpacing,
                                                    width  / columns - cellSpacing) * cellScale)

    property int mapWidth:  columns * cellSize + (columns-1) * cellSpacing
    property int mapHeight: rows    * cellSize + (rows   -1) * cellSpacing

    anchors {
        left: parent.left;                   right: parent.right
        top:  currentWarehouseLabel.bottom; bottom: buttonsLayout.top

        leftMargin:   parent.width  * 0.05; rightMargin: parent.width * 0.05
        bottomMargin: parent.height * 0.03
    }

    Material.background: Material.primary
    Material.elevation:  6

    Component {
        id: baseComponent

        DropArea {
            id: dropArea

            signal tileDropped(int tileType)

            readonly property int row: index / root.columns
            readonly property int col: index % root.columns

            onTileDropped: EditorPresenter.setTile(row, col, tileType)

            Layout.alignment: Qt.AlignCenter

            Layout.preferredWidth:  root.cellSize
            Layout.preferredHeight: root.cellSize

            Rectangle {
                id: baseRectangle

                anchors.fill: parent

                color: !dropArea.containsDrag ? Material.accent : Qt.darker(Material.accent, 1.5)
            }
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

            MouseArea {
                id: actorMouseArea

                anchors.fill: parent

                acceptedButtons: Qt.MiddleButton | Qt.RightButton

                onClicked: {
                    if (mouse.button === Qt.RightButton) {
                        rotateActor()
                        console.log(model.rotation)
                    }
                    else if (mouse.button === Qt.MiddleButton)
                        EditorPresenter.removeTile(model.row, model.column)
                }

                function rotateActor() {
                    model.rotation = (model.rotation + 90) % 360
                }
            }
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

            MouseArea {
                id: chargingStationMouseArea

                anchors.fill: parent

                acceptedButtons: Qt.MiddleButton

                onClicked: EditorPresenter.removeTile(model.row, model.column)
            }
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

            MouseArea {
                id: actorMouseArea

                anchors.fill: parent

                acceptedButtons: Qt.MiddleButton | Qt.RightButton

                onClicked: {
                    if (mouse.button === Qt.RightButton) {
                        console.log("Opened Pod")
                    }
                    else if (mouse.button === Qt.MiddleButton)
                        EditorPresenter.removeTile(model.row, model.column)
                }
            }
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

            MouseArea {
                id: actorMouseArea

                anchors.fill: parent

                acceptedButtons: Qt.MiddleButton

                onClicked: EditorPresenter.removeTile(model.row, model.column)
            }
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
        propagateComposedEvents: true

        onWheel: {
            zoomScale = Math.max(zoomScale + wheel.angleDelta.y / (120*2), 1)
            zoomX     = wheel.x
            zoomY     = wheel.y
        }
    }

    Flickable {
        id: mapFlickable

        anchors.fill: parent

        clip: true

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
                       actors: EditorPresenter.layout.actors
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
                        chargingStations: EditorPresenter.layout.chargingStations
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
                        podDocks: EditorPresenter.layout.podDocks
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
                        deliveryStations: EditorPresenter.layout.deliveryStations
                    }
                    delegate: deliveryStationComponent
                }
            }
        }
    }
}
