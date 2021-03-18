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

            text:           qsTr("Actor Info")
            font.pixelSize: titleBar.height * 0.45
        }
    }

    Rectangle {
        id: border

        z: 1
        anchors {
            right: parent.right
            top:   titleBar.bottom; bottom: parent.bottom
        }

        color: Material.primary
    }
}
