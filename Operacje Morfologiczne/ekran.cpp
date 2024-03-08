#include "ekran.h"

#include <QPainter>
int zmienna=1;
Ekran::Ekran(QWidget *parent)
    : QWidget{parent}
{
    //To maluje tylko maly kwadrat na czarno
    im=QImage(800,800,QImage::Format_RGB32);
    im.load("C:\\Users\\KamilK\\Desktop\\erozja\\image.png");
    im2=QImage(800,800,QImage::Format_RGB32);
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


}
void Ekran::mouseMoveEvent(QMouseEvent *e)
{

}
void Ekran::mouseReleaseEvent(QMouseEvent *e)
{

}
void Ekran::czarnebiale()
{
    for (int y = 0; y < im.height(); y++)
    {
        QRgb* pixelRow = reinterpret_cast<QRgb*>(im.scanLine(y));
        for (int x = 0; x < im.width(); x++)
        {
            QRgb pixel = pixelRow[x];
            int red = qRed(pixel);
            int green = qGreen(pixel);
            int blue = qBlue(pixel);
            if(red>150 || green>150 || blue>150)
            {
                drawPixel(x,y,255,255,255);
            }
            else
            {
                drawPixel(x,y,0,0,0);
            }
        }
    }
    update();
}
void Ekran::changeZmienna(int a)
{
    zmienna=a;
}
void Ekran::dylatacja()
{
    for (int y = 0; y < im.height(); y++)
    {
        for (int x = 0; x < im.width(); x++)
        {
            // Sprawdź kolor piksela
            QRgb pixel = im.pixel(x, y);
            int red = qRed(pixel);
            int green = qGreen(pixel);
            int blue = qBlue(pixel);

            // Jeśli piksel jest czarny
            if (red == 0 && green == 0 && blue == 0)
            {
                // Sprawdź piksele wokół aktualnego piksela
                for (int i = -zmienna; i <= zmienna; i++)
                {
                    for (int j = -zmienna; j <= zmienna; j++)
                    {
                        // Sprawdź czy piksel jest w granicach obrazu
                        if (x + i >= 0 && y + j >= 0 && x + i < im.width() && y + j < im.height())
                        {
                            // Sprawdź kolor piksela wokół
                            QRgb neighborPixel = im.pixel(x + i, y + j);
                            int neighborRed = qRed(neighborPixel);
                            int neighborGreen = qGreen(neighborPixel);
                            int neighborBlue = qBlue(neighborPixel);

                            // Jeśli piksel sąsiedni jest biały, zamaluj go na czerwono
                            if (neighborRed == 255 && neighborGreen == 255 && neighborBlue == 255)
                            {
                                drawPixel(x + i, y + j, 255, 0, 0);
                            }
                        }
                    }
                }
            }
        }
    }
    for (int y = 0; y < im.height(); y++)
    {
        for (int x = 0; x < im.width(); x++)
        {
            // Sprawdź kolor piksela
            QRgb pixel = im.pixel(x, y);
            int red = qRed(pixel);
            int green = qGreen(pixel);
            int blue = qBlue(pixel);

            // Jeśli piksel jest czarny
            if (red == 255 && green == 0 && blue == 0)
            {

                drawPixel(x,y,0,0,0);
            }
        }
    }
    update();
}
void Ekran::erozja()
{
    for (int y = 0; y < im.height(); y++)
    {
        for (int x = 0; x < im.width(); x++)
        {
            // Sprawdź kolor piksela
            QRgb pixel = im.pixel(x, y);
            int red = qRed(pixel);
            int green = qGreen(pixel);
            int blue = qBlue(pixel);

            // Jeśli piksel jest czarny
            if (red == 255 && green == 255 && blue == 255)
            {
                // Sprawdź piksele wokół aktualnego piksela
                for (int i = -zmienna; i <= zmienna; i++)
                {
                    for (int j = -zmienna; j <= zmienna; j++)
                    {
                        // Sprawdź czy piksel jest w granicach obrazu
                        if (x + i >= 0 && y + j >= 0 && x + i < im.width() && y + j < im.height())
                        {
                            // Sprawdź kolor piksela wokół
                            QRgb neighborPixel = im.pixel(x + i, y + j);
                            int neighborRed = qRed(neighborPixel);
                            int neighborGreen = qGreen(neighborPixel);
                            int neighborBlue = qBlue(neighborPixel);

                            // Jeśli piksel sąsiedni jest biały, zamaluj go na czerwono
                            if (neighborRed == 0 && neighborGreen == 0 && neighborBlue == 0)
                            {
                                drawPixel(x + i, y + j, 255, 0, 0);
                            }
                        }
                    }
                }
            }
        }
    }
    for (int y = 0; y < im.height(); y++)
    {
        for (int x = 0; x < im.width(); x++)
        {
            // Sprawdź kolor piksela
            QRgb pixel = im.pixel(x, y);
            int red = qRed(pixel);
            int green = qGreen(pixel);
            int blue = qBlue(pixel);

            // Jeśli piksel jest czerwony
            if (red == 255 && green == 0 && blue == 0)
            {

                drawPixel(x,y,255,255,255);
            }
        }
    }
    update();
}
void Ekran::domkniecie()
{
    dylatacja();
    erozja();
}
void Ekran::otwarcie()
{
    erozja();
    dylatacja();
}
