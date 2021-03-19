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

                property real simulationSpeed: 1

                states: [
                    State {
                        name: "normal"
                        PropertyChanges { target: speed; text: "1×"; simulationSpeed: 1 }
                    },
                    State {
                        name: "double"
                        PropertyChanges { target: speed; text: "2×"; simulationSpeed: 2 }
                    },
                    State {
                        name: "half"
                        PropertyChanges { target: speed; text: "0.5×"; simulationSpeed: 0.5 }
                    }
                ]
                state: "normal"

                Layout.fillWidth:  true
                Layout.fillHeight: true

                font.bold: true
                font.letterSpacing: 0
                font.pixelSize: Math.min(parent.height * 0.35, 30)

                onClicked: {
                    switch (speed.state) {
                    case "normal": speed.state = "double"; break
                    case "double": speed.state = "half";   break
                    case "half":   speed.state = "normal"; break
                    }
                }
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
