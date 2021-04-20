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
            height:           simulationMap.height * 0.12

            MediaControlButton {
                id: play

                property bool paused: SimPresenter.paused

                Layout.fillWidth:  true
                Layout.fillHeight: true

                icon.source: paused ? "qrc:/play_white.png" : "qrc:/pause_white.png"

                onClicked: {
                    if (paused)
                        SimPresenter.simulationStart()
                    else
                        SimPresenter.simulationStop()
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
                    case "normal": speed.state = "double"; SimPresenter.setTickRate(TickRate.TWICE);      break
                    case "double": speed.state = "half";   SimPresenter.setTickRate(TickRate.HALF_SPEED); break
                    case "half":   speed.state = "normal"; SimPresenter.setTickRate(TickRate.NORMAL);     break
                    }
                }
            }

            MediaControlButton {
                id: reset

                Layout.fillWidth:  true
                Layout.fillHeight: true

                icon.source: "qrc:/replay_white.png"

                onClicked: SimPresenter.reloadWarehouse();
            }
        }
    }
}
