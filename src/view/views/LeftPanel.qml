import QtQuick                   2.15
import QtQuick.Layouts           1.12
import QtQuick.Controls          2.15
import QtQuick.Controls.Material 2.15

Item {
    id: root

    property real  maxWidth
    property color tabBarColor

    readonly property bool isFullSize: width >= maxWidth * 0.8

    z: 1
    implicitWidth: maxWidth

    //Tabs
    TabBar {
        id: tabBar

        anchors {
            left: parent.left; right: parent.right
            top:  parent.top
        }
        contentHeight: parent.height * 0.06

        Material.background: tabBarColor

        TabButton { text: root.isFullSize  ?  qsTr("Outliner")   : qsTr("Out")  }
        TabButton { text: root.isFullSize  ?  qsTr("Statistics") : qsTr("Stat") }
        TabButton { text: root.isFullSize  ?  qsTr("Orders")     : qsTr("Ord")  }
        TabButton { text: root.isFullSize  ?  qsTr("Settings")   : qsTr("Set")  }
    }

    Pane {
        id: pane

        readonly property real paddingWidth: 10

        anchors {
            left: parent.left;   right:  parent.right
            top:  tabBar.bottom; bottom: parent.bottom
        }

        Material.elevation:  10

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
