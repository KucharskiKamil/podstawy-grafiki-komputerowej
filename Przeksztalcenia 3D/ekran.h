#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>



class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);

private:
    QImage im;
    struct punkt3D
    {
        int x;
        int y;
        int z;
    };
    QVector<punkt3D> punkty;
    QVector<QPoint> punkty2D;
    int alphaX=0, alphaY=0, alphaZ=0;
    double sX=1,sY=1,sZ=1;
    double tX=0,tY=0,tZ=0;



    void drawPixel(int x, int y);
    void scanLine(QVector<QPoint> P);
public:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
    void drawLine(int x1, int y1, int x2, int y2);
    void rysuj(QVector<QPoint>);
    void dwaDtrzyD(QVector<punkt3D>);
    void liczMacierze();
public slots:
    void transX(int);
    void transY(int);
    void transZ(int);
    void scaleX(int);
    void scaleY(int);
    void scaleZ(int);
    void rotX(int);
    void rotY(int);
    void rotZ(int);
};

#endif // EKRAN_H
