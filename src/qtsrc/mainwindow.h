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
#include <QDesktopServices>
#include <QFileInfo>
#include <QMessageBox>


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
     * @brief Button with number was clicked
     *
     * Function to control all buttons with numbers.
     * If someone clicks on the button with number,
     * number on the button appears on the "tapped nums".
     */
    void clickOnNumberButton();

    /**
     * @brief Button with operation was clicked
     *
     * Function to control all buttons with operations.
     * If someone clicks on the button with operation,
     * so we find out what operation is going on.
     * And accordingly we call the function from the math library
     * and print the text on the display.
     */
    void clickOnOperationButton();

    /**
     * @brief Button with dot clicked
     *
     * Function to control button with dot.
     * It prints a decimal point if no dot is not printed yet.
     */
    void on_dot_clicked();

    /**
     * @brief Button with C clicked
     *
     * Function to control the display clear button.
     * It deletes everything on the display and prints zero.
     */
    void on_clear_clicked();

    /**
     * @brief Show pdf documentation
     *
     * Function opens the pdf documentation.
     */
    void on_help_clicked();

    /**
     * @brief Show error message
     *
     * Function open dialog window with error message and
     * it calls function on_clear_clicked.
     *
     * @param message String with error message
     */
    void errorMessage( QString message );

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
