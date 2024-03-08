#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQueue>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QImage im_source, im_destination, im1, im2;
    QPoint startPos, endPos;
    int iSourcePoints1=-1, iSourcePoints2=-1;
    int frame=0, frames=200;
    QVector<QPoint> sourcePoints1, sourcePoints2, destinationPoints;

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void drawPixel(QImage *im, QPoint p, QColor color);
    void drawLine(QImage *im, QPoint start, QPoint end, QColor color);
    void draw();
    void drawTriangle(QImage *im, QVector<QPoint> points);
    void fill();
    int findNearbyPoint(QPoint p, QVector<QPoint> points);
    int selectedImage = 1;
private:

private:
    Ui::MainWindow *ui;
public slots:
    void changeLayer(int a);
    void suwak(int frame);
};
#endif // MAINWINDOW_H
