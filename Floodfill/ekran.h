#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include <QMouseEvent>
#include <qlabel.h>
#include <QListWidget>
using namespace std;
class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);

public slots:
    void changeDrawColor(int n, int a, QLabel* label,QLabel* label2);
    void changeDrawMode(QListWidgetItem* item, QSlider* slider, QLabel* label);
    void changeEllipsePoints(int n,QLabel* label);
private:
    QImage im;
    QImage im2;
protected:
    void drawPixel(int x, int y);
    void drawCircle(int x, int y,int r);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void drawElipse(int x0, int y0, int x1, int y1);
    void drawLine(int x_0,int y_0,int x_1, int y_1);
    void drawCurvePoints(vector<vector<int>> vect);
    void consoleCurvePoints(vector<vector<int>> vect);
    void drawBezier();
    void drawBspline();
    void floodFill(int beginingX, int beginingY);
signals:

};

#endif // EKRAN_H
