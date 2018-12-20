import QtQuick 2.4
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import Qt.labs.calendar 1.0

Item {
    id: item1
    width: 17
    height: 24
    property alias rectangleColor: rectangle1.color
    property string colorActive: '#2D4A6B'
    property string colorNonActive: '#27405d'
    property bool disable: false

    Rectangle {
        id: rectangle1
        color: "#27405d"
        anchors.fill: parent
    }

    Image {
        id: image
        width: 11
        height: 16
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        source: "../img/tools/Vector-5.png"
    }
    /*MouseArea {
        id: mouseArea1
        hoverEnabled: true
        anchors.fill: parent
        cursorShape: "PointingHandCursor"

        onExited: {
            if (! disable){
                rectangleColor = colorNonActive

            } else {
                cursorShape: "ForbiddenCursor"
            }
        }
        onEntered: {
            rectangleColor = colorActive
            cursorShape: "ArrowCursor"
        }

    }*/
}
