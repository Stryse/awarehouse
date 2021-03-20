import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Material

CustomTitleBarWindow {
    id: editorRoot

    readonly property string windowTitle:    qsTr("aWarehouse Editor")
    readonly property color  secondaryColor: "#2b2b2b"

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

        SplitView {
            id: verticalSplit

            SplitView.maximumWidth: parent.width * 0.28
            implicitWidth:          parent.width * 0.28

            orientation:  Qt.Vertical

            WarehouseSettingsTab {
                id: warehouseSettings

                implicitHeight: parent.height * (2/5)

                titleBarHeight: editorRoot.height * 0.05
                borderWidth:    editorRoot.width  * 0.0025
            }

            WarehouseTilesTab {
                id: warehouseTiles

                SplitView.fillHeight: true

                titleBarHeight: editorRoot.height * 0.05
                borderWidth:    editorRoot.width  * 0.0025
            }
        }

        Pane {
            id: editorArea

            SplitView.fillWidth: true

            padding: 0

            Material.background: secondaryColor
        }
    }
}
