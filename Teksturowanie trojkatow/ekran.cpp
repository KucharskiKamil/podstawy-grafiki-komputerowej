#include "ekran.h"
#include <QPainter>
#include <QMouseEvent>
int klikniecia=0;
int wyborPolowki=0;
int wyborPunkty=0;
int imageWidth=0;
int imageHeight=0;
Ekran::Ekran(QWidget *parent) : QWidget{parent}
{
    im = QImage(900, 600, QImage::Format_RGB32);
    im2 = QImage(900, 600, QImage::Format_RGB32);
    im.fill(0);
    im2.load("C://Users//KamilK//Desktop//pgk do oddania robienie//trojkaty2//teksturowanie//test2.jpg");

}
void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0, 0, im);
    p.drawImage(450, 0, im2);
}
void Ekran::drawPixel(int x, int y, int B, int G, int R)
{
    int width = im.width();
    int height = im.height();
    if ((x >= 0) && (x <= width) && (y >= 0) && (y <= height))
    {
        if ((x >= 0) && (x <= 450) && (y >= 0) && (y <= height))
        {
            uchar *pix = im.scanLine(y); // dostęp do piksela
            pix[4 * x] = B;              // blue
            pix[4 * x + 1] = G;          // green
            pix[4 * x + 2] = R;          // red
        }
        else
        {
            uchar *pix2 = im2.scanLine(y); // dostęp do piksela
            pix2[4 * x] = B;               // blue
            pix2[4 * x + 1] = G;           // green
            pix2[4 * x + 2] = R;           // red
        }
    }
}
void Ekran::drawLine(int x0, int y0, int x1, int y1)
{
    double m = 0;
    if (x1 == x0 && y1 == y0)
    {
        drawPixel(x1, y1, 255, 255, 255);
    }
    else
    {
        if (abs(x1 - x0) >= abs(y1 - y0))
        {
            if (x0 > x1)
            {
                std::swap(y0, y1);
                std::swap(x0, x1);
            }
            m = (double)(y1 - y0) / (x1 - x0);
            for (int x = x0; x <= x1; x++)
            {
                int y = m * (x - x0) + y0;
                drawPixel(x, (int)floor(y + 0.5), 255, 255, 255);
            }
        }
        else
        {
            if (y0 > y1)
            {
                std::swap(x0, x1);
                std::swap(y0, y1);
            }
            m = (double)(x1 - x0) / (y1 - y0);
            for (int y = y0; y <= y1; y++)
            {
                int x = m * (y - y0) + x0;
                drawPixel((int)floor(x + 0.5), y, 255, 255, 255);
            }
        }
    }
}
QRgb Ekran::interpolacja(QImage* im, double x, double y)
{
    if (x >= 0 && x <= 450 && y >= 0 && y <= 450)
    {
        int x0 = static_cast<int>(x);
        int x1 = x0 + 1;
        int y0 = static_cast<int>(y);
        int y1 = y0 + 1;

        double dx = x - x0;
        double dy = y - y0;

        unsigned char* ptr = im->bits();


        int red = (1 - dx) * (1 - dy) * ptr[4 * (x0 + 450 * y0) + 2] + dx * (1 - dy) * ptr[4 * (x1 + 450 * y0) + 2] + (1 - dx) * dy * ptr[4 * (x0 + 450 * y1) + 2] + dx * dy * ptr[4 * (x1 + 450 * y1) + 2];
        int green = (1 - dx) * (1 - dy) * ptr[4 * (x0 + 450 * y0) + 1] + dx * (1 - dy) * ptr[4 * (x1 + 450 * y0) + 1] + (1 - dx) * dy * ptr[4 * (x0 + 450 * y1) + 1] + dx * dy * ptr[4 * (x1 + 450 * y1) + 1];
        int blue = (1 - dx) * (1 - dy) * ptr[4 * (x0 + 450 * y0) + 0] + dx * (1 - dy) * ptr[4 * (x1 + 450 * y0) + 0] + (1 - dx) * dy * ptr[4 * (x0 + 450 * y1) + 0] + dx * dy * ptr[4 * (x1 + 450 * y1) + 0];
        return qRgb(red, green, blue);
    }
    else
    {
        return qRgb(0, 0, 0); // kolor tła
    }
}
void Ekran::dzialaj()
{

    int minY = punkty[0].y();
    int maxY = punkty[0].y();
    int minX = punkty[0].x();
    int maxX = punkty[0].x();
    for (int i = 1; i < punkty.size(); i++)
    {
        if (punkty[i].y() < minY)
        {
            minY = punkty[i].y();
        }
        if (punkty[i].y() > maxY)
        {
            maxY = punkty[i].y();
        }
        if (punkty[i].x() < minX)
        {
            minX = punkty[i].x();
        }
        if (punkty[i].x() > maxX)
        {
            maxX = punkty[i].x();
        }
    }



    for (int y = minY; y < maxY; y++)
    {
        for (int x = minX; x < maxX; x++)
        {
            double w = ((punkty[1].y() - punkty[2].y()) * (x - punkty[2].x()) + (punkty[2].x() - punkty[1].x()) * (y - punkty[2].y())) / ((punkty[0].y() - punkty[1].y()) * (punkty[2].x() - punkty[1].x()) + (punkty[1].x() - punkty[0].x()) * (punkty[2].y() - punkty[1].y()));
            double v = ((punkty[2].y() - punkty[0].y()) * (x - punkty[2].x()) + (punkty[0].x() - punkty[2].x()) * (y - punkty[2].y())) / ((punkty[0].y() - punkty[1].y()) * (punkty[2].x() - punkty[1].x()) + (punkty[1].x() - punkty[0].x()) * (punkty[2].y() - punkty[1].y()));
            double u = 1.0 - w - v;

            if ((u > 0) && (u < 1) && (v > 0) && (v < 1) && (w > 0) && (w < 1))
            {
                double Ppx = u * punkty2[2].x() + v * punkty2[1].x() + w * punkty2[0].x();
                double Ppy = u * punkty2[2].y() + v * punkty2[1].y() + w * punkty2[0].y();
                if (interpolacjaValue)
                {
                    color = interpolacja(&im2, Ppx - 450, Ppy);
                }
                else
                {
                    color = im2.pixelColor(Ppx - 450, Ppy);
                }
                drawPixel(x, y, color.blue(), color.green(), color.red());
            }
        }
    }
    update();
}
void Ekran::dzialaj2()
{
    im.fill(0);
    im2.load("C://Users//KamilK//Desktop//pgk do oddania robienie//trojkaty2//teksturowanie//test2.jpg");
    dzialaj();
    for(int i=0;i<2;i++)
    {
        drawLine(punkty[i].x(),punkty[i].y(),punkty[i+1].x(),punkty[i+1].y());
        drawLine(punkty2[i].x(),punkty2[i].y(),punkty2[i+1].x(),punkty2[i+1].y());

    }
    drawLine(punkty[0].x(),punkty[0].y(),punkty[2].x(),punkty[2].y());
    drawLine(punkty2[0].x(),punkty2[0].y(),punkty2[2].x(),punkty2[2].y());
    for(int i=0;i<2;i++)
    {
        for (int j = -2; j <= 2; j++)
        {
            for (int k = -2; k <= 2; k++)
            {
                drawPixel(punkty[i].x() + j, punkty[i].y() + k, 0, 0, 255);
            }
        }
        for (int j = -2; j <= 2; j++)
        {
            for (int k = -2; k <= 2; k++)
            {
                drawPixel(punkty2[i].x() + j, punkty2[i].y() + k, 0, 0, 255);
            }
        }
    }
    for (int j = -2; j <= 2; j++)
    {
        for (int k = -2; k <= 2; k++)
        {
            drawPixel(punkty[2].x() + j, punkty[2].y() + k, 0, 0, 255);
            drawPixel(punkty2[2].x() + j, punkty2[2].y() + k, 0, 0, 255);
        }
    }
    update();
}
void Ekran::interChange()
{
    if(interpolacjaValue)
    {
        interpolacjaValue = false;
        qDebug("Wyłączono interpolację");
        if(klikniecia>=6)
        {
            dzialaj2();
        }
    }
    else
    {
        interpolacjaValue = true;
        qDebug("Włączono interpolację");
        if(klikniecia>=6)
        {
            dzialaj2();
        }
    }
}
void Ekran::rysujPunkty()
{
    for (int i = 0; i < punkty.size(); i++)
    {
        drawPixel(punkty[i].x(), punkty[i].y(), 0, 0, 255);
    }
    for (int i = 0; i < punkty2.size(); i++)
    {
        drawPixel(punkty2[i].x(), punkty2[i].y(), 0, 0, 255);
    }
}
void Ekran::czysc()
{
    klikniecia=0;
    im.fill(0);
    im2.load("C://Users//KamilK//Desktop//pgk do oddania robienie//trojkaty2//teksturowanie//test2.jpg");
    punkty.clear();
    punkty2.clear();
    update();
}
void Ekran::mousePressEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
    {
        // Pobieramy wspolrzedne punktu klikniecia
        // Sa to wspolrzedne wzgledem glownego okna
        int x = e->pos().x();
        int y = e->pos().y();
        if(klikniecia>5)
        {
            qDebug("Juz dodano punkty!");
            return;
        }
        if (x < ((im.width()) / 2))
        {
            if(klikniecia<3)
            {
                punkty.push_back(QPointF(x, y));
                klikniecia++;
            }
        }
        else
        {
            if(klikniecia>2)
            {
                punkty2.push_back(QPointF(x, y));

                klikniecia++;
            }
        }
        rysujPunkty();
        if(klikniecia==6)
        {
            dzialaj2();
        }
        update();
    }else if(e->buttons() & Qt::MiddleButton)// Wybieranie najblizszego punktu
    {
        int x=e->pos().x();
        int y=e->pos().y();
        if(klikniecia<5)
        {
            return;
        }
        if(x<(im.width()/2))// Po lewej
        {
            float najmOdlegosc=999999999.9;
            for(int i=0;i<3;i++)
            {
                float odleglosc=qSqrt(pow(punkty[i].x()-x,2)+pow(punkty[i].y()-y,2));
                if(odleglosc<najmOdlegosc)
                {
                    najmOdlegosc=odleglosc;
                    wyborPolowki=1;
                    wyborPunkty=i;
                }
            }
            qDebug("Najblizszy punkt: (%f,%f) z polowki %d o id:%d",punkty[wyborPunkty].x(),punkty[wyborPunkty].y(),wyborPolowki,wyborPunkty);
        }
        else
        {
            float najmOdlegosc=999999999.9;
            for(int i=0;i<3;i++)
            {
                float odleglosc=qSqrt(pow(punkty2[i].x()-x,2)+pow(punkty2[i].y()-y,2));
                if(odleglosc<najmOdlegosc)
                {
                    najmOdlegosc=odleglosc;
                    wyborPolowki=1;
                    wyborPunkty=i;
                }
            }
            qDebug("Najblizszy punkt: (%f,%f) z polowki %d o id:%d",punkty2[wyborPunkty].x(),punkty2[wyborPunkty].y(),wyborPolowki,wyborPunkty);
        }

    }
}
void Ekran::mouseMoveEvent(QMouseEvent *e)
{
    // Pobieramy wspolrzedne punktu klikniecia
    // Sa to wspolrzedne wzgledem glownego okna
    if(e->buttons() & Qt::MiddleButton)
    {
        int x = e->pos().x();
        int y = e->pos().y();
        if (x < (im.width()) / 2)
        {
            punkty[wyborPunkty].setX(x);
            punkty[wyborPunkty].setY(y);
            if (klikniecia>5)
                dzialaj2();
            for (int i = 0; i < punkty.size(); i++)
                drawPixel(punkty[i].x(), punkty[i].y(), 0, 0, 255);
        }
        else
        {

            punkty2[wyborPunkty].setX(x);
            punkty2[wyborPunkty].setY(y);
            if (klikniecia>5)
                dzialaj2();
            for (int i = 0; i < punkty2.size(); i++)
                drawPixel(punkty2[i].x(), punkty2[i].y(), 0, 0, 255);
        }
        // odświeżanie okna
        update();
    }

}
void Ekran::mouseReleaseEvent(QMouseEvent *)
{
}
