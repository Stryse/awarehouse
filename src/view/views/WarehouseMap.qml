import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root

    property int rows:    warehouseSettings.rowCount
    property int columns: warehouseSettings.columnCount

    property int   cellSpacing: 1
    property real  cellScale:   zoomArea.zoomScale
    property real  cellSize:    Math.round(Math.min(height / rows    - cellSpacing,
                                                    width  / columns - cellSpacing) * cellScale)

    property int mapWidth:  columns * cellSize + (columns-1) * cellSpacing
    property int mapHeight: rows    * cellSize + (rows   -1) * cellSpacing

//    onRowsChanged: {
//        if (tileModel.count === 0)
//            return

//        if (tileModel.count < rows * columns)
//            tiles.addRow()
//        else
//            tiles.deleteRow()
//    }

//    onColumnsChanged: {
//        if (tileModel.count === 0)
//            return

//        if (tileComponent.count < rows * columns)
//            tiles.addColumn()
//        else
//            tiles.deleteColumn()
//    }

    anchors {
        left: parent.left;         right:  parent.right
        top:  previewLabel.bottom; bottom: buttonsLayout.top

        leftMargin:   parent.width  * 0.05; rightMargin: parent.width * 0.05
        bottomMargin: parent.height * 0.03
    }

    Material.background: Material.primary
    Material.elevation:  6

    ListModel {
        id: tileModel

//        Repeater {
//            id: tileModelRepeater

//            ListElement {
//                rowIdx:        1
//                columnIdx:     1
//                type:          "Road"
//                rotationAngle: 0
//            }
//        }
    }

    Component {
        id: tileComponent

//        Image {
//            id: tileImg

//            Layout.preferredHeight: root.cellSize
//            Layout.preferredWidth:  root.cellSize

//            source: imgSource //EDIT
//        }
        DropArea {
            id: dropArea

            readonly property int tileRowIdx:    rowIdx
            readonly property int tileColumnIdx: columnIdx

            property string tileType:     type
            property int    tileRotation: rotationAngle
            property color  tileColor

            onTileTypeChanged: {
                dropArea.tileColor = editorRoot.tileList.getTileColor(tileType)

                if (rotationAngle != null)
                    rotationAngle = 0
            }

            Binding {
                target: model; property: "type"; value: dropArea.tileType
            }

            Layout.alignment: Qt.AlignCenter

            Layout.preferredWidth:  root.cellSize
            Layout.preferredHeight: root.cellSize

            MouseArea {
                id: tileMouseArea

                anchors.fill: parent

                acceptedButtons: Qt.LeftButton

                onDoubleClicked: {
                    if (type === "Robot") {
                        rotationAngle = (rotationAngle + 90) % 360
                        console.log(rotationAngle)
                    }
                    else if (type === "Pod") {
                        console.log("Item list opened")
                    }
                }
            }

            Rectangle {
                id: tileRectangle

                anchors.fill: parent

                color: dropArea.tileColor

                states: State {
                    when: dropArea.containsDrag
                    PropertyChanges {
                        target: tileRectangle
                        color:  Qt.darker(dropArea.tileColor, 1.5)
                    }
                }

//                function getTileColor(type) {
//                    for (var i = 0; i < tileList.count; ++i) {
//                        var tile = tileList.get(i)

//                        if (tile.tileType === type)
//                            return tile.tileColor
//                    }

//                    return Material.accent
//                }
            }
        }
    }

//    Component {
//        id: actorComponent

//        Image {
//            id: actorImg

//            x: columnIdx * (root.cellSize + root.cellSpacing)
//            y: rowIdx    * (root.cellSize + root.cellSpacing)

//            width:  root.cellSize
//            height: root.cellSize

//            source: imgSource

//            rotation: rotationAngle
//        }
//    }

//    Component {
//        id: podComponent

//        Image {
//            id: podImg

//            x: columnIdx * (root.cellSize + root.cellSpacing)
//            y: rowIdx    * (root.cellSize + root.cellSpacing)

//            width:  root.cellSize
//            height: root.cellSize

//            source: imgSource

//            rotation: rotationAngle
//        }
//    }

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
                id: tiles

                anchors.centerIn: parent

                rows:    root.rows
                columns: root.columns

                rowSpacing:    root.cellSpacing
                columnSpacing: root.cellSpacing

                Repeater {
                    id: tileRepeater

                    model:    tileModel
                    delegate: tileComponent
                }

                Component.onCompleted: {
                    for (var i = 0; i < root.rows; ++i)
                        addRow()
                }

                function addRow() {
                    for (var i = 0; i < root.columns; ++i) {
                        tileModel.append({ "rowIdx":        i / root.columns,
                                           "columnIdx":     i % root.columns,
                                           "type":          "Road",
                                           "rotationAngle": 0 })
                    }
                }

//                function addColumn() {
//                    for (var i = root.rows; i > 0; --i)
//                        tileModel.insert((root.columns-1)*i-1, { "rowIdx":        i / root.columns,
//                                                                 "columnIdx":     i % root.columns,
//                                                                 "type":          "Road",
//                                                                 "rotationAngle": 0 })
//                }

//                function deleteRow() {
//                    tileModel.remove(tileModel.count - root.columns, root.columns)
//                }

//                function deleteColumn() {
//                    for (var i = root.rows; i > 0; --i)
//                        tileModel.remove((root.columns+1)*i-1)
//                }
            }
        }
    }
}
