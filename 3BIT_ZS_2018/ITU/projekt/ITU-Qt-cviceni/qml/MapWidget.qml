import QtQuick 2.4

MapWidgetForm {
    mouseArea.onClicked: {

}


    Keys.onUpPressed: scrollBar.decrease()
    Keys.onDownPressed: scrollBar.increase()
}
