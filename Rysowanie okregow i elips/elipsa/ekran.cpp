#include "ekran.h"
#include<qpainter.h>
int IloscKropek=3;
Ekran::Ekran(QWidget *parent)
    : QWidget{parent}
{
    im = QImage(851, 601, QImage::Format_RGB32);
    im.fill(0);
}
void Ekran::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.fillRect(0,0,width(),height(),Qt::white);
    p.drawImage(0,0,im);
}
void Ekran::drawPixel(int x, int y)
{
    if(x<0||y<0||x>=im.width() || y>=im.height())
    {
        return;
    }
    else
    {
        uchar *pix = im.scanLine(y);
        pix[4*x]=0;
        pix[4*x+1]=0;
        pix[4*x+2]=255;
        update();
    }
}
void Ekran::drawLine(int x_0,int y_0,int x_1, int y_1)
{

    int dx = abs(x_1 - x_0);
    int dy = abs(y_1 - y_0);
    int sx = (x_0 < x_1) ? 1 : -1;
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
        if(e2 > -dy)
        {
            err -= dy;
            x_0 += sx;
        }
        if(e2 < dx)
        {
            err += dx;
            y_0 += sy;
        }
    }
    update();
}

void Ekran::wartosc(int n)
{
    qDebug("Nowa wartosc zmiennej IloscKropek %d", n);
    IloscKropek=n;
}
int x_0,y_0;
void Ekran::mousePressEvent(QMouseEvent *e)
{
    im2=im;
    x_0=e->pos().x();
    y_0=e->pos().y();
}
void Ekran::mouseMoveEvent(QMouseEvent *e)
{
    im=im2;
    int x_1=e->pos().x();
    int y_1=e->pos().y();
    int dx = abs(x_1 - x_0);
    int dy = abs(y_1 - y_0);
    int y,x;
    double alfa;
    int pomX;
    int pomY;
    // Rysowanie linii między kolejnymi wierzchołkami
    for (int i=0;i<=IloscKropek;i++)
    {
        alfa = (i*2*M_PI)/IloscKropek;//obliczenie kąta miedzy punktami
        x = x_0+dx*cos(alfa); //ruszanie pierwszej przekatnej wzgledem x
        y = y_0+dy*sin(alfa); //ruszanie drugiej przekatnej wzgledem y
        if(i!=0)
        {
            drawLine(x, y, pomX, pomY);
        }
        pomX = x;
        pomY = y;
    }
    update();
}

