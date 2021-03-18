import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root

    anchors.fill: parent

    Pane {
        id: simulation

        readonly property alias inProgress: play.paused
        readonly property real aspectRatio: 16/9

        anchors.centerIn: parent
        height: parent.height * 0.7
        width:  height * aspectRatio

        Material.background: Material.accent
        Material.elevation:  6
    }

    Item {
        id: simulationControl

        anchors {
            top: simulation.bottom; bottom: parent.bottom
            horizontalCenter: simulation.horizontalCenter
        }
        width:  simulation.width  * 0.4

        RowLayout {
            id: controlLayout

            anchors.centerIn: parent
            height:           parent.height * 0.65

            MediaControlButton {
                id: play

                property bool paused: false

                Layout.fillWidth:  true
                Layout.fillHeight: true

                icon.source: paused ? "qrc:/pause_white.png" : "qrc:/play_white.png"
                //BUG: when button is at minimal size and clicked -> changed text appears
                //text:        paused ? qsTr("Pause")                : qsTr("Play")

                onClicked:   paused = !paused
            }

            MediaControlButton {
                id: speed

                Layout.fillWidth:  true
                Layout.fillHeight: true

                icon.source: "qrc:/speed_white.png"
                //text: "Speed"
            }

            MediaControlButton {
                id: reset

                Layout.fillWidth:  true
                Layout.fillHeight: true

                icon.source: "qrc:/replay_white.png"
                //text: "Reset"
            }
        }
    }
}
