/**
  * @file  main.cpp
  * @brief Definitions of main function for the GUI.
  *
  * File containing definitions of main functions for GUI, their implementation etc.
  *
  * @author Daniel Uhricek (xuhric00)
  * @author Peter Uhrin (xuhrin02)
  * @author Alena Tesarova (xtesar36)
  * @author Jan Sorm (xsormj00)
  */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

/*** End of file main.cpp ***/
