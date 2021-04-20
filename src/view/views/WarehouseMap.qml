import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Editor 1.0
import ActorList           1.0
import ChargingStationList 1.0
import PodList             1.0
import DeliveryStationList 1.0

Item {
    id: root

    property int rows:    EditorPresenter.layout.rows
    property int columns: EditorPresenter.layout.columns

    property int   cellSpacing: 1
    property real  cellScale:   zoomArea.zoomScale
    property real  cellSize:    Math.round(Math.min(height / rows    - cellSpacing,
                                                    width  / columns - cellSpacing) * cellScale)

    property int mapWidth:  columns * cellSize + (columns-1) * cellSpacing
    property int mapHeight: rows    * cellSize + (rows   -1) * cellSpacing

    anchors {
        left: parent.left;                   right: parent.right
        top:  currentWarehouseLabel.bottom; bottom: buttonsLayout.top

        leftMargin:   parent.width  * 0.05; rightMargin: parent.width * 0.05
        bottomMargin: parent.height * 0.03
    }

    Material.background: Material.primary
    Material.elevation:  6

    Component {
        id: baseComponent

        DropArea {
            id: dropArea

            signal tileDropped(int tileType)

            readonly property int row: index / root.columns
            readonly property int col: index % root.columns

            onTileDropped: EditorPresenter.setTile(row, col, tileType)

            Layout.alignment: Qt.AlignCenter

            Layout.preferredWidth:  root.cellSize
            Layout.preferredHeight: root.cellSize

            Rectangle {
                id: baseRectangle

                anchors.fill: parent

                color: !dropArea.containsDrag ? Material.accent : Qt.darker(Material.accent, 1.5)
            }
        }
    }

    Component {
        id: actorComponent

        Image {
            id: actorImg

            x: model.column * (root.cellSize + root.cellSpacing)
            y: model.row    * (root.cellSize + root.cellSpacing)

            width:  root.cellSize
            height: root.cellSize

            source: model.image

            rotation: model.rotation

            MouseArea {
                id: actorMouseArea

                anchors.fill: parent

                acceptedButtons: Qt.MiddleButton | Qt.RightButton

                onClicked: {
                    if (mouse.button === Qt.RightButton) {
                        rotateActor()
                        console.log(model.rotation)
                    }
                    else if (mouse.button === Qt.MiddleButton)
                        EditorPresenter.removeTile(model.row, model.column)
                }

                function rotateActor() {
                    model.rotation = (model.rotation + 90) % 360
                }
            }
        }
    }

    Component {
        id: chargingStationComponent

        Image {
            id: chargingStationImg

            x: model.column * (root.cellSize + root.cellSpacing)
            y: model.row    * (root.cellSize + root.cellSpacing)

            width:  root.cellSize
            height: root.cellSize

            source: model.image

            MouseArea {
                id: chargingStationMouseArea

                anchors.fill: parent

                acceptedButtons: Qt.MiddleButton

                onClicked: EditorPresenter.removeTile(model.row, model.column)
            }
        }
    }

    Component {
        id: podComponent

        Image {
            id: podImg

            property variant orders: model.orders

            x: model.column * (root.cellSize + root.cellSpacing)
            y: model.row    * (root.cellSize + root.cellSpacing)

            width:  root.cellSize
            height: root.cellSize

            source: model.image

            GridView {
                id: ordersGrid

                anchors.centerIn: parent

                clip: true

                width:  parent.width * 0.8
                height: width

                cellWidth:  width  / 3
                cellHeight: cellWidth

                model: podImg.orders
                delegate: Label {
                    id: orderLabel

                    text: modelData
                    font.bold: true

                    font.pixelSize: ordersGrid.cellWidth * 0.8
                }
            }

            MouseArea {
                id: actorMouseArea

                anchors.fill: parent

                acceptedButtons: Qt.MiddleButton | Qt.RightButton

                onClicked: {
                    if (mouse.button === Qt.RightButton) {
                        ordersPopup.open()
                        console.log("Opened Pod")
                    }
                    else if (mouse.button === Qt.MiddleButton)
                        EditorPresenter.removeTile(model.row, model.column)
                }
            }

            Popup {
                id: ordersPopup

                anchors.centerIn: parent//Overlay.overlay
                height:           root.height * 0.6
                width:            root.width  * 0.27

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
                    id: ordersPopupContent

                    Label {
                        id: ordersPopupLabel

                        anchors {
                            horizontalCenter: parent.horizontalCenter
                            top:              parent.top
                        }

                        text:           qsTr("Orders")
                        font.pixelSize: ordersPopupContent.height * 0.1
                    }

                    ListModel {
                        id: ordersListModel
                    }

                    ListView {
                        id: ordersList

                        anchors {
                            left: parent.left;       right:  parent.right
                            top:  ordersPopupLabel.bottom; bottom: cancelOrdersPopup.top

                            leftMargin:  ordersPopup.horizontalPadding * 0.1
                            rightMargin: ordersPopup.horizontalPadding * 0.1
                            topMargin:   ordersPopup.verticalPadding
                        }
                        clip: true

                        focus:        true
                        currentIndex: -1

                        model: ordersListModel
                        delegate: Item {
                            id: orderRecord

                            property bool   isChecked: model.isChecked
                            property string labelText: model.orderName

                            width:  ListView.view.width
                            height: ordersList.height * 0.15

                            CheckBox {
                                id: isOrderSelectedCheckBox

                                anchors {
                                    left: parent.left
                                }

                                leftPadding:   10
                                topPadding:    8
                                bottomPadding: 8

                                checked:          orderRecord.isChecked
                                onCheckedChanged: model.isChecked = checked
                            }

                            Label {
                                id: orderListLabel

                                anchors {
                                    left: isOrderSelectedCheckBox.right; right: parent.right
                                    verticalCenter: isOrderSelectedCheckBox.verticalCenter
                                }

                                rightPadding:  10
                                topPadding:    8
                                bottomPadding: 8

                                background: Rectangle {
                                    color: orderRecord.ListView.isCurrentItem ? Material.primary : "transparent"
                                    radius: 2
                                }

                                text: labelText
                                font.pixelSize: ordersPopupLabel.height * 0.4
                                color:          orderRecordMouseArea.containsMouse ? Material.accent : Material.foreground
                            }

                            MouseArea {
                                id: orderRecordMouseArea

                                acceptedButtons: Qt.NoButton

                                anchors.fill: parent

                                hoverEnabled: true
                            }
                        }
                    }

                    Button {
                        id: cancelOrdersPopup

                        anchors {
                            right:  parent.right
                            bottom: parent.bottom
                        }

                        flat:                true
                        Material.background: Material.primary

                        text:                qsTr("Cancel")
                        font.pixelSize:      ordersPopupLabel.height * 0.35
                        font.capitalization: Font.MixedCase;

                        onClicked: ordersPopup.close()
                    }
                }

                onAboutToShow: {
                    //Omegalul top kek

                    var orderCategoryCount = EditorPresenter.layout.categoryCount
                    var currentOrders      = model.orders

                    var i = 1;
                    var j = 0;

                    while (i <= orderCategoryCount && j < currentOrders.length) {
                        if (i < parseInt(currentOrders[j], 10)) {
                            ordersListModel.append({"isChecked": false, "orderName": i})
                            ++i
                        }
                        else if (i > parseInt(currentOrders[j], 10)) {
                            ordersListModel.append({"isChecked": false, "orderName": currentOrders[j]})
                            ++j
                        }
                        else if (i === parseInt(currentOrders[j], 10)) {
                            ordersListModel.append({"isChecked": true, "orderName": i})
                            ++i
                            ++j
                        }
                    }
                    for (; i <= orderCategoryCount; ++i)
                        ordersListModel.append({"isChecked": false, "orderName": i})
                }

                onClosed: {
                    var orders = []
                    for (var i = 0; i < ordersListModel.count; ++i)
                        if (ordersListModel.get(i).isChecked)
                            orders.push(ordersListModel.get(i).orderName)

                    model.orders = orders

                    ordersListModel.clear()
                }
            }
        }
    }

    Component {
        id: deliveryStationComponent

        Image {
            id: deliveryStationImg

            x: model.column * (root.cellSize + root.cellSpacing)
            y: model.row    * (root.cellSize + root.cellSpacing)

            width:  root.cellSize
            height: root.cellSize

            source: model.image

            MouseArea {
                id: actorMouseArea

                anchors.fill: parent

                acceptedButtons: Qt.MiddleButton

                onClicked: EditorPresenter.removeTile(model.row, model.column)
            }
        }
    }

    MouseArea {
        id: zoomArea
        z:  100

        property real zoomScale: 1
        property real zoomX
        property real zoomY

        anchors.fill: mapFlickable

        acceptedButtons: Qt.MiddleButton
        propagateComposedEvents: true

        onWheel: {
            zoomScale = Math.max(zoomScale + wheel.angleDelta.y / (120*2), 1)
            zoomX     = wheel.x
            zoomY     = wheel.y
        }
    }

    Flickable {
        id: mapFlickable

        anchors.fill: parent

        clip: true

        contentWidth:  map.width
        contentHeight: map.height

        contentX: root.cellScale == 1 ? (contentWidth  - width)  / 2 : contentWidth/2  - width  + zoomArea.zoomX
        contentY: root.cellScale == 1 ? (contentHeight - height) / 2 : contentHeight/2 - height + zoomArea.zoomY

        boundsBehavior: Flickable.StopAtBounds

        Item {
            id: map

            anchors.centerIn: parent

            width:  Math.max(mapFlickable.width  * 1.5, root.mapWidth  * 1.1)
            height: Math.max(mapFlickable.height * 1.5, root.mapHeight * 1.1)

            GridLayout {
                id: base

                anchors.centerIn: parent

                rows:    root.rows
                columns: root.columns

                rowSpacing:    root.cellSpacing
                columnSpacing: root.cellSpacing

                Repeater {
                    id: baseRepeater

                    model:    root.rows * root.columns
                    delegate: baseComponent
                }
            }
            Item {
                id: actors

                anchors.fill: base

                Repeater {
                    id: actorRepeater

                    model: ActorListModel {
                       actors: EditorPresenter.layout.actors
                    }
                    delegate: actorComponent
                }
            }
            Item {
                id: chargingStations

                anchors.fill: base

                Repeater {
                    id: chargingStationRepeater

                    model: ChargingStationListModel {
                        chargingStations: EditorPresenter.layout.chargingStations
                    }
                    delegate: chargingStationComponent
                }
            }
            Item {
                id: pods

                anchors.fill: base

                Repeater {
                    id: podRepeater

                    model: PodListModel {
                        pods: EditorPresenter.layout.pods
                    }
                    delegate: podComponent
                }
            }
            Item {
                id: deliveryStations

                anchors.fill: base

                Repeater {
                    id: deliveryStationRepeater

                    model: DeliveryStationListModel {
                        deliveryStations: EditorPresenter.layout.deliveryStations
                    }
                    delegate: deliveryStationComponent
                }
            }
        }
    }
}
