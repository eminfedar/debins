import QtQuick 2.4

MainForm {
    transitions: Transition {
        from: "*"
        to: "*"
        ColorAnimation{
            properties: "color,border.color"
            duration: 500
            easing.type: Easing.InOutQuad
        }
        NumberAnimation{
            properties: "border.width"
            duration: 500
            easing.type: Easing.InOutQuad
        }
    }
}
