import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import org.debins.ddpkg 1.0

ApplicationWindow {
    id: app
    visible: true
    width: 320
    height: 425
    title: ddpkg.fileName

    MainForm{
        id: mainForm
    }

    Connections{
        target: mainForm.terminal_ma
        onClicked: {
            if(!mainForm.terminal_fl.enabled){
                app.setWidth(620)
                app.setHeight(350)
                mainForm.terminal_fl.enabled = true
            }else{
                app.setWidth(320)
                app.setHeight(425)
                mainForm.terminal_fl.enabled = false
            }
        }
    }

    DDpkg{
        id: ddpkg
        property bool packexists: false

        Component.onCompleted: {
            if(packageExists){
                mainForm.newVersion = newVersion
                mainForm.oldVersion = oldVersion
                if(newVersion || oldVersion){
                    mainForm.currentVersion = packageCurrentVersion.substring(0, 8) + " -> " + packageVersion.substring(0, 8);
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

            mainForm.terminal_btn_co.visible = false;
            packexists = true

            mainForm.currentVersion = ""
        }
        onInstallError: {
            mainForm.popup.background.color = "#AA0000"
            mainForm.popup.color = "#fff"
            mainForm.popup.text = qsTr("Error: ") + ddpkg.getErrorMessage(errorCode)
            mainForm.popup.closePolicy = Popup.CloseOnEscape | Popup.CloseOnPressOutside

            mainForm.terminal_btn_co.color = "#990000";
        }
        onUninstallFinished: {
            mainForm.popup.background.color = "#007700"
            mainForm.popup.color = "#fff"
            mainForm.popup.text = qsTr("Successfully Uninstalled!")
            mainForm.popup.closePolicy = Popup.CloseOnEscape | Popup.CloseOnPressOutside

            mainForm.newVersion = false;
            mainForm.oldVersion = false;
            mainForm.terminal_btn_co.visible = false;
            packexists = false

            mainForm.currentVersion = ""
        }
        onUninstallError: {
            mainForm.popup.background.color = "#AA0000"
            mainForm.popup.color = "#fff"
            mainForm.popup.text = qsTr("Error: ") + ddpkg.getErrorMessage(errorCode)
            mainForm.popup.closePolicy = Popup.CloseOnEscape | Popup.CloseOnPressOutside

            mainForm.terminal_btn_co.color = "#990000";
        }

        onOutputChanged: {
            mainForm.terminal_fl.terminal_ta.append(output.replace("\n",""))
            mainForm.popup.text = output
        }
    }
}
