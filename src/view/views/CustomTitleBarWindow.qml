import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

ApplicationWindow {
    id: root

    property alias titleVisible: titleLabel.visible
    property alias titleText:    titleLabel.text

    property color titleBarColor: "#212121"
    property real  borderWidth:   width * 0.002

    property bool isDraggable:  true

    readonly property alias isFullScreen: fullScreenButton.isFullScreen

    flags: Qt.FramelessWindowHint  |
           Qt.Window

    //Titlebar
    menuBar: Rectangle {
        id: titleBar

        height: Math.max(titleLabel.height,
                         buttonLayout.buttonIconHeight) + 10

        color: titleBarColor

        //Title
        Label {
            id: titleLabel

            anchors.centerIn: parent

            visible:          true
            font.pixelSize:   14
        }

        //Button layout
        RowLayout {
            id: buttonLayout

            property real buttonPadding:    18
            property real buttonIconHeight: 21

            anchors {
                right: parent.right
                top:   parent.top;  bottom: parent.bottom
            }

            spacing: 0

            //Minimize
            ToolButton {
                id: minimizeButton

                Layout.preferredWidth:  icon.width + 2*parent.buttonPadding
                Layout.preferredHeight: parent.height

                flat:        true
                icon.height: parent.buttonIconHeight
                icon.source: "qrc:/minimize_white.png"

                onClicked:   showMinimized()
            }
            //Fullscreen
            ToolButton {
                id: fullScreenButton

                property bool isFullScreen: false

                Layout.preferredWidth:  icon.width + 2*parent.buttonPadding
                Layout.preferredHeight: parent.height

                flat:        true
                icon.height: parent.buttonIconHeight
                icon.source: isFullScreen ? "qrc:/fullscreen_exit_white.png" : "qrc:/fullscreen_white.png"

                onClicked: {
                    if (isFullScreen) { showNormal()     }
                    else              { showFullScreen() }

                    isFullScreen = !isFullScreen
                }
            }
            //Close
            ToolButton {
                id: closeButton

                Layout.preferredWidth:  icon.width + 2*parent.buttonPadding
                Layout.preferredHeight: parent.height

                flat:        true
                icon.height: parent.buttonIconHeight
                icon.source: "qrc:/close_white.png"

                Rectangle{
                    id: redShadeRect

                    anchors.fill: parent

                    color:   Material.color(Material.Red, Material.Shade900)
                    opacity: 0
                }

                onHoveredChanged: hovered ? redShadeRect.opacity = .5 : redShadeRect.opacity = 0;
                onClicked:        Qt.quit()
            }
        }

        //Drag area
        MouseArea {
            id: titleBarDragArea

            property real lastMouseX: 0
            property real lastMouseY: 0

            anchors {
                left: parent.left; right:  buttonLayout.left
                top:  parent.top;  bottom: parent.bottom
            }

            onPressed: { lastMouseX = mouseX
                         lastMouseY = mouseY }
            onMouseXChanged: if(isDraggable && !isFullScreen) { root.x += (mouseX - lastMouseX) }
            onMouseYChanged: if(isDraggable && !isFullScreen) { root.y += (mouseY - lastMouseY) }
        }
    }

    //Left Border
    Rectangle {
        z: 1
        anchors {
            left: parent.left
            top:  parent.top; bottom: parent.bottom
        }
        width: borderWidth

        color: titleBarColor
    }
    //Right Border
    Rectangle {
        z: 1
        anchors {
            right: parent.right
            top:   parent.top;  bottom: parent.bottom
        }
        width: borderWidth

        color: titleBarColor
    }
    //Bottom Border
    Rectangle {
        z: 1
        anchors {
            left:   parent.left;  right: parent.right
            bottom: parent.bottom
        }
        height: borderWidth

        color: titleBarColor
    }
}
