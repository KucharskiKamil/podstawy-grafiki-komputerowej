#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QtCore>
#include <QRadioButton>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->czysc, SIGNAL(clicked()), ui->widget, SLOT(czysc()));

    QObject::connect(ui->interpolacjaButton, &QPushButton::clicked, this, [this]() {
        ui->widget->interChange();
        if(ui->interpolacjaButton->text()=="Interpolacja on")
        {
            ui->interpolacjaButton->setText("Interpolacja off");
        }
        else
        {
            ui->interpolacjaButton->setText("Interpolacja on");
        }

    });
}
MainWindow::~MainWindow()
{
    delete ui;
}


