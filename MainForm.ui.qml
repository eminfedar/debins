import QtQuick 2.4
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.0

Item {
    id: root
    anchors.fill: parent
    property alias installed: installed

    property string currentVersion: ""
    property bool newVersion: false
    property bool oldVersion: false
    property var install: install
    property var install_txt: install_txt
    property var terminal_ma : terminal_ma
    property var terminal_fl: terminal_fl
    property var terminal_btn_co: terminal_btn_co

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
            y: 25
            width: 150
            height: 150
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            source: "img/debicon.png"

            Text {
                id: fileinfo
                x: 55
                y: 184
                color: "#2d2d2d"
                text: ddpkg.fileSize + " | " + ddpkg.packageVersion
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
                y: 161
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

                Image {
                    id: installed
                    y: 2
                    x: parent.contentWidth + 4
                    visible: ddpkg.packexists
                    source: "img/installed.png"

                    MouseArea{
                        id: installed_ma
                        anchors.fill: parent
                        hoverEnabled: true

                        ToolTip{
                            id: installed_tt
                            text: ddpkg.packageName + " " + ddpkg.packageCurrentVersion + qsTr(" is installed.")
                            background: Rectangle{
                                color: "#f5f5f5"
                                border.width: 1
                                border.color: "#999"
                            }
                            visible: installed_ma.containsMouse
                        }

                        Connections{
                            target: ddpkg
                            onPackageCurrentVersionChanged: {
                                installed_tt.text = ddpkg.packageName + " " + ddpkg.packageCurrentVersion + qsTr(" is installed.")
                            }
                        }
                    }
                }
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
                y: 8
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

                Text {
                    id: install_ver
                    x: 46
                    y: 114
                    width: parent.width+8
                    color: "#2d2d2d"
                    text: currentVersion
                    wrapMode: Text.Wrap
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    anchors.horizontalCenterOffset: 0
                    verticalAlignment: Text.AlignVCenter
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
                y: 8
                width: 90
                height: 90
                source: "img/uninstall.png"

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
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
            }
        }

        Rectangle {
            id: split_rect
            x: 27
            y: 239
            width: 270
            height: 1
            color: "#cccccc"
            anchors.horizontalCenter: parent.horizontalCenter
            border.width: 0


        }
    }
    Flickable{
        id: terminal_fl
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
        enabled: false
        visible: enabled

        property var terminal_ta: TextArea {
            id: terminal_ta
            anchors.fill: parent
            text: ">_\n"
            font.pointSize: 10
            background: Rectangle{
                color: "#222"
            }
            color: "#fff"
            visible: terminal_fl.enabled
            readOnly: true
            wrapMode: TextEdit.WordWrap

        }

        TextArea.flickable: terminal_ta

        ScrollBar.vertical: ScrollBar{}

    }

    Image {
        id: terminal_btn
        x: parent.width - 16 - 8
        y: 8
        source: "img/downarrow.png"
        opacity: terminal_ma.pressed ? 0.8 : 1

        MouseArea {
            id: terminal_ma
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
        }
    }

    ColorOverlay {
        id: terminal_btn_co
        anchors.fill: terminal_btn
        source: terminal_btn
        visible: false
        color: "#009900"
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
    }

    Connections {
        target: install_ma
        onClicked: {
            popup.closePolicy = Popup.NoAutoClose
            popup.background.color = "#f0f0f0"
            popup.color = "#000"
            popup.text = qsTr("Installing...") + "\n"
            popup.open()
            terminal_btn_co.visible = true
            terminal_fl.terminal_ta.text = "";
            ddpkg.install()
        }
    }

    Connections {
        target: uninstall_ma
        onClicked: {
            popup.closePolicy = Popup.NoAutoClose
            popup.background.color = "#f0f0f0"
            popup.color = "#000"
            popup.text = qsTr("Uninstalling...") + "\n"
            popup.open()
            terminal_btn_co.visible = true
            terminal_fl.terminal_ta.text = "";
            ddpkg.uninstall()

        }
    }

    Connections {
        target: ddpkg

        onPackexistsChanged: {
            uninstall_rect.enabled = ddpkg.packexists
            if (ddpkg.packexists) {
                if (newVersion) {
                    install.source = "img/upgrade.png"
                    install_txt.text = qsTr("Upgrade")
                } else if(oldVersion){
                    install.source = "img/downgrade.png"
                    install_txt.text = qsTr("Downgrade")
                } else {
                    install.source = "img/reinstall.png"
                    install_txt.text = qsTr("Reinstall")
                }
            } else {
                install.source = "img/install.png"
                install_txt.text = qsTr("Install")
            }
        }
    }


    property var popup: Popup{
        id: popup
        width: parent.width
        height: parent.height/3
        y: parent.height / 2 - parent.height / 8
        modal: true
        property string text: ""
        property string color: "#000"

        background: Rectangle{
            color: "#f0f0f0"
        }

        property int lineCount: topic.lineCount
        Text{
            id: topic
            text: popup.text
            color: popup.color
            wrapMode: Text.Wrap
            anchors.fill: parent
        }
    }

}
