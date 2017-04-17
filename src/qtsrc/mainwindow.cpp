/**
  * @file  mainwindow.cpp
  * @brief Definitions of functions for the GUI.
  *
  * File containing definitions of functions for GUI, their implementation etc.
  *
  * @author Daniel Uhricek (xuhric00)
  * @author Peter Uhrin (xuhrin02)
  * @author Alena Tesarova (xtesar36)
  * @author Jan Sorm (xsormj00)
  */

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->number1, SIGNAL(clicked()), this, SLOT(on_numberButton_clicked()));
    connect(ui->number2, SIGNAL(clicked()), this, SLOT(on_numberButton_clicked()));
    connect(ui->number3, SIGNAL(clicked()), this, SLOT(on_numberButton_clicked()));
    connect(ui->number4, SIGNAL(clicked()), this, SLOT(on_numberButton_clicked()));
    connect(ui->number5, SIGNAL(clicked()), this, SLOT(on_numberButton_clicked()));
    connect(ui->number6, SIGNAL(clicked()), this, SLOT(on_numberButton_clicked()));
    connect(ui->number7, SIGNAL(clicked()), this, SLOT(on_numberButton_clicked()));
    connect(ui->number8, SIGNAL(clicked()), this, SLOT(on_numberButton_clicked()));
    connect(ui->number9, SIGNAL(clicked()), this, SLOT(on_numberButton_clicked()));
    connect(ui->number0, SIGNAL(clicked()), this, SLOT(on_numberButton_clicked()));

    connect(ui->plus, SIGNAL(clicked()), this, SLOT(on_operation_clicked()));
    connect(ui->minus, SIGNAL(clicked()), this, SLOT(on_operation_clicked()));
    connect(ui->times, SIGNAL(clicked()), this, SLOT(on_operation_clicked()));
    connect(ui->div, SIGNAL(clicked()), this, SLOT(on_operation_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_eqv_clicked()
{
    ui->plainTextEdit->appendPlainText("42");
}

void MainWindow::on_numberButton_clicked()
{
    QString tapp_num = ui->tapped_nums->toPlainText();

    if (((QPushButton *)sender())->text() == "0" and tapp_num == "0") {
        return;
    }

    ui->tapped_nums->clear();
    if (isChange) {
        ui->tapped_nums->append(tapp_num+((QPushButton *)sender())->text());
    }
    else {
        ui->tapped_nums->append(((QPushButton *)sender())->text());
        isChange = 1;
    }
}

void MainWindow::on_operation_clicked()
{
    QString secondOperand = ui->tapped_nums->toPlainText();
    QString newOperation = ((QPushButton *)sender())->text();

    QString displayString = ui->display->toPlainText();
    if ( displayString != "" ) {
        QStringList displayList = displayString.split(" ");
        QString firstOperand = displayList[0];
        QString oldOperation = displayList[1];

        // TODO Call functions from math library
        float result;
        if ( oldOperation == "+" ) {
            result = firstOperand.toFloat() + secondOperand.toFloat();
        } else if ( oldOperation == "−" ) {
            result = firstOperand.toFloat() - secondOperand.toFloat();
        } else if ( oldOperation == "×" ) {
            result = firstOperand.toFloat() * secondOperand.toFloat();
        } else if ( oldOperation == "÷" ) {
            result = firstOperand.toFloat() / secondOperand.toFloat();
        }

        ui->display->clear();
        ui->display->append( QString::number(result) + " " + newOperation );
    } else {
        ui->display->append(secondOperand + " " + newOperation);
    }
    isChange = 0;
}

void MainWindow::on_dot_clicked()
{
    QString string = ui->tapped_nums->toPlainText();
    ui->tapped_nums->clear();
    if ( !string.contains(".") ) {
        ui->tapped_nums->insertPlainText(string + ".");
        isChange = 1;
    }
    else {
        ui->tapped_nums->insertPlainText(string);
    }
}

void MainWindow::on_clear_clicked()
{
    ui->display->clear();
    ui->tapped_nums->clear();
    ui->tapped_nums->insertPlainText("0");
    isChange = 0;
}
