#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    connect(ui->slajderRed, &QSlider::valueChanged, ui->ekran, [=](int value)
    {
        ui->ekran->changeDrawColor(value, 1,ui->labelColorDisplay,ui->label_RGBValue);
    });
    connect(ui->slajderGreen, &QSlider::valueChanged, ui->ekran, [=](int value)
    {
        ui->ekran->changeDrawColor(value, 2,ui->labelColorDisplay,ui->label_RGBValue);
    });
    connect(ui->slajderBlue, &QSlider::valueChanged, ui->ekran, [=](int value)
    {
        ui->ekran->changeDrawColor(value, 3,ui->labelColorDisplay,ui->label_RGBValue);
    });
    connect(ui->listWidget, &QListWidget::itemClicked, ui->ekran,std::bind(&Ekran::changeDrawMode, ui->ekran,std::placeholders::_1, ui->slajderLiczbaPunktow, ui->labelPunkciki));

    connect(ui->slajderLiczbaPunktow, &QSlider::valueChanged, ui->ekran, [=](int value){
        ui->ekran->changeEllipsePoints(value, ui->labelPunkciki);
    });
    connect(ui->clearButton, SIGNAL(clicked()), ui->ekran, SLOT(clearScreen()));

    ui->labelPunkciki->setVisible(false);
    ui->slajderLiczbaPunktow->setVisible(false);
}
MainWindow::~MainWindow()
{
    delete ui;
}

