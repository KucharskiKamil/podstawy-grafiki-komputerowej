#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include <QMouseEvent>
class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);

public slots:
    void wartosc(int);
private:
    QImage im;
    QImage im2;
protected:
    void drawPixel(int x, int y);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void drawElipse(int x0, int y0, int x1, int y1);
    void drawLine(int x_0,int y_0,int x_1, int y_1);
signals:

};

#endif // EKRAN_H
