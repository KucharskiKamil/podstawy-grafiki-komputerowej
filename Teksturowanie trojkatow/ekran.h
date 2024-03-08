#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include <QPainter>

class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);
    int co_rysowac = 0;
    void rysujPunkty();
    bool narysowane = false;
    bool interpolacjaValue = true;
    QColor color;
    QVector<QPointF> punkty;
    QVector<QPointF> punkty2;

    void drawPixel(int x, int y, int B, int G, int R);
    void drawLine(int x0, int y0, int x1, int y1);
    void dzialaj();
    QRgb interpolacja(QImage* im, double x, double y);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
signals:
public slots:
    void czysc();
    void dzialaj2();
    void interChange();

private:
    QImage im, im2;
};

#endif // EKRAN_H
