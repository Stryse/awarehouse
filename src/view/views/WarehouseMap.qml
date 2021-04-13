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
    }

    Component {
        id: baseComponent

        Rectangle {
            id: baseRectangle

            Layout.alignment: Qt.AlignCenter

            Layout.preferredWidth:  root.cellSize
            Layout.preferredHeight: root.cellSize

            color: Material.accent
        }
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

            signal tileDropped(string tileType)

            readonly property int tileRowIdx:    rowIdx
            readonly property int tileColumnIdx: columnIdx

            property string tileType:     type
            property int    tileRotation: rotationAngle
            property color  tileColor:    tileType === "Empty" ? "transparent" : editorRoot.tileList.getTileColor(tileType)

            onTileDropped: {
                type = tileType
                dropArea.resetRotation()
            }

            Layout.alignment: Qt.AlignCenter

            Layout.preferredWidth:  root.cellSize
            Layout.preferredHeight: root.cellSize

            MouseArea {
                id: tileMouseArea

                anchors.fill: parent

                acceptedButtons: Qt.MiddleButton | Qt.RightButton

                onClicked: {
                    if (mouse.button === Qt.RightButton) {
                        if (type === "Robot") {
                            dropArea.rotateActor()
                            console.log(rotationAngle)
                        }
                        else if (type === "Pod") {
                            console.log("Item list opened")
                        }
                    }
                    else if (mouse.button === Qt.MiddleButton) {
                        if (type !== "Empty") {
                            dropArea.resetTile()
                        }
                    }
                }
            }

            Rectangle {
                id: tileRectangle

                anchors.fill: parent

                color: dropArea.tileColor

                states: [
                    State {
                        when: dropArea.containsDrag && dropArea.tileType !== "Empty"
                        PropertyChanges {
                            target: tileRectangle
                            color:  Qt.darker(dropArea.tileColor, 1.5)
                        }
                    },
                    State {
                        when: dropArea.containsDrag && dropArea.tileType === "Empty"
                        PropertyChanges {
                            target: tileRectangle
                            color:  Qt.darker(Material.accent, 1.5)
                        }
                    }
                ]
            }

            function rotateActor() {
                if (rotationAngle != null)
                    rotationAngle = (rotationAngle + 90) % 360
            }

            function resetRotation() {
                if (rotationAngle != null)
                    rotationAngle = 0
            }

            function resetTile() {
                if (type != null) {
                    type = "Road"
                    dropArea.resetRotation()
                }
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
                                           "type":          "Empty",
                                           "rotationAngle": 0 })
                    }
                }
            }
        }
    }
}
