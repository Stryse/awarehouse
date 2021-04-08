import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import SimulationMap 1.0

Item {
    id: root

    readonly property real  aspectRatio: 16/9

    property int rows:    10
    property int columns: 10

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
        id: tileComponent

//        Image {
//            id: tileImg

//            Layout.preferredHeight: root.cellSize
//            Layout.preferredWidth:  root.cellSize

//            source: imgSource //EDIT
//        }
        Rectangle {
            id: tilePlaceholder

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

            x: columnIdx * (root.cellSize + root.cellSpacing)
            y: rowIdx    * (root.cellSize + root.cellSpacing)

            width:  root.cellSize
            height: root.cellSize

            source: imgSource

            rotation: rotationAngle
        }
    }

    Component {
        id: podComponent

        Image {
            id: podImg

            x: columnIdx * (root.cellSize + root.cellSpacing)
            y: rowIdx    * (root.cellSize + root.cellSpacing)

            width:  root.cellSize
            height: root.cellSize

            source: imgSource

            rotation: rotationAngle
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
                id: tiles

                anchors.centerIn: parent

                rows:    root.rows
                columns: root.columns

                rowSpacing:    root.cellSpacing
                columnSpacing: root.cellSpacing

                Repeater {
                    id: tileRepeater

                    model:    root.rows * root.columns
                    delegate: tileComponent
                }
            }
            Item {
                id: actors

                anchors.fill: tiles

                Repeater {
                    id: actorRepeater

                    model:    placeholderModel
                    delegate: actorComponent
                }
            }
            Item {
                id: pods

                anchors.fill: tiles

                Repeater {
                    id: podRepeater

                    model:    placeholderModel
                    delegate: podComponent
                }
            }
        }
    }
}
