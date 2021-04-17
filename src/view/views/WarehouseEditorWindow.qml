import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Editor 1.0

Item {
    id: editorRoot

    signal editorClosed()

    readonly property string windowTitle: qsTr("aWarehouse Editor")

    property color secondaryColor
    property real  borderWidth

    property string currentWarehouse: "New"

    readonly property ListModel tileList: ListModel {
        ListElement {
            tileType:     TileType.ACTOR
            tileTypeName: "Actor"
            tileColor:    "#80DEEA"
        }
        ListElement {
            tileType:     TileType.POD_DOCK
            tileTypeName: "Pod"
            tileColor:    "#FFAB91"
        }
        ListElement {
            tileType:     TileType.CHARGING_STATION
            tileTypeName: "Charging Station"
            tileColor:    "#CE93D8"
        }
        ListElement {
            tileType:     TileType.DELIVERY_STATION
            tileTypeName: "Delivery Station"
            tileColor:    "#A5D6A7"
        }

        function getTileColor(type) {
            for (var i = 0; i < tileList.count; ++i) {
                var tile = tileList.get(i)

                if (tile.tileType === type)
                    return tile.tileColor
            }

            return Material.accent
        }
    }

    Popup {
        id: loadWarehousePopup

        anchors.centerIn: Overlay.overlay
        height:           editorRoot.height * 0.7
        width:            editorRoot.width  * 0.25

        Overlay.modal: Rectangle {
            color: Qt.rgba(0, 0, 0, 0.5)
        }

        background: Rectangle {
            color: Material.background
            radius: 2
        }

        verticalPadding:   10
        horizontalPadding: 20

        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        contentItem: Item {
            id: popupContent

            Label {
                id: popupLabel

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top:              parent.top
                }
                height: popupContent.height * 0.08

                text:           qsTr("Warehouses")
                font.pixelSize: popupContent.height * 0.09
            }

            Rectangle {
                id: listViewBackground

                anchors.fill: popupWarehouseList

                color: mainRoot.secondaryColor
                radius: 2
            }

            ListView {
                id: popupWarehouseList

                anchors {
                    left: parent.left;       right:  parent.right
                    top:  popupLabel.bottom; bottom: cancelPopup.top

                    leftMargin:  loadWarehousePopup.horizontalPadding * 0.1
                    rightMargin: loadWarehousePopup.horizontalPadding * 0.1
                    topMargin:   loadWarehousePopup.verticalPadding   * 3
                }
                clip: true

                focus:        true
                currentIndex: -1

                model: SimPresenter.warehouses

                delegate: Label {
                    id: warehouseRecord

                    property string warehouseName: modelData

                    width:  ListView.view.width

                    leftPadding:   10
                    rightPadding:  10
                    topPadding:    8
                    bottomPadding: 8

                    background: Rectangle {
                        color: warehouseRecord.ListView.isCurrentItem ? Material.primary : "transparent"
                        radius: 2
                    }

                    text:           warehouseName
                    font.pixelSize: popupLabel.height * 0.4
                    color:          warehouseRecordMouseArea.containsMouse ? Material.accent : Material.foreground

                    MouseArea {
                        id: warehouseRecordMouseArea

                        anchors.fill: parent

                        hoverEnabled: true

                        onClicked: popupWarehouseList.currentIndex = index

                        onDoubleClicked: loadWarehousePopup.loadWarehouse(warehouseName)
                    }
                }
            }

            Button {
                id: loadWarehouse

                anchors {
                    left:  popupWarehouseList.left
                    bottom: parent.bottom
                }

                flat:                true
                Material.background: Material.primary

                text:                qsTr("Load")
                font.pixelSize:      popupLabel.height * 0.4
                font.capitalization: Font.MixedCase;

                onClicked: {
                    //TODO: Initiate load
                    if (popupWarehouseList.currentIndex !== -1) {
                        loadWarehousePopup.loadWarehouse(popupWarehouseList.currentItem.warehouseName)
                    }
                    else {
                        console.log("Select a warehouse!")
                    }
                }
            }

            Button {
                id: cancelPopup

                anchors {
                    right:  popupWarehouseList.right
                    bottom: parent.bottom
                }

                flat:                true
                Material.background: Material.primary

                text:                qsTr("Cancel")
                font.pixelSize:      popupLabel.height * 0.4
                font.capitalization: Font.MixedCase;

                onClicked: loadWarehousePopup.close()
            }
        }

        function loadWarehouse(warehouseName) {
            editorRoot.currentWarehouse = warehouseName
            console.log(editorRoot.currentWarehouse)
            loadWarehousePopup.close()
        }
    }

    SplitView {
        id: horizontalSplit

        readonly property real leftPanelMaxWidth: width * 0.25

        anchors.fill: parent

        orientation:  Qt.Horizontal

        SplitView {
            id: verticalSplit

            readonly property real tabRatio: 2/7

            SplitView.preferredWidth: horizontalSplit.leftPanelMaxWidth
            SplitView.maximumWidth:   horizontalSplit.leftPanelMaxWidth

            orientation:  Qt.Vertical

            WarehouseSettingsTab {
                id: warehouseSettings

                SplitView.preferredHeight: parent.height * verticalSplit.tabRatio
                SplitView.minimumHeight:   parent.height * verticalSplit.tabRatio

                titleBarHeight: editorRoot.height * 0.05
                borderWidth:    editorRoot.width  * 0.0025
            }
            WarehouseTilesTab {
                id: warehouseTiles

                SplitView.fillHeight:    true
                SplitView.minimumHeight: parent.height * verticalSplit.tabRatio

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

                text:           qsTr("Preview")
                font.pixelSize: editorRoot.height * 0.04
            }

            RowLayout {
                id: buttonsLayout

                readonly property real buttonFontSize: height * 0.4

                anchors {
                    left:   parent.left;  right: parent.right
                    bottom: parent.bottom

                    leftMargin:   parent.width * 0.02; rightMargin: parent.width * 0.02
                    bottomMargin: parent.height * 0.02
                }
                height: parent.height * 0.06

                Button {
                    id: saveButton

                    Layout.fillHeight:   true

                    flat:                true
                    Material.background: Material.primary

                    text:                qsTr("Save")
                    font.pixelSize:      buttonsLayout.buttonFontSize
                    font.capitalization: Font.MixedCase;
                }
                Button {
                    id: loadButton

                    Layout.fillHeight:   true

                    flat:                true
                    Material.background: Material.primary

                    text:                qsTr("Load")
                    font.pixelSize:      buttonsLayout.buttonFontSize
                    font.capitalization: Font.MixedCase;

                    onClicked: loadWarehousePopup.open();
                }
                //Space filler
                Item {
                    id: space

                    Layout.fillWidth:  true
                    Layout.fillHeight: true
                }
                Button {
                    id: cancelButton

                    Layout.fillHeight:   true

                    flat:                true
                    Material.background: Material.primary

                    text:                qsTr("Cancel")
                    font.pixelSize:      buttonsLayout.buttonFontSize
                    font.capitalization: Font.MixedCase;

                    onClicked: editorRoot.editorClosed()
                }
            }

            WarehouseMap {
                id: warehouse
            }
        }
    }
}
