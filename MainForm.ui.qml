import QtQuick 2.4
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.0

Item {
    id: root
    width: 320
    height: 425

    Rectangle {
        id: base
        x: 0
        y: 0
        width: 320
        height: 425
        border.color: "#000000"
        border.width: 0
        color: "#f3f3f3"

        Image {
            id: debicon
            x: 85
            y: 22
            width: 150
            height: 150
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            source: "img/debicon.png"

            Text {
                id: fileinfo
                x: 55
                y: 183
                color: "#2d2d2d"
                text: ddpkg.fileSize + " MB"
                style: Text.Raised
                verticalAlignment: Text.AlignVCenter
                font.bold: false
                anchors.horizontalCenterOffset: 0
                styleColor: "#ffffff"
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 13
            }

            Text {
                id: filename
                x: 12
                y: 159
                color: "#2d2d2d"
                text: ddpkg.fileName
                anchors.horizontalCenterOffset: 0
                styleColor: "#ffffff"
                style: Text.Raised
                font.bold: true
                font.pixelSize: 16
                anchors.horizontalCenter: parent.horizontalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }

        Rectangle {
            id: install_rect
            x: 27
            y: 253
            width: 120
            height: 144
            radius: 10
            border.color: "#44000000"
            border.width: install_ma.containsMouse ? 1 : 0
            color: install_ma.pressed ? "#eeeeee" : "#ffffff"

            MouseArea {
                id: install_ma
                cursorShape: Qt.PointingHandCursor
                anchors.fill: parent
                hoverEnabled: true
            }

            Image {
                id: install
                x: 15
                y: 13
                width: 90
                height: 90
                source: "img/install.png"

                Text {
                    id: install_txt
                    x: 37
                    y: 94
                    text: qsTr("Install")
                    anchors.horizontalCenterOffset: 0
                    anchors.horizontalCenter: parent.horizontalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 16
                }
            }
        }

        Rectangle {
            id: uninstall_rect
            enabled: ddpkg.packageExists
            x: 173
            y: 253
            width: 120
            height: 144
            radius: 10
            border.color: "#44000000"
            border.width: uninstall_ma.containsMouse ? 1 : 0
            color: enabled ? uninstall_ma.pressed ? "#eeeeee" : "#ffffff" : "#dddddd"

            MouseArea {
                id: uninstall_ma
                width: 120
                cursorShape: Qt.PointingHandCursor
                anchors.fill: parent
                hoverEnabled: true
            }

            Image {
                id: uninstall
                x: 15
                y: 13
                width: 90
                height: 90
                source: "img/cancel.png"

                Text {
                    id: uninstall_txt
                    x: 37
                    y: 94
                    text: qsTr("Uninstall")
                    font.bold: false
                    anchors.horizontalCenterOffset: 0
                    horizontalAlignment: Text.AlignHCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 16
                    verticalAlignment: Text.AlignVCenter
                }
            }

            ColorOverlay {
                anchors.fill: uninstall
                source: uninstall
                visible: !parent.enabled
                color: "#bbbbbb"
            }
        }
    }

    Connections {
        target: install_ma
        onClicked: {
            popup.closePolicy = Popup.NoAutoClose
            popup.text = qsTr("Installing...")
            popup.open()
            ddpkg.install()
            popup.closePolicy = Popup.CloseOnEscape | Popup.CloseOnPressOutside
        }
    }

    Connections {
        target: uninstall_ma
        onClicked: {
            popup.closePolicy = Popup.NoAutoClose
            popup.text = qsTr("Uninstalling...")
            popup.open()
            ddpkg.uninstall()

        }
    }

    Connections{
        target: ddpkg

        onPackexistsChanged: {
            console.log("Changed")
            uninstall_rect.enabled = ddpkg.packexists
            if(ddpkg.packexists){
                install.source = "img/refresh.png"
                install_txt.text = qsTr("Reinstall");
            }else{
                install.source = "img/install.png"
                install_txt.text = qsTr("Install");
            }
        }
    }


    property var popup: Popup{
        id: popup
        width: parent.width
        height: parent.height/4
        y: parent.height / 2 - parent.height / 8
        modal: true
        property string text: ""
        property string color: ""

        background: Rectangle{
            color: "#f0f0f0"
        }

        Text{
            id: topic
            text: popup.text
            color: popup.color
        }
    }
}
