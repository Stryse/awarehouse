import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root

    property real  maxWidth

    readonly property bool isFullSize: width >= maxWidth * 0.8

    z: 1
    implicitWidth: maxWidth

    //Tabs
    TabBar {
        id: tabBar

        readonly property real textSize: contentHeight * 0.32

        anchors {
            left: parent.left; right: parent.right
            top:  parent.top
        }
        contentHeight: parent.height * 0.06

        Material.background: Material.primary

        TabButton { text: root.isFullSize  ?  qsTr("Outliner")   : qsTr("Out") ; font.capitalization: Font.MixedCase; font.pixelSize: tabBar.textSize }
        TabButton { text: root.isFullSize  ?  qsTr("Statistics") : qsTr("Stat"); font.capitalization: Font.MixedCase; font.pixelSize: tabBar.textSize }
        TabButton { text: root.isFullSize  ?  qsTr("Orders")     : qsTr("Ord") ; font.capitalization: Font.MixedCase; font.pixelSize: tabBar.textSize }
        TabButton { text: root.isFullSize  ?  qsTr("Settings")   : qsTr("Set") ; font.capitalization: Font.MixedCase; font.pixelSize: tabBar.textSize }
    }

    Pane {
        id: pane

        readonly property real paddingWidth: 10

        anchors {
            left: parent.left;   right:  parent.right
            top:  tabBar.bottom; bottom: parent.bottom
        }

        Material.elevation:  6

        padding:       0
        topPadding:    paddingWidth
        leftPadding:   paddingWidth
        bottomPadding: paddingWidth

        //Tab layout
        StackLayout {
            id: tabLayout

            anchors.fill: parent

            currentIndex: tabBar.currentIndex

            //Tab content
            OutlinerTab   { id: outliner   }
            StatisticsTab { id: statistics }
            OrdersTab     { id: orders     }
            SettingsTab   { id: settings   }
        }
    }
}
