import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Editor 1.0

Item {
    id: root

    property alias titleBarHeight: titleBar.height
    property alias borderWidth:    border.width

    Pane {
        id: pane

        anchors {
            left: parent.left; right:  border.left
            top:  parent.top;  bottom: parent.bottom
        }

        padding:       0
        bottomPadding: 10

        clip: true

        Rectangle {
            id: titleBar

            anchors {
                left: parent.left; right: parent.right
            }

            color: Material.primary

            Label {
                id: title

                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }
                leftPadding:    10

                text:           qsTr("Tiles")
                font.pixelSize: titleBar.height * 0.5
            }
        }

        ScrollView {
            id: settingsScrollView

            anchors {
                left: parent.left;     right:  parent.right
                top:  titleBar.bottom; bottom: parent.bottom
            }
            padding:    0
            topPadding: 5

            clip: true

            ScrollBar.horizontal.policy:    ScrollBar.AlwaysOff
            ScrollBar.vertical.interactive: false

            GridView {
                id: gridView

                readonly property real cellSize: horizontalSplit.leftPanelMaxWidth / 3.05

                anchors.fill: parent
                anchors {
                    topMargin: editorRoot.height * 0.025; bottomMargin: editorRoot.height * 0.025
                }

                cellWidth:  cellSize
                cellHeight: cellSize

                model: tileList

                delegate: ColumnLayout {
                    id: tileDelegate

                    height: gridView.cellSize
                    width:  gridView.cellSize

                    spacing: 0

                    MouseArea {
                        id: dragArea

                        Layout.alignment:       Qt.AlignHCenter
                        Layout.preferredHeight: gridView.cellSize * (4/5)
                        Layout.preferredWidth:  Layout.preferredHeight

                        drag.target: tileDragImg
                        onReleased:  {
                            var dragTarget = tileDragImg.Drag.target

                            if (dragTarget !== null) {
                                console.log("dragType: " + model.tileType)
                                dragTarget.tileDropped(model.tileType)
                            }
                            else
                                parent = tileDelegate
                        }

                        Rectangle {
                            id: tileImg

                            width:  dragArea.width
                            height: dragArea.height

                            color: model.tileColor
                        }

                        Rectangle {
                            id: tileDragImg

                            anchors.centerIn: dragArea

                            visible: false

                            width:  warehouse.cellSize
                            height: warehouse.cellSize

                            color: model.tileColor

                            Drag.active: dragArea.drag.active
                            Drag.hotSpot.x: width/2
                            Drag.hotSpot.y: width/2

                            states: State {
                                when: dragArea.drag.active
                                PropertyChanges { target: tileDragImg; visible: true }
                                ParentChange    { target: tileDragImg; parent: editorRoot }
                                AnchorChanges   { target: tileDragImg; anchors.verticalCenter: undefined; anchors.horizontalCenter: undefined }
                            }
                        }
                    }
                    Text {
                        id: tileLabel

                        Layout.alignment: Qt.AlignHCenter

                        height: gridView.cellSize * (1/5)
                        width:  gridView.cellSize

                        color:    Material.foreground

                        text:     qsTr(model.tileTypeName)
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }
    }

    Rectangle {
        id: border

        anchors {
            right: parent.right
            top:   parent.top; bottom: parent.bottom
        }

        color: Material.primary
    }
}
