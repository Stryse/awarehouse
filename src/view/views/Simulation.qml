import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Simulator 1.0

Item {
    id: root

    anchors.fill: parent

    SimulationMap { id: simulationMap }

    Item {
        id: simulationControl

        anchors {
            top: simulationMap.bottom; bottom: parent.bottom
            horizontalCenter: simulationMap.horizontalCenter
        }
        width:  simulationMap.width  * 0.4

        RowLayout {
            id: controlLayout

            anchors.centerIn: parent
            height:           parent.height * 0.65

            MediaControlButton {
                id: play

                property bool paused: true

                Layout.fillWidth:  true
                Layout.fillHeight: true

                icon.source: paused ? "qrc:/play_white.png" : "qrc:/pause_white.png"
                //BUG: when button is at minimal size and clicked -> changed text appears
                //text:        paused ? qsTr("Pause")                : qsTr("Play")

                onClicked: {
                    if(paused)
                        simpresenter.simulationStart()
                    else
                        simpresenter.simulationStop()

                    paused = !paused
                }
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
                    case "normal": speed.state = "double"; simpresenter.setTickRate(TickRate.TWICE);      break
                    case "double": speed.state = "half";   simpresenter.setTickRate(TickRate.HALF_SPEED); break
                    case "half":   speed.state = "normal"; simpresenter.setTickRate(TickRate.NORMAL);     break
                    }
                }
            }

            MediaControlButton {
                id: reset

                Layout.fillWidth:  true
                Layout.fillHeight: true

                icon.source: "qrc:/replay_white.png"
                //text: "Reset"

                onClicked: simpresenter.reloadWarehouse();
            }
        }
    }
}
