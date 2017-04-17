/**
  * @file  mainwindow.h
  * @brief Declarations of functions for the GUI.
  *
  * Header file for the GUI for project CalculaTron.
  * Contains the prototypes for the GUI functions,
  * constants, global variables and macros.
  *
  * @author Daniel Uhricek (xuhric00)
  * @author Peter Uhrin (xuhrin02)
  * @author Alena Tesarova (xtesar36)
  * @author Jan Sorm (xsormj00)
  */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextBlock>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    /** a global variable to indicate whether the initial number has been changed */
    int isChange = 0;

private slots:
    /**
     * @brief NumberButton clicked
     *
     * Function to control all buttons with numbers.
     */
    void on_numberButton_clicked();

    /**
     * @brief Button with operation clicked
     *
     * Function to control all buttons with operations.
     */
    void on_operation_clicked();

    /**
     * @brief Button with dot clicked
     *
     * Function to control button with dot.
     */
    void on_dot_clicked();

    /**
     * @brief Button with C clicked
     *
     * Function to control the display clear button.
     */
    void on_clear_clicked();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
