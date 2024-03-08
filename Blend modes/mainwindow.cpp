#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->alfaSlajder, SIGNAL(valueChanged(int)), ui->ekran, SLOT(setAlfa(int)));
    connect(ui->rb11, &QRadioButton::toggled, ui->ekran, [=](bool checked) {
        if (checked) {
            ui->ekran->setValue(1, 1);
        }
    });
    connect(ui->rb12, &QRadioButton::toggled, ui->ekran, [=](bool checked) {
        if (checked) {
            ui->ekran->setValue(1, 2);
        }
    });
    connect(ui->rb13, &QRadioButton::toggled, ui->ekran, [=](bool checked) {
        if (checked) {
            ui->ekran->setValue(1, 3);
        }
    });
    connect(ui->rb21, &QRadioButton::toggled, ui->ekran, [=](bool checked) {
        if (checked) {
            ui->ekran->setValue(2, 1);
        }
    });
    connect(ui->rb22, &QRadioButton::toggled, ui->ekran, [=](bool checked) {
        if (checked) {
            ui->ekran->setValue(2, 2);
        }
    });
    connect(ui->rb23, &QRadioButton::toggled, ui->ekran, [=](bool checked) {
        if (checked) {
            ui->ekran->setValue(2, 3);
        }
    });
    connect(ui->rb24, &QRadioButton::toggled, ui->ekran, [=](bool checked) {
        if (checked) {
            ui->ekran->setValue(2, 4);
        }
    });
    connect(ui->rb25, &QRadioButton::toggled, ui->ekran, [=](bool checked) {
        if (checked) {
            ui->ekran->setValue(2, 5);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

