TEMPLATE = app

QT += qml quick

HEADERS += lut.h \
           lut_table.h

SOURCES += main.cpp \
           lut.cpp
        

RESOURCES += qml.qrc

OTHER_FILES +=  qml/main.qml \
                qml/MyButton.qml \
                qml/MyClickButton.qml \
                qml/MySlider.qml

OTHER_FILES += js/Theme.js

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
