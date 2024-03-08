#include "ekran.h"

#include <QPainter>
Ekran::Ekran(QWidget *parent)
    : QWidget{parent}
{
    //To maluje tylko maly kwadrat na czarno
    im=QImage(400,400,QImage::Format_RGB32);
    im.fill(0);
    im2=QImage(400,400,QImage::Format_RGB32);
    im2.fill(0);
}

//          Ta funkcja maluje caly ekran na bialo
void Ekran::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.fillRect(0,0,width(),height(),Qt::white);
    p.drawImage(0,0,im);
}

//          Lepsza funkcja rysujaca pixel
void Ekran::drawPixel(int x, int y)
{
    if (x < 0 || y < 0 || x >= im.width() || y >= im.height()) {
        return;
    }
    uchar *pix = im.scanLine(y);
    pix[4*x] = 0;
    pix[4*x+1] = 0;
    pix[4*x+2] = 255;
}


double x_1=0;
double y_1=0;
double x_0=0;
double y_0=0;
double z_0;
double z_1;
void Ekran::mousePressEvent(QMouseEvent *e)
{

    qDebug("1 punkt:\tx:%d Y:%d",e->pos().x(),e->pos().y());
    x_0 = e->pos().x();
    y_0 = e->pos().y();
    if((x_0 < 0) || (y_0 < 0) || (x_0 >= im.width()) || (y_0 >= im.width())) return;
    z_0 = x_0;
    z_1 = y_0;
    im2=im;
}
void Ekran::mouseMoveEvent(QMouseEvent *e)
{
    x_0 = z_0;
    y_0 = z_1;
    x_1 = e->pos().x();
    y_1 = e->pos().y();
    im=im2;
    drawPixel(x_1, y_1);
    update();

    int dx = abs(x_1 - x_0);  //Roznica pixeli pomiedzy punktami X
    int dy = abs(y_1 - y_0);  //Roznica pixeli pomiedzy punktami Y

    int sx = (x_0 < x_1) ? 1 : -1; //Kierunki ruchu
    int sy = (y_0 < y_1) ? 1 : -1;

    int err = dx - dy;
    while(true)
    {
        drawPixel(x_0, y_0);
        if((x_0 == x_1) && (y_0 == y_1))
        {
            break;
        }
        int e2 = 2 * err;
        // obliczany jest błąd e2, który jest porównywany z wartościami dx i dy, aby określić, czy należy przesunąć linię wzdłuż osi x czy y.
        if(e2 > -dy) //przesuwa pixele w prawo/lewo
        {
            err -= dy;
            x_0 += sx;
        }
        if(e2 < dx) //to w gora/dol
        {
            err += dx;
            y_0 += sy;
        }
    }
    update();
}
void Ekran::mouseReleaseEvent(QMouseEvent *e)
{
    qDebug("2 punkt:\tx:%d Y:%d",e->pos().x(),e->pos().y());
}
