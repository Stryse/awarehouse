import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Material

CustomTitleBarWindow {
    id: mainRoot

    readonly property string windowTitle:     qsTr("aWarehouse Manager")
    readonly property color  secondaryColor:   "#2b2b2b"

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

            borderWidth: mainRoot.borderWidth
        }

        SplitView {
            id: verticalSplit

            SplitView.fillWidth: true

            orientation:  Qt.Vertical

            Pane {
                id: simulationArea

                SplitView.minimumHeight: 50
                SplitView.fillHeight:    true

                padding: 0

                Material.background: secondaryColor

                Simulation { id: simulation }
            }

            BottomPanel {
                id: bottomPanel

                maxHeight:   parent.height * 0.325
                borderWidth: mainRoot.borderWidth
            }

        }

    }
}
