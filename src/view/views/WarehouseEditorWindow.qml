import QtQuick
import QtQuick.Layouts
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

            Label {
                id: previewLabel

                anchors {
                    left: parent.left
                    top:  parent.top
                }

                leftPadding: editorRoot.width  * 0.01
                topPadding:  editorRoot.height * 0.005

                text: qsTr("Preview")
                font.pixelSize: editorRoot.height * 0.04
            }

            RowLayout {
                id: buttonsLayout

                anchors {
                    left:   parent.left;  right: parent.right
                    bottom: parent.bottom

                    leftMargin:   parent.width * 0.02; rightMargin: parent.width * 0.02
                    bottomMargin: parent.height * 0.02
                }
                height: parent.height * 0.06

                Button {
                    id: saveButton

                    Layout.fillHeight: true

                    flat: true
                    Material.background: Material.primary

                    text: qsTr("Save")
                    font.capitalization: Font.MixedCase;
                    font.pixelSize: buttonsLayout.height * 0.4
                }
                Button {
                    id: loadButton

                    Layout.fillHeight: true

                    flat: true
                    Material.background: Material.primary

                    text: qsTr("Load")
                    font.capitalization: Font.MixedCase;
                    font.pixelSize: buttonsLayout.height * 0.4
                }
                Item {
                    id: space

                    Layout.fillWidth:  true
                    Layout.fillHeight: true
                }
                Button {
                    id: cancelButton

                    Layout.fillHeight: true

                    flat: true
                    Material.background: Material.primary

                    text: qsTr("Cancel")
                    font.capitalization: Font.MixedCase;
                    font.pixelSize: buttonsLayout.height * 0.4
                }
            }

            Item {
                id: warehouse

                property int wareHouseRows: 10
                property int wareHouseCols: 20

                readonly property real  aspectRatio: 16/9

                anchors {
                    left: parent.left; right: parent.right
                    top: previewLabel.bottom; bottom: buttonsLayout.top

                    leftMargin:   parent.width * 0.05; rightMargin: parent.width * 0.05
                    bottomMargin: parent.height * 0.03
                }

                Material.background: Material.primary
                Material.elevation:  6

                Grid {
                    id: warehouseGrid

                    property real cellSize: Math.min(warehouse.width  / warehouse.wareHouseCols - spacing,
                                                     warehouse.height / warehouse.wareHouseRows - spacing)

                    anchors.centerIn: parent

                    rows:    warehouse.wareHouseRows
                    columns: warehouse.wareHouseCols
                    spacing: 1

                    Repeater {
                        model: warehouse.wareHouseRows * warehouse.wareHouseCols
                        Rectangle {
                            width:  warehouseGrid.cellSize
                            height: warehouseGrid.cellSize
                            color:  Material.accent
                        }
                    }
                }
            }
        }
    }
}
