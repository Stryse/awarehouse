import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

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

                model: ListModel {
                    ListElement {
                        type: "Robot"
                    }
                    ListElement {
                        type: "Shelf"
                    }
                    ListElement {
                        type: "Charging Station"
                    }
                    ListElement {
                        type: "Delivery Station"
                    }
                }

                delegate: ColumnLayout {
                    id: tileDelegate

                    height: gridView.cellSize
                    width:  gridView.cellSize

                    spacing: 0

                    Rectangle {
                        id: tileImg

                        Layout.alignment:       Qt.AlignHCenter
                        Layout.preferredHeight: gridView.cellSize * (4/5)
                        Layout.preferredWidth:  Layout.preferredHeight

                        color: Material.accent
                    }
                    Text {
                        id: tileLabel

                        Layout.alignment: Qt.AlignHCenter

                        height: gridView.cellSize * (1/5)
                        width:  gridView.cellSize

                        color:    Material.foreground

                        text:     qsTr(model.type)
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
