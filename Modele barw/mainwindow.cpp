#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->verR, &QSlider::valueChanged, ui->ekran, [this](int value)
    {
        ui->ekran->changeValue(value, 1);
    });
    connect(ui->verG, &QSlider::valueChanged, ui->ekran, [this](int value)
    {
        ui->ekran->changeValue(value, 2);
    });
    connect(ui->verB, &QSlider::valueChanged, ui->ekran, [this](int value)
    {
        ui->ekran->changeValue(value, 3);
    });
    connect(ui->verH, &QSlider::valueChanged, ui->ekran, [this](int value)
    {
        ui->ekran->changeValue(value, 4);
    });
    connect(ui->verS, &QSlider::valueChanged, ui->ekran, [this](int value)
    {
        ui->ekran->changeValue(value, 5);
    });
    connect(ui->verV, &QSlider::valueChanged, ui->ekran, [this](int value)
    {
        ui->ekran->changeValue(value, 6);
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

