#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->slajder, SIGNAL(valueChanged(int)), ui->ekran, SLOT(wartosc(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

