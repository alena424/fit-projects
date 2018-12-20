import QtQuick 2.0

// Základem každého prvku je čtverec
// Vlastně vytvořený posuvník
/*Rectangle {

    width: 20
    height: 20
    color: "black"

}
*/

Rectangle {
    id: rectPreview
    width: 200
    height: 200
    opacity: 1.0
    z: click.drag.active ||  click.pressed ? 2 : 1
    property point beginDrag
    property bool caught: false
    Drag.active: click.drag.active

    Component {
        id: noteComponent
        Rectangle {
        width: 100;height: 100;color: "red"
        }
    }

    Item {
        id: container
        anchors.fill: parent
    }

    Item
    {
        id: positionAnchorHook
        width: 0
        height: 0
    }

    MouseArea {
        id: click

        property int oldMouseX

        anchors.fill: parent
        drag.target: parent
        hoverEnabled: true
        onPressed: {
            parent.beginDrag = Qt.point(parent.x, parent.y);
            oldMouseX = mouseX
        }
        onReleased: {
            if( !parent.caught ) {
                backAnimX.from = parent.x;
                backAnimX.to = parent.beginDrag.x;
                backAnimY.from = parent.y;
                backAnimY.to = parent.beginDrag.y;
                backAnim.start()
            } else {
                document.addNewPhoto( 20, 20, 20, 20 );
                document.addNewPhoto( rect.x - paper.x - photo.width, rect.y - paper.y + newPhotoBar.height, rect.width, rect.height );
            }
        }

        onPositionChanged: {

           if (pressed) {
               left.width = left.width + (mouseX - oldMouseX)
           }
       }

    }

    MouseArea
    {
        id: rightBottomPhoto
        width: 4
        opacity: 0.01
        height: 4
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        drag.target: rightBottomPhoto
        drag.axis: Drag.XandYAxis
        cursorShape: Qt.SizeFDiagCursor
        property int clickedXInHandle: 0 // used to keep the mouse pointed at the position you clicked
        property int clickedYInHandle: 0
        property int clickedXPlace: 0
        property int clickedYPlace: 0

        onPressed:
        {
            // We only set the anchors for a second to put the anchor element on the right position. After that's done we release the hook to this element.
            positionAnchorHook.anchors.left = parent.left
            positionAnchorHook.anchors.bottom = parent.bottom
            positionAnchorHook.anchors.top = parent.top
            positionAnchorHook.anchors.right = parent.right
            positionAnchorHook.anchors.left = undefined
            positionAnchorHook.anchors.bottom = undefined
            positionAnchorHook.anchors.top = undefined
            positionAnchorHook.anchors.right = undefined

            // Now hook our object to the achor element
            parent.anchors.left = positionAnchorHook.left
            parent.anchors.top = positionAnchorHook.top

            var mouseToPlaceholder = mapToItem(positionAnchorHook, mouseX, mouseY)
            clickedXInHandle = mouseX
            clickedYInHandle = mouseY
            console.log( "clickedXPlace" + Math.abs(mouseToPlaceholder.x) )
            clickedXPlace = Math.abs(mouseToPlaceholder.x);
            clickedYPlace = Math.abs(mouseToPlaceholder.y);
        }

        onReleased:
        {
            parent.anchors.top = undefined
            parent.anchors.bottom = undefined
            parent.anchors.left = undefined
            parent.anchors.right = undefined
        }

        onPositionChanged:
        {
            var mouseToPlaceholder = mapToItem(positionAnchorHook, mouseX, mouseY)
            var newWidth = Math.abs(mouseToPlaceholder.x) + clickedXInHandle
            var newHeight = Math.abs(mouseToPlaceholder.y) + clickedYInHandle

      //      console.log( "root.x - " + root.x )
      //      console.log( "root.width - " + root.width )
            console.log( "newWidth - " + newWidth )
            console.log( "mouseToPlaceholder.x - " + mouseToPlaceholder.x )
            console.log( "clickedXInHandle - " + clickedXInHandle )
            if ( mouseToPlaceholder.x >= 20 && mouseToPlaceholder.y >= 20 ) {
               parent.width = newWidth
               parent.height = newHeight
            }
        }

        Rectangle
        {
            id: rightBottomHandleRect
            anchors.fill: parent
            color: "black"
        }
    }
}
