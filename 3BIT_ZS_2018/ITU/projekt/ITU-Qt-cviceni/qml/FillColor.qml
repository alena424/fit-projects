import QtQuick 2.4
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import Qt.labs.calendar 1.0

Item {
    id: item1
    width: 32
    height: 32
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
        anchors.fill: parent
        source: "../img/tools/Group.png"
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
