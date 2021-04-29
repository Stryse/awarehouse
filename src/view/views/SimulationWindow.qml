import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: simulationRoot

    signal editorOpened()

    readonly property string windowTitle: qsTr("aWarehouse Manager")

    property color secondaryColor
    property real  borderWidth


    SplitView {
        id: horizontalSplit

        anchors.fill: parent

        orientation:  Qt.Horizontal

        LeftPanel {
            maxWidth:               parent.width * 0.28
            SplitView.maximumWidth: maxWidth

            borderWidth: simulationRoot.borderWidth
        }

        Pane {
            id: simulationArea

            SplitView.fillHeight:    true

            padding: 0

            Material.background: secondaryColor

            Simulation { id: simulation }
        }

//        SplitView {
//            id: verticalSplit

//            SplitView.fillWidth: true

//            orientation:  Qt.Vertical



//            BottomPanel {
//                id: bottomPanel

//                maxHeight:   parent.height * 0.325
//                borderWidth: simulationRoot.borderWidth
//            }

//        }

    }
}
