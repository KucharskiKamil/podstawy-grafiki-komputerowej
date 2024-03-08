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
    if(x<0 || x>=im.width() || y<0 || y>=im.height())
        return;
    uchar *pix = im.scanLine(y);
    pix[4*x]=150;
    pix[4*x+1]=20;
    pix[4*x+2]=255;
}

int centerX=0;
int centerY=0;
int secondX=0;
int secondY=0;
float radius=0;
void Ekran::mousePressEvent(QMouseEvent *e)
{
    qDebug("x:%d\ty:%d ", e->pos().x(), e->pos().y());
    centerX = e->pos().x();
    centerY = e->pos().y();
    im2=im;
}

void Ekran::mouseMoveEvent(QMouseEvent *e)
{

    secondX=e->pos().x();
    secondY=e->pos().y();
    int deltaX = centerX - secondX;
    int deltaY = centerY - secondY;
    radius = std::sqrt((float)(deltaX * deltaX) + deltaY * deltaY);

    int x = 0;
    int y = radius;
    qDebug("%d",y);
    int d = 3 - 2 * radius;
    im=im2;
    while (y >= x)
    {
        drawPixel(centerX + x, centerY + y);
        drawPixel(centerX + y, centerY + x);
        drawPixel(centerX - y, centerY + x);
        drawPixel(centerX - x, centerY + y);
        drawPixel(centerX - x, centerY - y);
        drawPixel(centerX - y, centerY - x);
        drawPixel(centerX + y, centerY - x);
        drawPixel(centerX + x, centerY - y);
        if (d < 0)
        {
            d += 2 * x + 3;//d(new)=2*xp +3
        }
        else
        {
            d += 2 * (x - y) + 5; //d(new)=2xp-2yp+5
            y--;
        }
        x++;
    }
    update();
}
