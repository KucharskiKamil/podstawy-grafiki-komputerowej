#include "ekran.h"

#include <QPainter>
float alfa=0;
int aktualnaWarstwa=0;
Ekran::warstwa warstwy[3];
Ekran::Ekran(QWidget *parent)
    : QWidget{parent}
{
    im=QImage(800,600,QImage::Format_RGB32);
    im.fill(0);
}
void Ekran::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.fillRect(0,0,width(),height(),Qt::white);
    p.drawImage(0,0,im);
    warstwy[0].im.load("C://Users//KamilK//Desktop//blending//odcinki//im1.jpg");
    warstwy[1].im.load("C://Users//KamilK//Desktop//blending//odcinki//im2.jpg");
    warstwy[2].im.load("C://Users//KamilK//Desktop//blending//odcinki//im3.jpg");
}
void Ekran::drawWarstwy(){
    for(int i = 2; i>=0; i--){
        mieszaj(im,warstwy[i].im,warstwy[i].alfa,warstwy[i].blendType);
    }
    update();
}
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
void Ekran::setAlfa(int a)
{
    alfa=(float)a/100;
    warstwy[aktualnaWarstwa].alfa=alfa;
    drawWarstwy();
}
void Ekran::setValue(int a, int b)
{
    if(a==1)
    {
        //Zmieniamy warstwe
        aktualnaWarstwa=b-1;
    }
    else
    {

        warstwy[aktualnaWarstwa].blendType=b;
        drawWarstwy();
    }
}
int darkenTryb(int a, int b)
{
    if(a<b)
    {
        return a;
    }
    else
    {
        return b;
    }
}
int lightenTryb(int a, int b)
{
    if(a>b)
    {
        return a;
    }
    else
    {
        return b;
    }
}
void Ekran::mieszaj(QImage bg, QImage fg, float alfa, int tryb)
{
    //QImage *res = new QImage(w,h,QImage::Format_RGB32);

    int r,g,b;
    for(int y = 0 ; y < im.height() ; y++){
        uchar *pix1 = bg.scanLine(y) ;
        uchar *pix2 = fg.scanLine(y);
        for (int x = 0; x < im.width(); x++) {
            if(tryb==1)//average
            {
                r = alfa * ((pix1[4*x + 2]+pix2[4*x+2])>>1) + (1.0 - alfa)*pix1[4*x + 2];
                g = alfa * ((pix1[4*x + 1]+pix2[4*x+1])>>1) + (1.0 - alfa)*pix1[4*x + 1];
                b = alfa * ((pix1[4*x]+pix2[4*x])>>1) + (1.0 - alfa)*pix1[4*x];
                drawPixel(x,y,r,g,b);
            }
            if(tryb==2)//multiply
            {
                r = alfa * ((pix2[4*x+2]*pix1[4*x+2])>>8) + (1.0 - alfa)*pix1[4*x + 2];
                g = alfa * ((pix2[4*x+1]*pix1[4*x+1])>>8) + (1.0 - alfa)*pix1[4*x + 1];
                b = alfa * ((pix2[4*x]*pix1[4*x])>>8) + (1.0 - alfa)*pix1[4*x];
                drawPixel(x,y,r,g,b);
            }
            if(tryb==3)//darken
            {
                r = alfa * darkenTryb(pix2[4*x+2],pix1[4*x+2]) + (1.0 - alfa)*pix1[4*x + 2];
                g = alfa * darkenTryb(pix2[4*x+1],pix1[4*x+1]) + (1.0 - alfa)*pix1[4*x + 1];
                b = alfa * darkenTryb(pix2[4*x],pix1[4*x]) + (1.0 - alfa)*pix1[4*x];
                drawPixel(x,y,r,g,b);
            }
            if(tryb==4)//lighten
            {
                r = alfa * lightenTryb(pix2[4*x+2],pix1[4*x+2]) + (1.0 - alfa)*pix1[4*x + 2];
                g = alfa * lightenTryb(pix2[4*x+1],pix1[4*x+1]) + (1.0 - alfa)*pix1[4*x + 1];
                b = alfa * lightenTryb(pix2[4*x],pix1[4*x]) + (1.0 - alfa)*pix1[4*x];
                drawPixel(x,y,r,g,b);
            }
            if(tryb==5)//screen
            {
                r = alfa * (255 - ((255-pix2[4*x+2]) * (255-pix1[4*x+2])>>8)) + (1.0 - alfa)*pix1[4*x + 2];
                g = alfa *(255 - ((255-pix2[4*x+1]) * (255-pix1[4*x+1])>>8)) + (1.0 - alfa)*pix1[4*x + 1];
                b = alfa * (255 - ((255-pix2[4*x]) * (255-pix1[4*x])>>8)) + (1.0 - alfa)*pix1[4*x];
                drawPixel(x,y,r,g,b);
            }
        }
    }
}


