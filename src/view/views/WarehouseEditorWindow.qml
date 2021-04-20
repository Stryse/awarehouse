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

    property string currentWarehouse: EditorPresenter.currentWarehouseName
    readonly property bool isDeafultWarehouse: currentWarehouse == "New" || currentWarehouse == "Default"

    readonly property ListModel tileList: ListModel {
        ListElement { tileType: TileType.ACTOR;            tileTypeName: "Actor";            tileColor:    "#80DEEA" }
        ListElement { tileType: TileType.POD;              tileTypeName: "Pod";              tileColor:    "#FFAB91" }
        ListElement { tileType: TileType.CHARGING_STATION; tileTypeName: "Charging Station"; tileColor:    "#CE93D8" }
        ListElement { tileType: TileType.DELIVERY_STATION; tileTypeName: "Delivery Station"; tileColor:    "#A5D6A7" }

        function getTileColor(type) {
            for (var i = 0; i < tileList.count; ++i) {
                var tile = tileList.get(i)

                if (tile.tileType === type)
                    return tile.tileColor
            }

            return Material.accent
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

            Label {
                id: currentWarehouseLabel

                anchors {
                    left: previewLabel.left
                    top:  previewLabel.bottom

                    topMargin: height * -0.35
                }
                leftPadding: previewLabel.leftPadding

                text: editorRoot.currentWarehouse

                font.pixelSize: editorRoot.height * 0.026
                font.weight: Font.Light
                font.italic: true
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

                    onClicked: saveWarehousePopup.open();
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
            id: loadPopupContent

            Label {
                id: loadPopupLabel

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top:              parent.top
                }

                text:           qsTr("Warehouses")
                font.pixelSize: loadPopupContent.height * 0.1
            }

            Rectangle {
                id: listViewBackground

                anchors.fill: warehouseList

                color: mainRoot.secondaryColor
                radius: 2
            }

            ListView {
                id: warehouseList

                anchors {
                    left: parent.left;       right:  parent.right
                    top:  loadPopupLabel.bottom; bottom: cancelLoadPopup.top

                    leftMargin:  loadWarehousePopup.horizontalPadding * 0.1
                    rightMargin: loadWarehousePopup.horizontalPadding * 0.1
                    topMargin:   loadWarehousePopup.verticalPadding
                }
                clip: true

                focus:        true
                currentIndex: -1

                model: EditorPresenter.persistence.warehouses

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
                    font.pixelSize: loadPopupLabel.height * 0.25
                    color:          warehouseRecordMouseArea.containsMouse ? Material.accent : Material.foreground

                    MouseArea {
                        id: warehouseRecordMouseArea

                        anchors.fill: parent

                        hoverEnabled: true

                        onClicked: warehouseList.currentIndex = index

                        onDoubleClicked: {
                            loadWarehousePopup.loadWarehouse(warehouseName)
                            loadWarehousePopup.close()
                        }
                    }
                }
            }

            Button {
                id: loadWarehouse

                anchors {
                    left:   warehouseList.left
                    bottom: parent.bottom
                }

                flat:                true
                Material.background: Material.primary

                text:                qsTr("Load")
                font.pixelSize:      loadPopupLabel.height * 0.25
                font.capitalization: Font.MixedCase;

                onClicked: {
                    if (warehouseList.currentIndex !== -1) {
                        loadWarehousePopup.loadWarehouse(warehouseList.currentItem.warehouseName)
                        warehouseList.currentIndex = -1
                        loadWarehousePopup.close()
                    }
                    else {
                        console.log("Select a warehouse!")
                    }
                }
            }

            Button {
                id: cancelLoadPopup

                anchors {
                    right:  warehouseList.right
                    bottom: parent.bottom
                }

                flat:                true
                Material.background: Material.primary

                text:                qsTr("Cancel")
                font.pixelSize:      loadPopupLabel.height * 0.25
                font.capitalization: Font.MixedCase;

                onClicked: loadWarehousePopup.close()
            }
        }

        onClosed: warehouseList.currentIndex = -1

        function loadWarehouse(warehouseName) {
            EditorPresenter.loadWarehouse(warehouseName);

            if (editorRoot.isDeafultWarehouse)
                savePopupStack.replace(saveAsComponent)
            else
                savePopupStack.replace(overwriteComponent)
        }
    }

    Popup {
        id: saveWarehousePopup

        property bool isSaveAs: true

        anchors.centerIn: Overlay.overlay
        height:           editorRoot.height * 0.33
        width:            editorRoot.width  * 0.4

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
            id: savePopupContent

            Label {
                id: savePopupLabel

                anchors {
                    left: parent.left
                    top:  parent.top
                }

                text:           qsTr("Save Warehouse")
                font.pixelSize: savePopupContent.height * 0.2
            }

            StackView {
                id: savePopupStack

                anchors {
                    left: parent.left;           right:  parent.right
                    top:  savePopupLabel.bottom; bottom: parent.bottom
                }

                initialItem: saveAsComponent
            }

            Component {
                id: overwriteComponent

                Item {
                    id: overwriteItem

                    Label {
                        id: overwriteLabel

                        text: "Do you wish to overwrite " + editorRoot.currentWarehouse + " warehouse?"

                        font.pixelSize: savePopupLabel.height * 0.34
                    }

                    Button {
                        id: yesButton

                        anchors {
                            left:   parent.left
                            bottom: parent.bottom

                            topMargin: overwriteLabel.height * 0.1
                        }

                        flat:                true
                        Material.background: Material.primary

                        text:                qsTr("Yes")
                        font.pixelSize:      savePopupLabel.height * 0.28
                        font.capitalization: Font.MixedCase;

                        onClicked: {
                            saveWarehousePopup.saveWarehouse(editorRoot.currentWarehouse)
                            saveWarehousePopup.close()
                        }
                    }

                    Button {
                        id: noButton

                        anchors {
                            left:   yesButton.right
                            bottom: yesButton.bottom

                            leftMargin: width * 0.08
                        }

                        flat:                true
                        Material.background: Material.primary

                        text:                qsTr("No")
                        font.pixelSize:      savePopupLabel.height * 0.28
                        font.capitalization: Font.MixedCase;

                        onClicked: {
                            saveWarehousePopup.isSaveAs = true
                            savePopupStack.replace(saveAsComponent)
                        }
                    }

                    Button {
                        id: cancelOverwritePopup

                        anchors {
                            right:  parent.right
                            bottom: parent.bottom
                        }

                        flat:                true
                        Material.background: Material.primary

                        text:                qsTr("Cancel")
                        font.pixelSize:      savePopupLabel.height * 0.28
                        font.capitalization: Font.MixedCase;

                        onClicked: saveWarehousePopup.close()
                    }
                }
            }

            Component {
                id: saveAsComponent

                Item {
                    id: saveAsItem

                    Label {
                        id: saveAsLabel

                        text: "Please enter a new warehouse name below"

                        font.pixelSize: savePopupLabel.height * 0.34
                    }

                    TextField {
                        id: warehouseNameField

                        anchors {
                            top: saveAsLabel.bottom

                            topMargin: (parent.height - (saveAsLabel.height + height + defaultWarehouseNameLabel.height + saveAsButton.height))/2
                        }
                        width: saveAsLabel.width * 0.5

                        focus: true

                        placeholderText: "Warehouse name"
                        hoverEnabled: true

                        font.pixelSize: savePopupLabel.height * 0.28

                        onTextChanged: {
                            defaultWarehouseNameLabel.visible = false
                            zeroWarehouseNameLabel.visible    = false
                        }
                    }

                    Label {
                        id: defaultWarehouseNameLabel

                        anchors {
                            left: warehouseNameField.left
                            top:  warehouseNameField.bottom
                        }

                        visible: false

                        color: "#EF9A9A"
                        text: "Warehouse name can't be \"New\" or \"Default\""

                        font.pixelSize: savePopupLabel.height * 0.2
                    }
                    Label {
                        id: zeroWarehouseNameLabel

                        anchors {
                            left: warehouseNameField.left
                            top:  warehouseNameField.bottom
                        }

                        visible: false

                        color: "#EF9A9A"
                        text: "Warehouse name can't be empty"

                        font.pixelSize: savePopupLabel.height * 0.2
                    }

                    Button {
                        id: saveAsButton

                        anchors {
                            left:   parent.left
                            bottom: parent.bottom

                            leftMargin: width * 0.08
                        }

                        flat:                true
                        Material.background: Material.primary

                        text:                qsTr("Save")
                        font.pixelSize:      savePopupLabel.height * 0.28
                        font.capitalization: Font.MixedCase;

                        onClicked: {
                            if (warehouseNameField.text      == "Default" || warehouseNameField.text == "New")
                                defaultWarehouseNameLabel.visible = true
                            else if (warehouseNameField.text == "")
                                zeroWarehouseNameLabel.visible    = true
                            else {
                                saveWarehousePopup.saveWarehouse(warehouseNameField.text)
                                editorRoot.currentWarehouse = warehouseNameField.text
                                saveWarehousePopup.close()
                            }
                        }
                    }

                    Button {
                        id: cancelSaveAsPopup

                        anchors {
                            right:  parent.right
                            bottom: parent.bottom
                        }

                        flat:                true
                        Material.background: Material.primary

                        text:                qsTr("Cancel")
                        font.pixelSize:      savePopupLabel.height * 0.28
                        font.capitalization: Font.MixedCase;

                        onClicked: {
                            saveWarehousePopup.close()
                        }
                    }
                }
            }
        }

        onClosed: {
            if (!editorRoot.isDeafultWarehouse && saveWarehousePopup.isSaveAs) {
                savePopupStack.replace(overwriteComponent)
                saveWarehousePopup.isSaveAs = false
            }
            else if (editorRoot.isDeafultWarehouse || saveWarehousePopup.isSaveAs)
                savePopupStack.replace(saveAsComponent)
        }

        function saveWarehouse(warehouseName) {
            EditorPresenter.saveWarehouse(warehouseName);

            console.log("Saved " + warehouseName + " warehouse!")
        }
    }
}
