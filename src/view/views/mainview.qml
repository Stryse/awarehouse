import QtQuick                   2.15
import QtQuick.Controls          2.15
import QtQuick.Controls.Material 2.15

CustomTitleBarWindow {
    id: root

    readonly property string windowTitle:     qsTr("aWarehouse Manager")
    readonly property color  secondaryColor:  "#292929"
    readonly property color  tertiaryColor:   "#2b2b2b"

    visible: true

    title:        windowTitle
    titleText:    windowTitle
    titleVisible: true

    x: Screen.width/2  - width/2
    y: Screen.height/2 - height/2

    width:  1280
    height: 720

    minimumWidth:  1280
    minimumHeight: 720

    SplitView {
        id: horizontalSplit

        anchors.fill: parent

        orientation:  Qt.Horizontal

        LeftPanel {
            maxWidth:               parent.width * 0.28
            SplitView.maximumWidth: maxWidth

            tabBarColor: secondaryColor
        }

        Item {
            id: contentArea

            SplitView.minimumWidth: 50
            SplitView.fillWidth:    true

            SplitView {
                id: verticalSplit

                anchors.fill: parent

                orientation:  Qt.Vertical

                Pane {
                    id: simulationArea

                    SplitView.minimumHeight: 50
                    SplitView.fillHeight:    true

                    Material.background: tertiaryColor

                    Simulation { id: simulation }
                }
                BottomPanel {
                    id: bottomPanel

                    maxHeight: parent.height * 0.325
                }

            }
        }
    }
}
