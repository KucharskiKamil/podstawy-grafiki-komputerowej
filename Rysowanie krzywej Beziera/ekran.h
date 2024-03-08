#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include <QMouseEvent>
using namespace std;
class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);


private:
    QImage im;
    QImage im2;
protected:
    void drawPixel(int x, int y);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void drawLine(int x_0,int y_0,int x_1, int y_1);
    void wypiszPunkty(vector<vector<int>> vect);
    void removePixel(int x, int y);
    void bezier(vector<vector<int>> vect, int k);
    void mouseReleaseEvent(QMouseEvent *e);
    void usunKrzywa(vector<vector<int>> vect);
    void rysujPixele();
signals:

};

#endif // EKRAN_H
