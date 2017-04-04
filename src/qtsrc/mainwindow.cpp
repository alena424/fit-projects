#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->tlac2, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tlac3, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tlac4, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tlac5, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tlac6, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tlac7, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tlac8, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tlac9, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tlac0, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));

    connect(ui->minus, SIGNAL(clicked()), this, SLOT(on_plus_clicked()));
    connect(ui->times, SIGNAL(clicked()), this, SLOT(on_plus_clicked()));
    connect(ui->div, SIGNAL(clicked()), this, SLOT(on_plus_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QTextDocument *text = ui->plainTextEdit->document();
    int numberOfLines = ui->plainTextEdit->blockCount();
    QString lines[numberOfLines];
    for (int i = 0; i < numberOfLines; i++) {
        QTextBlock line = text->findBlockByLineNumber(i);
        lines[i] = line.text();
    }

    ui->plainTextEdit->clear();
    if (lines[0] != "0") {
        ui->plainTextEdit->appendPlainText(lines[0]+((QPushButton *)sender())->text());
    }
    else {
        ui->plainTextEdit->appendPlainText(((QPushButton *)sender())->text());
    }

    for (int i = 1; i < numberOfLines; i++) {
        ui->plainTextEdit->appendPlainText(lines[i]);
    }
}

void MainWindow::on_plus_clicked()
{
    QTextDocument *text = ui->plainTextEdit->document();
    int numberOfLines = ui->plainTextEdit->blockCount();
    QString lines[numberOfLines];
    for (int i = 0; i < numberOfLines; i++) {
        QTextBlock line = text->findBlockByLineNumber(i);
        lines[i] = line.text();
    }

    ui->plainTextEdit->clear();
    if (numberOfLines == 3) {
        QString symbol = lines[1];
        int result;
        if (symbol == "+") {
            result = lines[2].toInt() + lines[0].toInt();
        } else if (symbol == "-") {
            result = lines[2].toInt() - lines[0].toInt();
        } else if (symbol == "×") {
            result = lines[2].toInt() * lines[0].toInt();
        } else if (symbol == "÷") {
            result = lines[2].toInt() / lines[0].toInt();
        }
        ui->plainTextEdit->insertPlainText("0\n"+((QPushButton *)sender())->text()+"\n"+QString::number(result));
    } else {
        ui->plainTextEdit->insertPlainText("0\n"+((QPushButton *)sender())->text()+"\n"+lines[0]);
    }

/*    QTextDocument *text = ui->plainTextEdit->document();
    QTextBlock firstLine = text->findBlockByLineNumber(0);
    QString str = firstLine.text();
    ui->plainTextEdit->clear();
    ui->plainTextEdit->insertPlainText("0\n"+((QPushButton *)sender())->text()+"\n"+str);*/
    //int vysledek = ui->lineEdit_2->text().toInt() + ui->lineEdit->text().toInt();

      //  ui->lineEdit->setText(QString::number(vysledek));
        //ui->lineEdit_2->setText("0");
}
