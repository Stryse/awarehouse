import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root

    property alias titleBarHeight: titleBar.height
    property alias borderWidth: border.width

    implicitHeight: pane.implicitHeight
    implicitWidth:  pane.implicitWidth

    Pane {
        id: pane

        anchors {
            left: parent.left; right:  border.left
            top:  parent.top;  bottom: parent.bottom
        }

        implicitHeight: titleBar.implicitHeight + settingsScrollView.implicitHeight

        padding:       0
        bottomPadding: 10

        clip:   true

        Rectangle {
            id: titleBar

            anchors {
                left: parent.left; right: parent.right
            }

            color:  Material.primary

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

            ScrollBar.horizontal.policy:    ScrollBar.AlwaysOff
            ScrollBar.vertical.interactive: false

            ListView {
                clip: true

                //PLACEHOLDER
                model: 20
                delegate: ItemDelegate {
                    text: "Item " + index
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
