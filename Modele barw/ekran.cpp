#include "ekran.h"
#include <QPainter>
int red=0;
int blue=0;
int green=0;
int hue=0;
int saturation=0;
int value=0;
int flaga=0;
Ekran::Ekran(QWidget *parent)
    : QWidget{parent}
{
    //To maluje tylko maly kwadrat na czarno
    im=QImage(780,550,QImage::Format_RGB32);
    im.fill(0);
}

//          Ta funkcja maluje caly ekran na bialo
void Ekran::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.fillRect(0,0,width(),height(),Qt::white);
    p.drawImage(0,0,im);
}

//          Lepsza funkcja rysujaca pixel
void Ekran::drawPixel(int x, int y,int r, int g, int b)
{
    if (x < 0 || y < 0 || x >= im.width() || y >= im.height()) {
        return;
    }
    uchar *pix = im.scanLine(y);
    pix[4*x] = b;
    pix[4*x+1] = g;
    pix[4*x+2] = r;
}

void Ekran::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    qDebug("R:%d\tG:%d\tB%d\tH:%d\tS:%d\tV:%d",red,green,blue,hue,saturation,value);
}
void Ekran::dzialanie()
{
    int x,y;
    if(flaga==1)
    {
        for (int g = 0; g < im.height(); g++)
        {
            y=g;
            int greenn = (g * 255) / (im.height() - 1);
            for (int b = 0; b < im.width(); b++)
            {
                x=b;
                int bluee = (b * 255) / (im.width() - 1);
                drawPixel(x, y, red, greenn, bluee);
            }
        }
        flaga=0;
    }
    if(flaga==2)
    {
        for (int r=0; r<im.height();r++)
        {
            y=r;
            int redd = (r * 255) / (im.height() - 1);
            for (int b=0;b<im.width();b++)
            {
                x=b;
                int bluee = (b * 255) / (im.width() - 1);
                drawPixel(x, y, redd, green, bluee);
            }
        }
        flaga=0;
    }
    if(flaga==3)
    {
        for (int r = 0; r < im.height(); r++)
        {
            y = r;
            int redd = (r * 255) / (im.height() - 1);
            for (int g = 0; g < im.width(); g++)
            {
                x = g;
                int greenn = (g * 255) / (im.width() - 1);
                drawPixel(x, y, redd, greenn, blue);
            }
        }
        flaga = 0;
    }
    if(flaga==4)
    {
        float S,V,C,X,H_prim;
        float w = im.width() - 1;
        float h = im.height() - 1;
        for (int y=0; y<im.height(); y++) {
            for (int x=0; x<im.width(); x++) {
                S = (float)x / w;
                V = (float)y / h;
                C = S * V;
                H_prim = hue / 60.0f;
                X = C * (1 - fabs(fmod(H_prim, 2.0f) - 1));
                float r, g, b;
                if(H_prim >= 0 && H_prim <= 1) {
                    r = V;
                    g = X + V - C;
                    b = V - C;
                } else if(H_prim >= 1 && H_prim <= 2) {
                    r = X + V - C;
                    g = V;
                    b = V - C;
                } else if(H_prim >= 2 && H_prim <= 3) {
                    r = V - C;
                    g = V;
                    b = X + V - C;
                } else if(H_prim >= 3 && H_prim <= 4) {
                    r = V - C;
                    g = X + V - C;
                    b = V;
                } else if(H_prim >= 4 && H_prim <= 5) {
                    r = X + V - C;
                    g = V - C;
                    b = V;
                } else {
                    r = V;
                    g = V - C;
                    b = X + V - C;
                }
                drawPixel(x, y, r * 255, g * 255, b * 255);
            }
        }
        flaga=0;
    }
    if(flaga==5)
    {
        float S,V,C,X,H_prim;
        float w = im.width() - 1;
        float h = im.height() - 1;
        for (int y=0; y<im.height(); y++) {
            for (int x=0; x<im.width(); x++) {
                S = (float)saturation / 255;
                V = (float)y / h;
                C = S * V;
                H_prim = ((float)x / w) * 6;
                X = C * (1 - fabs(fmod(H_prim, 2.0f) - 1));
                float r, g, b;
                if(H_prim >= 0 && H_prim <= 1) {
                    r = V;
                    g = X + V - C;
                    b = V - C;
                } else if(H_prim >= 1 && H_prim <= 2) {
                    r = X + V - C;
                    g = V;
                    b = V - C;
                } else if(H_prim >= 2 && H_prim <= 3) {
                    r = V - C;
                    g = V;
                    b = X + V - C;
                } else if(H_prim >= 3 && H_prim <= 4) {
                    r = V - C;
                    g = X + V - C;
                    b = V;
                } else if(H_prim >= 4 && H_prim <= 5) {
                    r = X + V - C;
                    g = V - C;
                    b = V;
                } else {
                    r = V;
                    g = V - C;
                    b = X + V - C;
                }
                drawPixel(x, y, r * 255, g * 255, b * 255);
            }
        }
        flaga=0;
    }
    if(flaga==6)
    {
        float S,V,C,X,H_prim;
        float w = im.width() - 1;
        float h = im.height() - 1;
        for (int y=0; y<im.height(); y++) {
            for (int x=0; x<im.width(); x++) {
                S = (float)y / w;
                V = (float)value / 255;
                C = S * V;
                H_prim = ((float)x / w) * 6;
                X = C * (1 - fabs(fmod(H_prim, 2.0f) - 1));
                float r, g, b;
                if(H_prim >= 0 && H_prim <= 1) {
                    r = V;
                    g = X + V - C;
                    b = V - C;
                } else if(H_prim >= 1 && H_prim <= 2) {
                    r = X + V - C;
                    g = V;
                    b = V - C;
                } else if(H_prim >= 2 && H_prim <= 3) {
                    r = V - C;
                    g = V;
                    b = X + V - C;
                } else if(H_prim >= 3 && H_prim <= 4) {
                    r = V - C;
                    g = X + V - C;
                    b = V;
                } else if(H_prim >= 4 && H_prim <= 5) {
                    r = X + V - C;
                    g = V - C;
                    b = V;
                } else {
                    r = V;
                    g = V - C;
                    b = X + V - C;
                }
                drawPixel(x, y, r * 255, g * 255, b * 255);
            }
        }
        flaga=0;
    }
}
void Ekran::changeValue(int a, int b)
{
    if(b==1)
    {
        red=a;
        flaga=1;
    }
    if(b==2)
    {
        green=a;
        flaga=2;
    }
    if(b==3)
    {
        blue=a;
        flaga=3;
    }
    if(b==4)
    {
        hue=a;
        flaga=4;
    }
    if(b==5)
    {
        saturation=a;
        flaga=5;
    }
    if(b==6)
    {
        value=a;
        flaga=6;
    }
    dzialanie();
    update();
}
