#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include <QPainter>
#include <vector>
#include <QStack>
#include <QColorDialog>

class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);
    bool interpolacja_status = true;
    void drawPixel(int x, int y, int B, int G, int R);
    void przeksztalcenia(double x, double y, double alfa, double Sx, double Sy, double Shx, double Shy);
    QRgb interpolacja(QImage* im, double x, double y);

protected:
    void paintEvent(QPaintEvent *event);
signals:

public slots:
    void setRotacja(int value);
    void setLeftRight(int value);
    void setTopDown(int value);
    void setLeftRightSkalowanie(int value);
    void setTopDownSkalowanie(int value);
    void setLeftRightPochylenie(int value);
    void setTopDownPochylenie(int value);
    void reset();

public slots:

private:
    QImage im, im2, temp;
};

#endif // EKRAN_H
