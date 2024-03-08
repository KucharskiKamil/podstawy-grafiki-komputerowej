#ifndef EKRAN_H
#define EKRAN_H

#include<QWidget>
#include <QMouseEvent>
class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);
    void drawPixel(int x, int y);
private:
    QImage im;
    QImage im2;
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
signals:

};

#endif // EKRAN_H
