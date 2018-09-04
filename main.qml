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
            if(packageExists){
                mainForm.newVersion = newVersion
                mainForm.oldVersion = oldVersion
                if(newVersion || oldVersion){
                    mainForm.currentVersion = packageCurrentVersion + " -> " + packageVersion;
                }else{
                    mainForm.currentVersion = "";
                }
            }
            packexists = packageExists
        }

        onInstallFinished: {
            mainForm.popup.background.color = "#007700"
            mainForm.popup.color = "#fff"
            mainForm.popup.text = qsTr("Successfully Installed!")
            mainForm.popup.closePolicy = Popup.CloseOnEscape | Popup.CloseOnPressOutside

            mainForm.newVersion = false;
            mainForm.oldVersion = false;
            mainForm.install.source = "img/reinstall.png"
            mainForm.install_txt.text = qsTr("Reinstall")
            packexists = true

            mainForm.currentVersion = ""
        }
        onInstallError: {
            mainForm.popup.background.color = "#AA0000"
            mainForm.popup.color = "#fff"
            mainForm.popup.text = qsTr("Error: ") + errorCode
            mainForm.popup.closePolicy = Popup.CloseOnEscape | Popup.CloseOnPressOutside

        }
        onUninstallFinished: {
            mainForm.popup.background.color = "#007700"
            mainForm.popup.color = "#fff"
            mainForm.popup.text = qsTr("Successfully Uninstalled!")
            mainForm.popup.closePolicy = Popup.CloseOnEscape | Popup.CloseOnPressOutside

            mainForm.newVersion = false;
            mainForm.oldVersion = false;
            packexists = false

            mainForm.currentVersion = ""
        }
        onUninstallError: {
            mainForm.popup.background.color = "#AA0000"
            mainForm.popup.color = "#fff"
            mainForm.popup.text = qsTr("Error: ") + errorCode
            mainForm.popup.closePolicy = Popup.CloseOnEscape | Popup.CloseOnPressOutside
        }
    }
}
