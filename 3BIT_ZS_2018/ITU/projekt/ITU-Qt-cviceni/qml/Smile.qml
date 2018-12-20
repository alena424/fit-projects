import QtQuick 2.4

SmileForm {
    property string colorActive: '#2D4A6B'
    property string colorNonActive: '#27405d'

    property bool disable: false

    mouseArea1.cursorShape: "PointingHandCursor"

    mouseArea1.onExited: {
        rectangleColor = colorNonActive
        cursorShape: "ArrowCursor"
        rectangle1.color = '#11253B';

    }
    mouseArea1.onEntered: {
        if (disable  ){
            cursorShape: "ForbiddenCursor"
        } else {
            rectangleColor = colorActive
            rectangle1.color = '#FEC72F';
        }

    }
}
