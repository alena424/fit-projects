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
#include "../AteamMathLib.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize( this->geometry().width(), this->geometry().height() );

    // connections for buttons with numbers
    connect(ui->number1, SIGNAL(clicked()), this, SLOT(clickOnNumberButton()));
    connect(ui->number2, SIGNAL(clicked()), this, SLOT(clickOnNumberButton()));
    connect(ui->number3, SIGNAL(clicked()), this, SLOT(clickOnNumberButton()));
    connect(ui->number4, SIGNAL(clicked()), this, SLOT(clickOnNumberButton()));
    connect(ui->number5, SIGNAL(clicked()), this, SLOT(clickOnNumberButton()));
    connect(ui->number6, SIGNAL(clicked()), this, SLOT(clickOnNumberButton()));
    connect(ui->number7, SIGNAL(clicked()), this, SLOT(clickOnNumberButton()));
    connect(ui->number8, SIGNAL(clicked()), this, SLOT(clickOnNumberButton()));
    connect(ui->number9, SIGNAL(clicked()), this, SLOT(clickOnNumberButton()));
    connect(ui->number0, SIGNAL(clicked()), this, SLOT(clickOnNumberButton()));

    // connections for buttons with operations
    connect(ui->plus, SIGNAL(clicked()), this, SLOT(clickOnOperationButton()));
    connect(ui->minus, SIGNAL(clicked()), this, SLOT(clickOnOperationButton()));
    connect(ui->times, SIGNAL(clicked()), this, SLOT(clickOnOperationButton()));
    connect(ui->div, SIGNAL(clicked()), this, SLOT(clickOnOperationButton()));
    connect(ui->factorial, SIGNAL(clicked()), this, SLOT(clickOnOperationButton()));
    connect(ui->power, SIGNAL(clicked()), this, SLOT(clickOnOperationButton()));
    connect(ui->sqrt, SIGNAL(clicked()), this, SLOT(clickOnOperationButton()));
    connect(ui->log, SIGNAL(clicked()), this, SLOT(clickOnOperationButton()));

    connect(ui->equal, SIGNAL(clicked()), this, SLOT(clickOnOperationButton()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::clickOnNumberButton()
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

void MainWindow::clickOnOperationButton()
{
    double result, epsilon = 0.00000000001;
    QString secondOperand = ui->tapped_nums->toPlainText();
    QString newOperation = ((QPushButton *)sender())->text();
    // first we will change the signification of some operations
    if ( newOperation == "x!" ) {
        newOperation = "!";
    } else if ( newOperation == "x" ) {
        newOperation = "^";
    } else if ( newOperation == "√x" ) {
        newOperation = "yroot";
    } else if ( newOperation == "log x" ) {
        newOperation = "log";
    }

    QString displayString = ui->display->toPlainText();
    // we will continue to depend on whether something is on display
    if ( displayString != "" ) {
        QStringList displayList = displayString.split(" ");
        QString firstOperand = displayList[0];
        QString oldOperation = displayList[1];

        // we calculate the result in dependece on the operation
        if ( oldOperation == "+" ) {
            result = MathLib::AteamMathLib::addition( firstOperand.toDouble(), secondOperand.toDouble() );
        } else if ( oldOperation == "−" ) {
            result = MathLib::AteamMathLib::subtraction( firstOperand.toDouble(), secondOperand.toDouble() );
        } else if ( oldOperation == "×" ) {
            result = MathLib::AteamMathLib::multiplication( firstOperand.toDouble(), secondOperand.toDouble() );
        } else if ( oldOperation == "÷" ) {
            try {
                result = MathLib::AteamMathLib::division( firstOperand.toDouble(), secondOperand.toDouble() );
            } catch ( const std::out_of_range& exception ) {
                errorMessage( exception.what() );
                return;
            }
        } else if ( oldOperation == "^" ) {
            try {
                result = MathLib::AteamMathLib::power( firstOperand.toDouble(), secondOperand.toDouble() );
            } catch ( const std::out_of_range& exception ) {
                errorMessage( exception.what() );
                return;
            }
        } else if ( oldOperation == "yroot" ) {
            if ( secondOperand.toDouble() - secondOperand.toInt() > epsilon ) {
                errorMessage( "The second operand is not an integer." );
                return;
            }
            try {
                result = MathLib::AteamMathLib::nroot( firstOperand.toDouble(), secondOperand.toInt() );
            } catch ( const std::out_of_range& exception ) {
                errorMessage( exception.what() );
                return;
            }
        }

        // we write a calculated result
        ui->display->clear();
        if ( newOperation == "=" ) {
            ui->tapped_nums->clear();
            ui->tapped_nums->append( QString::number( result, 'g', 11 ) );
        } else if ( newOperation == "!" ) {
            // factorial is unary operation, we can calculate result right now
            ui->tapped_nums->clear();
            try {
                result = MathLib::AteamMathLib::factorial( result );
            } catch ( const std::out_of_range& exception ) {
                errorMessage( exception.what() );
                return;
            }
            ui->tapped_nums->append( QString::number( result, 'g', 11 ) );
        } else if ( newOperation == "log" ) {
            // natural logarithm is unary operation, we can calculate result right now
            ui->tapped_nums->clear();
            try {
                result = MathLib::AteamMathLib::logarithm( result );
            } catch ( const std::out_of_range& exception ) {
                errorMessage( exception.what() );
                return;
            }
            ui->tapped_nums->append( QString::number( result, 'g', 11 ) );
        } else {
            ui->display->append( QString::number( result, 'g', 11 ) + " " + newOperation );
        }
    } else if ( newOperation == "!" ) {
        // factorial is unary operation, we can calculate result right now
        ui->tapped_nums->clear();
        if ( secondOperand.toDouble() - secondOperand.toInt() > epsilon  ) {
            errorMessage( "The number is not an integer." );
            return;
        }
        try {
            result = MathLib::AteamMathLib::factorial( secondOperand.toShort() );
        } catch ( const std::out_of_range& exception ) {
            errorMessage( exception.what() );
            return;
        }
        ui->tapped_nums->append( QString::number( result, 'g', 11 ) );
    } else if ( newOperation == "log" ) {
        // natural logarithm is unary operation, we can calculate result right now
        ui->tapped_nums->clear();
        try {
            result = MathLib::AteamMathLib::logarithm( secondOperand.toDouble() );;
        } catch ( const std::out_of_range& exception ) {
            errorMessage( exception.what() );
            return;
        }
        ui->tapped_nums->append( QString::number( result, 'g', 11 ) );
    } else if ( newOperation != "=" ) {
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

void MainWindow::on_help_clicked()
{
    QString appPath = QFileInfo(".").absolutePath();
    int lastSlash = appPath.lastIndexOf("/");
    appPath.chop( appPath.length() - lastSlash - 1 );
    //QString path = "file:///" + "usr/shared/calculatron" + "dokumentace.pdf";
    QString path = "file:///usr/shared/calculatron/dokumentace.pdf";
    if ( ! QDesktopServices::openUrl( QUrl( path ) ) ) {
        QMessageBox::warning( this, "Warning", "Documentation not found.\nPath: " + path, "OK" );
    }
}

void MainWindow::errorMessage( QString message )
{
    QMessageBox::warning( this, "Error", message, "OK" );
    on_clear_clicked();
}

/*** End of file mainwindow.cpp ***/
