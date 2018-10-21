import QtQuick 2.9
import QtQuick.Window 2.2
import an.OpenGLItem 1.0

Window
{
    visible: true
    width: Qt.platform.os == "android" ? Screen.desktopAvailableWidth  : 640;
    height: Qt.platform.os == "android" ? Screen.desktopAvailableHeight : 480

    title: qsTr("MPS Opengl Qt/Quick 教程(0)!")

    OpenGLItem
    {
        id: openGLItem
        visible: true
        anchors.fill: parent
    }
}
