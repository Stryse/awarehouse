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

    signal shiftPressed()
    signal shiftReleased()

    property int rows:    EditorPresenter.layout.rows
    property int columns: EditorPresenter.layout.columns

    property int  cellSpacing: 1
    property real cellScale:   zoomArea.zoomScale
    property real cellSize:    Math.round(Math.min(height / rows    - cellSpacing,
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
            signal tileMoved(int tileType, int index)

            readonly property int row:    index / root.columns
            readonly property int column: index % root.columns

            onTileDropped: EditorPresenter.setTile(row, column, tileType)
            onTileMoved: {
                var mainTile = podRepeater.itemAt(index)

                if (mainTile === null)
                    return

                if (tileType === TileType.POD && selectedPodList.length > 0) {
                    var canMove = true

                    var deltaRow    = row    - mainTile.row
                    var deltaColumn = column - mainTile.column

                    EditorPresenter.moveMultipleTile(TileType.POD, selectedPodList, deltaRow, deltaColumn);

                    clearSelectedPodList()
                }
                else
                    EditorPresenter.moveTile(tileType, index, row, column)
            }

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

            Behavior on rotation {
                id: rotationBehavior

                PropertyAnimation {
                    properties: "rotation"
                    easing.type: Easing.InOutQuart
                    from: actorImg.rotation % 360 == 0 ? (actorImg.rotation == 0 && rotationBehavior.targetValue === 270 ? 360 : 0) : model.rotation
                    to: from === 270 && rotationBehavior.targetValue === 0 ? 360 : rotationBehavior.targetValue}
            }

            Drag.active:    actorMouseArea.drag.active
            Drag.hotSpot.x: width/2
            Drag.hotSpot.y: width/2

            states:
                State {
                    when: !actorMouseArea.drag.active
                    PropertyChanges { target: actorImg; x: model.column * (root.cellSize + root.cellSpacing); y: model.row * (root.cellSize + root.cellSpacing) }
                }

            MouseArea {
                id: actorMouseArea

                anchors.fill: parent

                acceptedButtons: Qt.MiddleButton | Qt.RightButton | Qt.LeftButton

                drag.target: actorImg
                onReleased: {
                    var dragTarget = actorImg.Drag.target

                    if (dragTarget !== null)
                        dragTarget.tileMoved(TileType.ACTOR, model.index)
                }

                onClicked: {
                    if (mouse.button === Qt.RightButton) {
                        rotateActor()
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

            Drag.active:    chargingStationMouseArea.drag.active
            Drag.hotSpot.x: width/2
            Drag.hotSpot.y: width/2

            states:
                State {
                    when: !chargingStationMouseArea.drag.active
                    PropertyChanges { target: chargingStationImg; x: model.column * (root.cellSize + root.cellSpacing); y: model.row * (root.cellSize + root.cellSpacing) }
                }

            MouseArea {
                id: chargingStationMouseArea

                anchors.fill: parent

                acceptedButtons: Qt.MiddleButton | Qt.LeftButton

                drag.target: chargingStationImg
                onReleased: {
                    var dragTarget = chargingStationImg.Drag.target

                    if (dragTarget !== null)
                        dragTarget.tileMoved(TileType.CHARGING_STATION, model.index)
                }

                onClicked: {
                    if (mouse.button === Qt.MiddleButton)
                        EditorPresenter.removeTile(model.row, model.column)
                }
            }
        }
    }

    property bool canSelectPod
    property var  selectedPodList: []

    function clearSelectedPodList() {
        for (var i = 0; i < selectedPodList.length; ++i) {
            var pod = podRepeater.itemAt(selectedPodList[i])
            if (pod !== null) {
                pod.resetDrag()
                pod.selectVisible = false
                pod.originX = pod.x
                pod.originY = pod.y
            }
        }

        selectedPodList.length = 0
    }
    function updateSelectedPodX(deltaX, mainIndex) {
        for (var i = 0; i < selectedPodList.length; ++i) {
            if (selectedPodList[i] === mainIndex)
                continue;

            var pod = podRepeater.itemAt(selectedPodList[i])
            if (pod !== null)
                pod.dragX = deltaX;
        }
    }
    function updateSelectedPodY(deltaY, mainIndex) {
        for (var i = 0; i < selectedPodList.length; ++i) {
            if (selectedPodList[i] === mainIndex)
                continue;

            var pod = podRepeater.itemAt(selectedPodList[i])
            if (pod !== null)
                pod.dragY = deltaY;
        }
    }
    function resetSelectedPodPos() {
        for (var i = 0; i < selectedPodList.length; ++i) {
            var pod = podRepeater.itemAt(selectedPodList[i])
            if (pod !== null) {
                pod.resetDrag()
                pod.originX = pod.x
                pod.originY = pod.y
            }
        }
    }

    Component {
        id: podComponent

        Item {
            id: podItem

            property variant orders: model.orders

            property int row:    model.row
            property int column: model.column

            property bool selectVisible: false

            //Bug: Breakes if cursor is out of window
            property double dragX: 0
            property double dragY: 0

            function resetDrag() {
                dragX = 0
                dragY = 0
            }

            x: model.column * (root.cellSize + root.cellSpacing) + dragX
            y: model.row    * (root.cellSize + root.cellSpacing) + dragY

            width:  root.cellSize
            height: root.cellSize

            Rectangle { anchors.fill:parent; color: podItem.selectVisible ? "#EF9A9A" : "transparent" }

            Image {
                id: podDockImg

                anchors.fill: parent

                source: "qrc:/podDockImg.png"
            }

            Image {
                id: podImg

                anchors.centerIn: parent
                width:  parent.width * 0.85
                height: width

                source: model.image

                GridView {
                    id: ordersGrid

                    anchors.centerIn: parent

                    clip: true

                    width:  parent.width * 0.8
                    height: width

                    cellWidth:  width  / 3
                    cellHeight: cellWidth

                    model: podItem.orders
                    delegate: Label {
                        id: orderLabel

                        text: modelData
                        font.bold: true

                        font.pixelSize: ordersGrid.cellWidth * 0.8
                    }
                }
            }

            Drag.active:    podMouseArea.drag.active
            Drag.hotSpot.x: width/2
            Drag.hotSpot.y: width/2

            states:
                State {
                    when: !podItem.Drag.active
                    PropertyChanges {
                        target: podItem;
                        x: model.column * (root.cellSize + root.cellSpacing) + dragX;
                        y: model.row    * (root.cellSize + root.cellSpacing) + dragY;
                    }
                }

            property int originX: x
            property int originY: y

            onXChanged: {
                if (podItem.Drag.active) {
                    if (selectedPodList.indexOf(model.index) < 0)
                        root.clearSelectedPodList()

                    var deltaX = x - originX;
                    root.updateSelectedPodX(deltaX, model.index)
                }
            }

            onYChanged: {
                if (podItem.Drag.active) {
                    if (selectedPodList.indexOf(model.index) < 0)
                        root.clearSelectedPodList()

                    var deltaY = y - originY;
                    root.updateSelectedPodY(deltaY, model.index)
                }
            }

            MouseArea {
                id: podMouseArea

                anchors.fill: parent

                acceptedButtons: Qt.MiddleButton | Qt.RightButton | Qt.LeftButton

                drag.target: podItem
                onReleased: {
                    var dragTarget = podItem.Drag.target

                    if (dragTarget !== null)
                        dragTarget.tileMoved(TileType.POD, model.index)

                    root.resetSelectedPodPos()
                }
                onPressed: {
                    var listIndex = root.selectedPodList.indexOf(model.index)

                    if (mouse.button === Qt.LeftButton) {
                        if (mouse.modifiers & Qt.ShiftModifier) {
                            // Add to selected list
                            if (listIndex > -1) {
                                root.selectedPodList.splice(listIndex, 1)
                                podItem.selectVisible = false
                            }
                            // Remove from selected list
                            else {
                                root.selectedPodList.push(model.index)
                                root.selectedPodList.sort()
                                podItem.selectVisible = true
                            }
                        }
                    }
                }

                onClicked: {
                    var listIndex = root.selectedPodList.indexOf(model.index)

                    // Order Popup
                    if (mouse.button === Qt.RightButton)
                        ordersPopup.open()
                    // Remove Pod
                    else if (mouse.button === Qt.MiddleButton) {
                        if (listIndex > -1) {
                            for (var i = listIndex+1; i < root.selectedPodList.length; ++i)
                                --root.selectedPodList[i]
                            root.selectedPodList.splice(listIndex, 1)
                            console.log("Removed " + model.index + " from selection")
                        }

                        EditorPresenter.removeTile(model.row, model.column)
                    }
                }
            }

            Popup {
                id: ordersPopup

                anchors.centerIn: Overlay.overlay
                height:           root.height * 0.6
                width:            root.width  * 0.2

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
                            left: parent.left;             right:  parent.right
                            top:  ordersPopupLabel.bottom; bottom: cancelOrdersPopup.top

                            leftMargin:  ordersPopup.horizontalPadding * 0.1
                            rightMargin: ordersPopup.horizontalPadding * 0.1
                            topMargin:   ordersPopup.verticalPadding
                        }
                        clip: true

                        currentIndex: -1

                        model: ordersListModel
                        delegate: Item {
                            id: orderRecord

                            property bool   isChecked: model.isChecked
                            property string labelText: model.orderName

                            width:  ListView.view.width
                            height: ordersList.height * 0.15

                            Item {
                                id: orderItem

                                anchors {
                                    top: parent.top; bottom: parent.bottom
                                    horizontalCenter: parent.horizontalCenter
                                }
                                width: isOrderSelectedCheckBox.implicitWidth + orderListLabel.implicitWidth

                                CheckBox {
                                    id: isOrderSelectedCheckBox

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

                        focusPolicy: Qt.NoFocus

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

            Label {
                id: acceptedOrderID

                anchors.centerIn: parent

                text:           model.index + 1
                font.bold:      true
                font.pixelSize: root.cellSize * 0.4
            }

            Drag.active:    deliveryStationMouseArea.drag.active
            Drag.hotSpot.x: width/2
            Drag.hotSpot.y: width/2

            states:
                State {
                    when: !deliveryStationMouseArea.drag.active
                    PropertyChanges { target: deliveryStationImg; x: model.column * (root.cellSize + root.cellSpacing); y: model.row * (root.cellSize + root.cellSpacing) }
                }

            MouseArea {
                id: deliveryStationMouseArea

                anchors.fill: parent

                acceptedButtons: Qt.MiddleButton | Qt.LeftButton

                drag.target: deliveryStationImg
                onReleased: {
                    var dragTarget = deliveryStationImg.Drag.target

                    if (dragTarget !== null)
                        dragTarget.tileMoved(TileType.DELIVERY_STATION, model.index)
                }

                onClicked: {
                    if (mouse.button === Qt.MiddleButton)
                        EditorPresenter.removeTile(model.row, model.column)
                }
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

        clip:  true

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
        }
    }
}
