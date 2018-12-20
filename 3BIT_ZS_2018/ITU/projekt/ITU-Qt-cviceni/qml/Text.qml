import QtQuick 2.4
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import Qt.labs.calendar 1.0

Item {
    id: item1
    width: 120
    height: 116
    property string colorActive: '#2D4A6B'
    property string colorNonActive: '#27405d'
    property bool disable: false

    Rectangle {
        id: rectangle
        color: "#27405d"
        anchors.fill: parent

        Image {
            id: image
            x: 44
            y: 16
            width: 34
            height: 37
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            fillMode: Image.Stretch
            source: "../img/tools/Vector-4.png"
        }

        Text {
            id: text1
            x: 54
            y: 64
            width: 59
            height: 35
            color: "#e1eeff"
            text: qsTr("Přidat text")
            anchors.horizontalCenterOffset: 1
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 14
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
}
