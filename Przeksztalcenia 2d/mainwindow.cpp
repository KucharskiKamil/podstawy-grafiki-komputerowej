#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->x_slider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setLeftRight(int)));
    connect(ui->y_slider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setTopDown(int)));

    connect(ui->alfa_slider, &QSlider::valueChanged, this, [this](int value) {
        ui->widget->setRotacja(value);
        ui->labelObracanie->setText(QString::number(value));
    });

    connect(ui->sx_slider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setLeftRightSkalowanie(int)));
    connect(ui->sy_slider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setTopDownSkalowanie(int)));
    connect(ui->shx_slider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setLeftRightPochylenie(int)));
    connect(ui->shy_slider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setTopDownPochylenie(int)));


    connect(ui->reset, &QPushButton::clicked, this, [this]() {
        ui->widget->reset();
        ui->x_slider->setValue(0);
        ui->y_slider->setValue(0);
        ui->alfa_slider->setValue(0);
        ui->sx_slider->setValue(100);
        ui->sy_slider->setValue(100);
        ui->shx_slider->setValue(0);
        ui->shy_slider->setValue(0);
    });
}
MainWindow::~MainWindow()
{
    delete ui;
}




