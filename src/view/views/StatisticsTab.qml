import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ActorList 1.0

Item {
    id: root

    anchors.fill: parent
    anchors {
        leftMargin:   10
        topMargin:    10
        bottomMargin: 10
    }

    clip: true

    ColumnLayout {
        id: statiscticsColumnLayout

        anchors.fill: parent

        Item {
            id: summaryItem

            property int    sumEnergy: SimPresenter.layout.actors.sumEnergy
            property int    sumMoves:  SimPresenter.layout.actors.sumMoves
            property double avgEnergy: Math.round(sumEnergy / SimPresenter.layout.actors.count * 100) / 100
            property double avgMoves:  Math.round(sumMoves  / SimPresenter.layout.actors.count * 100) / 100

            property double labelHeight: root.height * 0.035

            Layout.fillWidth:       true
            Layout.preferredHeight: summaryLabel.height + summaryLabels.height + 20

            Label {
                id: summaryLabel

                text: "Summary"

                font.pixelSize: root.height * 0.05
            }

            ColumnLayout {
                id: summaryLabels

                anchors {
                    left: parent.left;         right:  parent.right
                    top:  summaryLabel.bottom;
                }
                anchors {
                    leftMargin: 10;  rightMargin:  20
                    topMargin:  10;  bottomMargin: 10
                }

                spacing: 0

                Label {
                    id: timeLabel

                    Layout.fillWidth: true
                    Layout.preferredHeight: summaryItem.labelHeight

                    text: "Total Time Spent: " + SimPresenter.time

                    font.pixelSize: root.height * 0.025
                }
                Label {
                    id: sumEnergyUsedLabel

                    Layout.fillWidth: true
                    Layout.preferredHeight: summaryItem.labelHeight

                    text: "Total Energy Used: " + summaryItem.sumEnergy

                    font.pixelSize: root.height * 0.025
                }
                Label {
                    id: sumMovesLabel

                    Layout.fillWidth: true
                    Layout.preferredHeight: summaryItem.labelHeight

                    text: "Total Move Count: " + summaryItem.sumMoves

                    font.pixelSize: root.height * 0.025
                }
                Label {
                    id: avgEnergyUsedLabel

                    Layout.fillWidth: true
                    Layout.preferredHeight: summaryItem.labelHeight

                    text: "Average Energy Used: " + summaryItem.avgEnergy

                    font.pixelSize: root.height * 0.025
                }
                Label {
                    id: avgMovesLabel

                    Layout.fillWidth: true
                    Layout.preferredHeight: summaryItem.labelHeight

                    text: "Average Move Count: " + summaryItem.avgMoves

                    font.pixelSize: root.height * 0.025
                }
            }
        }

        Item {
            id: perActorItem

            Layout.fillWidth:  true
            Layout.fillHeight: true

            Label {
                id: perActorLabel

                text: "Actors"

                font.pixelSize: root.height * 0.05
            }

            ScrollView {
                id: actorStatScrollView

                anchors {
                    left: parent.left;          right:  parent.right
                    top:  perActorLabel.bottom; bottom: parent.bottom
                }

                ScrollBar.horizontal.policy:    ScrollBar.AlwaysOff
                ScrollBar.vertical.interactive: false

                ListView {
                    id: actorStatListView

                    anchors.fill: parent
                    anchors {
                        leftMargin: 10;  rightMargin:  20
                        topMargin:  10;  bottomMargin: 10
                    }

                    clip: true

                    model: ActorListModel {
                        actors: SimPresenter.layout.actors
                    }

                    delegate: Label {
                        Layout.fillWidth: true

                        text: "[" + (model.index+1) + "] Energy Used: " + model.energyUsed + " | Move Count: " + model.moves
                        font.pixelSize: root.height * 0.025
                    }
                }
            }
        }

        Button {
            id: exportButton

            focusPolicy: Qt.NoFocus

            flat: true
            Material.background: Material.primary
            font.capitalization: Font.MixedCase;

            text: qsTr("Export Statistics")

            onClicked: SimPresenter.exportStatistics()
        }
    }
}
