import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import SimulationMap 1.0

Item {
    id: root

    readonly property real  aspectRatio: 16/9

    property real rows:    12
    property real columns: 12

    property real cellSpacing: 1
    property real cellScale:   1
    property real cellSize:    Math.min(height / rows    - cellSpacing,
                                        width  / columns - cellSpacing) * cellScale

    property real mapWidth:  columns * cellSize + (columns-1) * cellSpacing
    property real mapHeight: rows    * cellSize + (rows   -1) * cellSpacing

    anchors.centerIn: parent
    height:           parent.height * 0.7
    width:            height * aspectRatio

    Material.background: Material.primary
    Material.elevation:  6

    Component {
        id: mapComponent

        Image {
            id: img

            anchors.fill: parent
        }

    }

    ListModel {
        id: placeholderModel

        ListElement { rowIdx: 1; columnIdx: 1; imgSource: "qrc:/placeholder_amogus.png"}
        ListElement { rowIdx: 3; columnIdx: 7; imgSource: "qrc:/placeholder_amogus.png"}
        ListElement { rowIdx: 9; columnIdx: 4; imgSource: "qrc:/placeholder_amogus.png"}
    }

    Rectangle {
        color: "red"

        anchors.fill: parent
    }

//    Flickable {
//        id: mapFlickable

//        anchors.fill: parent

//        contentHeight: root.rows    * root.cellSize
//        contentWidth:  root.columns * root.cellSize

//        boundsBehavior: Flickable.StopAtBounds

        Rectangle {
            color: "blue"

            anchors.fill: tiles
        }
        FixedGrid {
            id: tiles

            cellSize: root.cellSize

            rows:    root.rows
            columns: root.columns

            rowSpacing:    root.cellSpacing
            columnSpacing: root.cellSpacing

            anchors.centerIn: parent

            model:    placeholderModel
            delegate: mapComponent
        }
        GridLayout {
            id: actors

            rows:    root.rows
            columns: root.columns

            rowSpacing:    root.cellSpacing
            columnSpacing: root.cellSpacing

            anchors.centerIn: parent

        }
        GridLayout {
            id: pods

            rows:    root.rows
            columns: root.columns

            rowSpacing:    root.cellSpacing
            columnSpacing: root.cellSpacing

            anchors.centerIn: parent

        }
//    }
}
