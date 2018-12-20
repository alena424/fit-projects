import QtQuick 2.0

// Základem každého prvku je čtverec
Rectangle {
    id: btn;
    width: 100
    height: 100

    // Proměnné, které mají parametr property jsou přístupné z venčí
    property bool toggled;
    property alias text: txt.text;
    property color btnColor: "#777777";

    // Definování signálu
    signal clicked();


    // Změna barvy na základě, jestli je tlačítko aktivní "toggled"
    color: toggled ? Qt.darker(btnColor) : btnColor
    border.color: "#bbbbbb";
    border.width: 3;


    // Oblast pro zachytávaní události myši
    MouseArea {
        id: mouse;
        anchors.fill: parent;
        onClicked: {
            btn.clicked(); // volání signálu definovaného výše
        }
    }

    // Textový prvek pro zobrazení textu
    Text {
        id: txt;
        anchors.fill: parent;

        color: toggled ? "#0066FF" : "black"
        font.pointSize: 22;
        font.bold: true;

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

    }

}
