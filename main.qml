import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import org.debins.ddpkg 1.0

ApplicationWindow {
    visible: true
    width: 320
    height: 425
    title: ddpkg.fileName

    MainForm{
        id: mainForm
    }
    DDpkg{
        id: ddpkg
        property bool packexists: false

        Component.onCompleted: {
            packexists = packageExists
        }

        onInstallFinished: {
            mainForm.popup.background.color = "#f0f0f0"
            mainForm.popup.color = "#000"
            mainForm.popup.text = qsTr("Successfully Installed!")
            mainForm.popup.closePolicy = Popup.CloseOnEscape | Popup.CloseOnPressOutside
            packexists = true
        }
        onInstallError: {
            mainForm.popup.background.color = "#AA0000"
            mainForm.popup.color = "#fff"
            mainForm.popup.text = qsTr("Error: ") + errorCode
            mainForm.popup.closePolicy = Popup.CloseOnEscape | Popup.CloseOnPressOutside

        }
        onUninstallFinished: {
            mainForm.popup.background.color = "#f0f0f0"
            mainForm.popup.color = "#000"
            mainForm.popup.text = qsTr("Successfully Uninstalled!")
            mainForm.popup.closePolicy = Popup.CloseOnEscape | Popup.CloseOnPressOutside
            packexists = false
        }
        onUninstallError: {
            mainForm.popup.background.color = "#AA0000"
            mainForm.popup.color = "#fff"
            mainForm.popup.text = qsTr("Error: ") + errorCode
            mainForm.popup.closePolicy = Popup.CloseOnEscape | Popup.CloseOnPressOutside
        }
    }
}
