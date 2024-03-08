#include "ekran.h"
#include<qpainter.h>
using namespace std;
vector<vector<int>> punkty;
int rows = 0;
float przesunOdleglosc=9999999.9;
int przesunID=0;
bool przesuwam=false;
Ekran::Ekran(QWidget *parent)
    : QWidget{parent}
{
    im = QImage(851, 601, QImage::Format_RGB32);
    im.fill(0);
}
void Ekran::rysujPixele()
{
    for(int i=0;i<punkty.size();i++)
    {
        drawPixel(punkty[i][0],punkty[i][1]);
    }
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
}
void Ekran::removePixel(int x, int y)
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
        pix[4*x+2]=0;

    }
}
void Ekran::bezier(vector<vector<int>> vect, int k)
{
    if(vect.size()>3)
    {
        int poprzedniX, poprzedniY,punktX,punktY;
        for(int i=0;i<vect.size()-3;i+=3)
        {
            for(int j=0;j<=k;j++)
            {
                float t = (float)j/(float)k;
                punktX=qPow(1-t,3) * vect[i][0] + qPow(1-t,2)*3*t*vect[i+1][0] + (1-t)*3*t*t*vect[i+2][0] + t*t*t * vect[i+3][0];
                punktY=qPow(1-t,3) * vect[i][1] + qPow(1-t,2)*3*t*vect[i+1][1] + (1-t)*3*t*t*vect[i+2][1] + t*t*t * vect[i+3][1];
                if(j==0)
                {
                    poprzedniX=punktX;
                    poprzedniY=punktY;
                }
                else
                {
                    drawLine(poprzedniX,poprzedniY,punktX,punktY);
                    poprzedniX=punktX;
                    poprzedniY=punktY;

                }
            }
        }
    }
}

void Ekran::wypiszPunkty(vector<vector<int>> vect) //Funkcja wypisujaca aktualne punkty w naszym vektorze
{
    if(vect.size()==0)
    {
        qDebug("Brak punktow!");
        return;
    }
    for (uint i = 0; i < vect.size(); i++)
    {
        qDebug("Punkt %u\t x:%d y:%d", vect[i][2], vect[i][0], vect[i][1]);
    }
    qDebug("");
}

void Ekran::mousePressEvent(QMouseEvent *e)
{
    //im2=im;
    if (e->button() == Qt::RightButton) //Jesli wcisniety prawy przycisk myszki
    {
        if(rows==0) //Upewniam sie, ze sa juz jakies punkty na mapie
        {
            qDebug("Brak punktow ktore mozna skasowac!");
            return;
        }
        else
        {
            float najm=99999999.9;
            int idNajm=0;
            for (uint i = 0; i < punkty.size(); i++) //Obliczam ktory punkt ma najblizej do wcisnietego przeze mnie miejsca
            {
                float odleglosc=((punkty[i][0] - e->pos().x())*(punkty[i][0] - e->pos().x()) + (punkty[i][1] - e->pos().y())*(punkty[i][1] - e->pos().y()));
                if(odleglosc<najm)
                {
                    najm=odleglosc;
                    idNajm=punkty[i][2];
                }
            }
            im.fill(0);
            update();
            qDebug("Usuwam punkt %u\t x:%d y:%d", punkty[idNajm][2], punkty[idNajm][0], punkty[idNajm][1]); //Usuwam punkt
            removePixel(punkty[idNajm][0], punkty[idNajm][1]); //Zamalowywuje go z powrotem na czarno
            update();

            for(int i=idNajm;i<punkty.size()-1;i++) //Dla tablicy punktow: kazdy z elementow po wybranym przez nas punkcie jest przesuwany w lewo o 1 a jego id zmniejszany (zeby zachowac ladna kolejnosc)
            {
                punkty[i][0]=punkty[i+1][0];
                punkty[i][1]=punkty[i+1][1];
                punkty[i][2]=punkty[i+1][2];
                punkty[i][2]--;
            }
            rows--;
            punkty.resize(rows); //Zmniejszam ilosc wierszy i rozmiast vektora 'punkty' przez co usuwa nam sie ostatni zbedny wiersz ktory jest kopia
            wypiszPunkty(punkty); //Wypisuje nowa lista punktow po usunieciu poprzednika

            bezier(punkty,10);

            rysujPixele();
            update();
        }
    }
    if(e->button() == Qt::LeftButton)
    {
        if(e->pos().x() < 0 || e->pos().x() >= im.width() || e->pos().y()<0 || e->pos().y()>= im.height()) //Jesli jest w naszym miejscu do rysowania
        {
            return;
        }
        else
        {   //Zwiekszamy nasz vektor z punktami o 1 kolumne z 3 miejscami, do [0] wpisujemy X, [1] Y, [2] ID miejsca, a potem wszystko rysujemy
            punkty.resize(rows + 1);
            punkty[rows].resize(3);
            punkty[rows][0] = e->pos().x();
            punkty[rows][1] = e->pos().y();
            punkty[rows][2] = rows;
            rows++;
            wypiszPunkty(punkty);
            drawPixel(e->pos().x(),e->pos().y());
            update();
        }

        bezier(punkty,10);

        rysujPixele();
        update();

    }
    if(e->button() == Qt::MiddleButton) //Przesuwanie pixela
    {
        qDebug("Rozpoczynam przesuwanie pixela...");
        przesuwam = true;
        for (uint i = 0; i < punkty.size(); i++) //Obliczam ktory punkt ma najblizej do wcisnietego przeze mnie miejsca
        {
            float odleglosc=((punkty[i][0] - e->pos().x())*(punkty[i][0] - e->pos().x()) + (punkty[i][1] - e->pos().y())*(punkty[i][1] - e->pos().y()));
            if(odleglosc<przesunOdleglosc)
            {
                przesunOdleglosc=odleglosc;
                przesunID=punkty[i][2];
            }
        }
        removePixel(punkty[przesunID][0],punkty[przesunID][1]);
        update();
        qDebug("Najblizszy punkt %u\t x:%d y:%d o odleglosci %f", punkty[przesunID][2], punkty[przesunID][0], punkty[przesunID][1],przesunOdleglosc); //Usuwam punkt
    }
}
void Ekran::mouseMoveEvent(QMouseEvent *e)
{
    if(przesuwam) //Przesuwanie pixela
    {
        //im=im2;
        im.fill(0);
        removePixel(punkty[przesunID][0],punkty[przesunID][1]);
        punkty[przesunID][0] = e->pos().x();
        punkty[przesunID][1] = e->pos().y();
        qDebug("Przesuwam punkt %d na x:%d y:%d",punkty[przesunID][2],punkty[przesunID][0],punkty[przesunID][1]);
        drawPixel(punkty[przesunID][0],punkty[przesunID][1]);

        bezier(punkty,10);

        rysujPixele();
        update();
    }
}
void Ekran::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::MiddleButton) //Przesuwanie pixela
    {
        punkty[przesunID][0] = e->pos().x();
        punkty[przesunID][1] = e->pos().y();
        drawPixel(punkty[przesunID][0],punkty[przesunID][1]);
        update();
        przesuwam = false;
        przesunOdleglosc=9999999.9;
        przesunID=0;
        rysujPixele();
        update();
    }
    wypiszPunkty(punkty);
}

