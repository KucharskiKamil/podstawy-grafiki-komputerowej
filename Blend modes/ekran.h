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
    void drawPixel(int x, int y, int r, int g, int b);
    struct warstwa
    {
        QImage im;
        float alfa = 0.0;
        int blendType = 1;
    };
    void drawWarstwy();
    void mieszaj(QImage bg, QImage fg, float alfa, int tryb);
private:
    QImage im;
protected:
    void paintEvent(QPaintEvent *event);
signals:
public slots:
    void setAlfa(int a);
    void setValue(int a, int b);
};

#endif // EKRAN_H
