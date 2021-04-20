import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import ActorList 1.0

Item {
    id: root

    Pane {
        id: pane

        anchors.fill:   parent
        padding:        0
        bottomPadding:  10

        Rectangle {
            id: titleBar

            anchors {
                top:  parent.top
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

                leftMargin: 5
                topMargin:  5
            }

            ScrollBar.horizontal.policy:    ScrollBar.AlwaysOff
            ScrollBar.vertical.interactive: false

            ListView {
                id: actorListView

                anchors.fill: parent
                anchors {
                    leftMargin: 5;  rightMargin:  20
                    topMargin:  10; bottomMargin: 10
                }

                clip: true

                model: ActorListModel {
                    actors: SimPresenter.layout.actors
                }

                delegate: GridLayout {
                    id: actorDelegate

                    width:  actorListView.width
                    height: root.height * 0.2

                    columns:       3
                    columnSpacing: 0

                    Item {
                        id: actorImg

                        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                        Layout.row:       0
                        Layout.column:    0
                        Layout.rowSpan:   3

                        Layout.preferredWidth: height
                        Layout.fillHeight:     true

                        Image {
                            id: img

                            anchors.centerIn: parent

                            height: parent.height * (3/4)
                            width:  height

                            source: model.image
                        }
                    }

                    Label {
                        id: nameLabel

                        Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                        Layout.row:       0
                        Layout.column:    1

                        text:           model.name
                        font.pixelSize: actorDelegate.height * 0.25
                    }

                    ProgressBar {
                        id: batteryProgressBar

                        Layout.row:        1
                        Layout.column:     1
                        Layout.columnSpan: 2

                        Layout.preferredWidth: actorDelegate.width - actorImg.width

                        from:  0
                        to:    100
                        value: model.battery
                    }

                    Label {
                        id: taskAndRotationLabel

                        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                        Layout.row:       2
                        Layout.column:    1

                        text: model.action + " | " + getOrientation(model.rotation)

                        function getOrientation(rotation) {
                            switch (rotation) {
                                case 0:
                                    return "Up"
                                case 90:
                                    return "Right"
                                case 180:
                                    return "Down"
                                case 270:
                                    return "Left"
                            }
                        }
                    }

                    Label {
                        id: batteryLabel

                        Layout.alignment: Qt.AlignRight | Qt.AlignTop
                        Layout.row:       2
                        Layout.column:    2

                        text: model.battery
                    }
                }
            }
        }
    }
}
