#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->buttonCB, &QPushButton::clicked, ui->ekran, [this]()
    {
        ui->ekran->czarnebiale();
    });
    connect(ui->dylatacja, &QPushButton::clicked, ui->ekran, [this]()
    {
        ui->ekran->dylatacja();
    });
    connect(ui->erozja, &QPushButton::clicked, ui->ekran, [this]()
    {
        ui->ekran->erozja();
    });
    connect(ui->domkniecie, &QPushButton::clicked, ui->ekran, [this]()
    {
        ui->ekran->domkniecie();
    });
    connect(ui->otwarcie, &QPushButton::clicked, ui->ekran, [this]()
    {
        ui->ekran->otwarcie();
    });
    connect(ui->slajder, &QSlider::valueChanged, ui->ekran, [this](int value)
    {
        ui->ekran->changeZmienna(value);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

