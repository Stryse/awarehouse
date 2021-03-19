import QtQuick
import QtQuick.Controls

Item {
    id: root

    implicitHeight: pane.implicitHeight
    implicitWidth:  pane.implicitWidth

    Pane {
        id: pane

        anchors.fill:   parent

        implicitHeight: titleBar.implicitHeight + actorScrollView.implicitHeight

        padding:       0
        bottomPadding: 10

        Rectangle {
            id: titleBar

            anchors {
                left: parent.left; right: parent.right
            }
            height: parent.height * 0.12

            color:  Material.primary

            Label {
                id: title

                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }

                leftPadding:    10

                text:           qsTr("Tasks")
                font.pixelSize: titleBar.height * 0.4
            }

        }

        ScrollView {
            id: actorScrollView

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
}
