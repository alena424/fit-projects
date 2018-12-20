TEMPLATE = app


QT += quick widgets quickcontrols2

HEADERS += \
    document.h \
    photo.h \
    paper.h \
    liveimageprovider.h \
    menuicons.h

SOURCES += main.cpp \
    document.cpp \
    photo.cpp \
    paper.cpp \
    liveimageprovider.cpp
        

RESOURCES += qml.qrc  \
    plus.png

OTHER_FILES +=  qml/main.qml \
                qml/MyButton.qml \
                qml/MyClickButton.qml \
                qml/MySlider.qml \
                qml/TabWidget.qml

OTHER_FILES +=

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

#DISTFILES += \
 #   qml/TabWidget.qml

DISTFILES += \
    qml/DropArea.qml \
    plus.png \
    qml/BoldForm.ui.qml \
    qml/ForwardForm.ui.qml \
    qml/ItalicForm.ui.qml \
    qml/RotateLeftForm.ui.qml \
    qml/RotateRightForm.ui.qml \
    qml/TextFillForm.ui.qml \
    qml/TextMenuForm.ui.qml \
    qml/Bold.qml \
    qml/Crop.qml \
    qml/FillColor.qml \
    qml/Forward.qml \
    qml/Italic.qml \
    qml/main.qml \
    qml/RotateLeft.qml \
    qml/RotateRight.qml \
    qml/TabWidget.qml \
    qml/Text.qml \
    qml/TextMenu.qml \
    qml/Underline.qml \
    qml/BoldForm.ui.qml \
    qml/ForwardForm.ui.qml \
    qml/FrontMenuToolsForm.ui.qml \
    qml/ItalicForm.ui.qml \
    qml/MapForm.ui.qml \
    qml/RotateLeftForm.ui.qml \
    qml/RotateRightForm.ui.qml \
    qml/TextFillForm.ui.qml \
    qml/TextMenuForm.ui.qml \
    qml/Bold.qml \
    qml/Crop.qml \
    qml/DropArea.qml \
    qml/FillColor.qml \
    qml/Forward.qml \
    qml/FrontMenuTools.qml \
    qml/Italic.qml \
    qml/main.qml \
    qml/Map.qml \
    qml/MyButton.qml \
    qml/MyClickButton.qml \
    qml/MySlider.qml \
    qml/RotateLeft.qml \
    qml/RotateRight.qml \
    qml/TabWidget.qml \
    qml/Text.qml \
    qml/TextFill.qml \
    qml/TextMenu.qml \
    qml/Underline.qml \
    img/tools/Group 2.png \
    img/tools/Group.png \
    img/tools/Vector-1.png \
    img/tools/Vector-10.png \
    img/tools/Vector-2.png \
    img/tools/Vector-3.png \
    img/tools/Vector-4.png \
    img/tools/Vector-5.png \
    img/tools/Vector-6.png \
    img/tools/Vector-7.png \
    img/tools/Vector-8.png \
    img/tools/Vector-9.png \
    img/tools/Vector.png \
    img/baseline-add_location-24px.png \
    img/redo.png \
    img/Vector-1.png \
    img/Vector-2.png \
    img/Vector-3.png \
    img/Vector-4.png \
    img/Vector-5.png \
    img/Vector-6.png \
    img/Vector.png \
    img/baseline-brightness_6-24px.svg \
    img/baseline-exposure-24px.svg \
    img/baseline-format_align_center-24px.svg \
    img/baseline-format_align_justify-24px.svg \
    img/baseline-format_align_left-24px.svg \
    img/baseline-format_align_right-24px.svg \
    img/baseline-format_bold-24px.svg \
    img/baseline-format_color_fill-24px.svg \
    img/baseline-format_color_text-24px.svg \
    img/baseline-format_italic-24px.svg \
    img/baseline-format_list_bulleted-24px.svg \
    img/baseline-format_list_numbered-24px.svg \
    img/baseline-format_list_numbered_rtl-24px.svg \
    img/baseline-format_underlined-24px.svg \
    img/baseline-redo-24px.svg \
    img/baseline-rotate_left-24px.svg \
    img/baseline-rotate_right-24px.svg \
    img/baseline-title-24px.svg \
    img/baseline-undo-24px.svg \
    img/tools/Group 2.png \
    img/tools/Group.png \
    img/tools/Vector-1.png \
    img/tools/Vector-10.png \
    img/tools/Vector-2.png \
    img/tools/Vector-3.png \
    img/tools/Vector-4.png \
    img/tools/Vector-5.png \
    img/tools/Vector-6.png \
    img/tools/Vector-7.png \
    img/tools/Vector-8.png \
    img/tools/Vector-9.png \
    img/tools/Vector.png \
    img/baseline-add_location-24px.png \
    img/redo.png \
    img/Vector-1.png \
    img/Vector-2.png \
    img/Vector-3.png \
    img/Vector-4.png \
    img/Vector-5.png \
    img/Vector-6.png \
    img/Vector.png \
    img/baseline-brightness_6-24px.svg \
    img/baseline-exposure-24px.svg \
    img/baseline-format_align_center-24px.svg \
    img/baseline-format_align_justify-24px.svg \
    img/baseline-format_align_left-24px.svg \
    img/baseline-format_align_right-24px.svg \
    img/baseline-format_bold-24px.svg \
    img/baseline-format_color_fill-24px.svg \
    img/baseline-format_color_text-24px.svg \
    img/baseline-format_italic-24px.svg \
    img/baseline-format_list_bulleted-24px.svg \
    img/baseline-format_list_numbered-24px.svg \
    img/baseline-format_list_numbered_rtl-24px.svg \
    img/baseline-format_underlined-24px.svg \
    img/baseline-redo-24px.svg \
    img/baseline-rotate_left-24px.svg \
    img/baseline-rotate_right-24px.svg \
    img/baseline-title-24px.svg \
    img/baseline-undo-24px.svg \
    qml/MapForm.ui.qml \
    qml/Map.qml \
    qml/Style.qml \
    qml/texteditor/texteditor.qml \
    img/bryle.png \
    img/leo-rivas-130488-unsplash.png \
    qml/MapWidgetForm.ui.qml \
    qml/MapWidget.qml \
    qml/Brightness.qml \
    qml/ViewForm.ui.qml \
    qml/View.qml \
    img/view.png \
    qml/View.qml \
    qml/AlignCenterForm.ui.qml \
    qml/AlignRightForm.ui.qml \
    qml/BackForm.ui.qml \
    qml/BoldForm.ui.qml \
    qml/CenterAlignForm.ui.qml \
    qml/ClipartForm.ui.qml \
    qml/CommentForm.ui.qml \
    qml/ExportForm.ui.qml \
    qml/FlipartsForm.ui.qml \
    qml/ForwardForm.ui.qml \
    qml/FrontMenuToolsForm.ui.qml \
    qml/ItalicForm.ui.qml \
    qml/LeftAlignForm.ui.qml \
    qml/MapForm.ui.qml \
    qml/MapWidgetForm.ui.qml \
    qml/MenuForm.ui.qml \
    qml/RotateLeftForm.ui.qml \
    qml/RotateRightForm.ui.qml \
    qml/SmileForm.ui.qml \
    qml/TextFillForm.ui.qml \
    qml/TextChangeForm.ui.qml \
    qml/TextMenuForm.ui.qml \
    qml/ViewForm.ui.qml \
    qml/texteditor.html \
    qml/AlignCenter.qml \
    qml/AlignRight.qml \
    qml/Back.qml \
    qml/BackNew.qml \
    qml/Bold.qml \
    qml/Brightness.qml \
    qml/CenterAlign.qml \
    qml/Clipart.qml \
    qml/Comment.qml \
    qml/Crop.qml \
    qml/DropArea.qml \
    qml/Export.qml \
    qml/FillColor.qml \
    qml/Fliparts.qml \
    qml/Forward.qml \
    qml/FrontMenuTools.qml \
    qml/Italic.qml \
    qml/LeftAlign.qml \
    qml/main.qml \
    qml/Map.qml \
    qml/MapWidget.qml \
    qml/Menu.qml \
    qml/MyButton.qml \
    qml/MyClickButton.qml \
    qml/MySlider.qml \
    qml/RotateLeft.qml \
    qml/RotateRight.qml \
    qml/Smile.qml \
    qml/Style.qml \
    qml/TabWidget.qml \
    qml/Text.qml \
    qml/TextFill.qml \
    qml/TextChange.qml \
    qml/TextMenu.qml \
    qml/Underline.qml \
    qml/View.qml
