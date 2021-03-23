import QtQuick
import QtQuick.Controls
import Outliner 1.0
import QtQuick.Controls.Material

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
            clip:   true

            color:  Material.primary

            Label {
                id: title

                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }

                leftPadding:    10

                text:           qsTr("Actors")
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
                model: OutlinerModel {
                    actors: simpresenter.actors
                }
                delegate: ItemDelegate {
                    text: model.name + " " + model.action + " " + model.battery + " " + model.orientation
                }
            }
        }
    }
}
