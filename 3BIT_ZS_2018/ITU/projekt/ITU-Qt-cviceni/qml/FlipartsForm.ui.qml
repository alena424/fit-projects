import QtQuick 2.4
import QtQuick.Controls 2.1

Item {
    width: 1080
    height: 120

    Rectangle {
        id: rectangle
        color: "#27405d"
        anchors.fill: parent

        Clipart {
            id: clipart
            y: 0
            width: 120
            anchors.top: parent.top
            anchors.topMargin: 4
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 116
            anchors.left: parent.left
            anchors.leftMargin: 0
        }

        Smile {
            id: smile
            x: 121
            width: 120
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 116
            anchors.top: parent.top
            anchors.topMargin: 4
        }

        ToolSeparator {
            id: toolSeparator2
            x: 357
            y: 8
            width: 13
            height: 98
            z: 1
        }

        ToolSeparator {
            id: toolSeparator
            x: 114
            y: 8
            width: 13
            height: 98
        }

        ToolSeparator {
            id: toolSeparator1
            x: 234
            y: 8
            width: 13
            height: 98
        }

        Comment {
            id: comment
            x: 367
            y: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 4
        }

        TextPole {
            id: textPole
            x: 247
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 4
            anchors.top: parent.top
            anchors.topMargin: 0
        }
    }
}


/*##^## Designer {
    D{i:2;anchors_height:200;anchors_width:200;anchors_x:13;anchors_y:0}D{i:4;anchors_height:0;anchors_x:0;anchors_y:4}
D{i:7;anchors_y:4}
}
 ##^##*/
