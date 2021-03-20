import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root

    property alias titleBarHeight: titleBar.height
    property alias borderWidth:    border.width

    Pane {
        id: titleBar

        anchors {
            left: parent.left; right: parent.right
        }

        Material.background: Material.primary

        Label {
            id: title

            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }

            text:           qsTr("Log")
            font.pixelSize: titleBar.height * 0.6
        }
    }

    ScrollView {
        id: asd

        anchors {
            left: border.right;    right:  parent.right
            top:  titleBar.bottom; bottom: parent.bottom
        }

        padding: 5

        ScrollBar.horizontal.policy:    ScrollBar.AlwaysOff
        ScrollBar.vertical.interactive: false

        ListView {
            clip: true

            //PLACEHOLDER
            model: 100
            delegate: ItemDelegate {
                text: "Log " + index
            }
        }
    }

    Rectangle {
        id: border

        z: 1
        anchors {
            left: parent.left
            top:  titleBar.bottom; bottom: parent.bottom
        }

        color: Material.primary
    }
}
