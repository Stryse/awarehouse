import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import ActorList           1.0
import ChargingStationList 1.0
import PodDockList         1.0
import PodList             1.0
import DeliveryStationList 1.0

Item {
    id: root

    readonly property real  aspectRatio: 16/9

    readonly property int rows:    SimPresenter.layout.rows
    readonly property int columns: SimPresenter.layout.columns

    property int  cellSpacing: 1
    property real cellScale:   zoomArea.zoomScale
    property real cellSize:    Math.round(Math.min(height / rows    - cellSpacing,
                                                   width  / columns - cellSpacing) * cellScale)

    property int mapWidth:  columns * cellSize + (columns-1) * cellSpacing
    property int mapHeight: rows    * cellSize + (rows   -1) * cellSpacing

    anchors.horizontalCenter: parent.horizontalCenter
    y: parent.height * 0.14 - controlLayout.height/2

    height: parent.height * 0.7
    width:  height * aspectRatio

    Material.background: Material.primary
    Material.elevation:  6

    Component {
        id: baseComponent

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

            property double row:    model.row
            property double column: model.column

            x: column * (root.cellSize + root.cellSpacing)
            y: row    * (root.cellSize + root.cellSpacing)

            width:  root.cellSize
            height: root.cellSize

            source: model.image

            rotation: model.rotation

            Behavior on column {
                enabled: !SimPresenter.paused
                PropertyAnimation { properties: "column"; easing.type: Easing.InOutQuart }
            }
            Behavior on row {
                enabled: !SimPresenter.paused
                PropertyAnimation { properties: "row"; easing.type: Easing.InOutQuart }
            }
            Behavior on rotation {
                id: rotationBehavior
                enabled: !SimPresenter.paused
                PropertyAnimation {
                    properties: "rotation"
                    easing.type: Easing.InOutQuart
                    from: actorImg.rotation % 360 == 0 ? (actorImg.rotation == 0 && rotationBehavior.targetValue === 270 ? 360 : 0) : model.rotation
                    to:   from === 270 && rotationBehavior.targetValue === 0 ? 360 : rotationBehavior.targetValue}
            }

            ProgressBar {
                id: battery

                y: cellSize * 0.35
                anchors {
                    left: parent.left;

                    leftMargin: cellSize * 0.05
                }
                width:  parent.width  * 0.07
                height: parent.height * 0.4

                background: Rectangle {
                    implicitWidth:  6
                    implicitHeight: 200
                    color: Material.background
                    radius: 1
                }

                contentItem: Item {
                    implicitWidth:  4
                    implicitHeight: 200

                    Rectangle {
                        anchors.bottom: parent.bottom
                        width:  parent.width
                        height: battery.visualPosition * parent.height
                        radius: 1
                        color: "#9ad69c"
                    }
                }

                from:  0
                to:    100
                value: model.battery

                Behavior on value {
                    PropertyAnimation { properties: "value"; easing.type: Easing.InOutQuart }
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
        }
    }

    Component {
        id: podDockComponent

        Image {
            id: podDockImg

            x: model.column * (root.cellSize + root.cellSpacing)
            y: model.row    * (root.cellSize + root.cellSpacing)

            width:  root.cellSize
            height: root.cellSize

            source: model.image
        }
    }

    Component {
        id: podComponent

        Item {
            id: podItem

            property variant orders: model.orders
            property double  row:    model.row
            property double  column: model.column

            x: column * (root.cellSize + root.cellSpacing)
            y: row    * (root.cellSize + root.cellSpacing)

            width:  root.cellSize
            height: root.cellSize

            Behavior on column {
                enabled: !SimPresenter.paused
                PropertyAnimation { properties: "column"; easing.type: Easing.InOutQuart }
            }
            Behavior on row {
                enabled: !SimPresenter.paused
                PropertyAnimation { properties: "row"; easing.type: Easing.InOutQuart }
            }

            Image {
                id: podImg

                anchors.centerIn: parent
                width:  parent.width * 0.85
                height: width

                source: model.image

                rotation: model.rotation

                Behavior on rotation {
                    id: rotationBehavior
                    enabled: !SimPresenter.paused
                    PropertyAnimation {
                        properties: "rotation"
                        easing.type: Easing.InOutQuart
                        from: podImg.rotation % 360 == 0 ? (podImg.rotation == 0 && rotationBehavior.targetValue === 270 ? 360 : 0) : model.rotation
                        to: from === 270 && rotationBehavior.targetValue === 0 ? 360 : rotationBehavior.targetValue}
                }

                GridView {
                    id: ordersGrid

                    anchors.centerIn: parent

                    clip: true

                    width:  parent.width * 0.8
                    height: width

                    cellWidth:  width / 3
                    cellHeight: cellWidth

                    model: podItem.orders
                    delegate: Label {
                        id: orderLabel

                        text: modelData
                        font.bold: true

                        font.pixelSize: ordersGrid.cellWidth * 0.8
                    }
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

            Label {
                id: acceptedOrderID

                anchors.centerIn: parent

                text:           model.index + 1
                font.bold:      true
                font.pixelSize: root.cellSize * 0.4
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
                id: podDocks

                anchors.fill: base

                Repeater {
                    id: podDockRepeater

                    model: PodDockListModel {
                        podDocks: SimPresenter.layout.podDocks
                    }
                    delegate: podDockComponent
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
                id: pods

                anchors.fill: base

                Repeater {
                    id: podRepeater

                    model: PodListModel {
                        pods: SimPresenter.layout.pods
                    }
                    delegate: podComponent
                }
            }
        }
    }
}
