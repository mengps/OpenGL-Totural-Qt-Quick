import QtQuick 2.9
import QtQuick.Window 2.2
import an.OpenGLItem 1.0

Window
{
    visible: true
    width: Qt.platform.os == "android" ? Screen.desktopAvailableWidth  : 640
    height: Qt.platform.os == "android" ? Screen.desktopAvailableHeight : 480

    title: qsTr("MPS Opengl Qt/Quick 教程(1)!")

    OpenGLItem
    {
        id: openGLItem
        visible: true
        anchors.fill: parent
    }

    Rectangle
    {
        id: tips
        width: text.implicitWidth + 20
        height: text.implicitHeight + 40
        color: "#AA333333"
        radius: 10
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.top: parent.top
        anchors.topMargin: 15

        Text
        {
            id: text
            anchors.centerIn: parent
            font.family: "方正"
            font.pointSize: 9
            color: "white"
            text: "FPS：" + openGLItem.fps
        }
    }
}
