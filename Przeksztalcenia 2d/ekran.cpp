#include "ekran.h"
#include <QPainter>
#include <QMouseEvent>
int imageHeight=0;
int imageWidth=0;
double Tx = 0.0;
double Ty = 0.0;
double alfa = 0.0;
double Sx = 1.0;
double Sy = 1.0;
double Shx = 0.0;
double Shy = 0.0;
Ekran::Ekran(QWidget *parent) : QWidget{parent}
{
    temp =  QImage(QString::fromStdString("C://Users//KamilK//Desktop//pgk do oddania robienie//2d//operacje2D//pug.jpg"));
    im2 =  QImage(QString::fromStdString("C://Users//KamilK//Desktop//pgk do oddania robienie//2d//operacje2D//pug.jpg")); //tylko obrazek
    im =  QImage(640, 640, QImage::Format_RGB32); //obraz po przeksztalceniu
    imageHeight=im2.height();
    imageWidth=im2.width();
    przeksztalcenia(Tx, Ty, alfa, Sx, Sy, Shx, Shy);
    update();
}
void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0, 0, im);
}
void Ekran::drawPixel(int x, int y, int B, int G, int R)
{
    int width = im.width();
    int height = im.height();
    if ((x>=0) && (x<=width) && (y>=0) && (y<=height))
    {
        uchar *pix=im.scanLine(y);
        pix[4*x]=B;
        pix[4*x+1]=G;
        pix[4*x+2]=R;
    }
}
QRgb Ekran::interpolacja(QImage* im, double x, double y)
{
    if ((x>=0) && (x<imageWidth) && (y>=0) && (y<imageHeight))
    {
        int x0=int(x);
        int x1=x0+1;
        int y0=int(y);
        int y1=y0+1;

        double dx=x-x0;
        double dy=y-y0;

        uchar* pix=im->bits();

        int red=(1-dx)*(1-dy)*pix[4*(x0+500*y0)+2]+dx*(1-dy)*pix[4*(x1+500*y0)+2]+(1-dx)*dy*pix[4*(x0+500*y1)+2]+dx*dy*pix[4*(x1+500*y1)+2];
        int green=(1-dx)*(1-dy)*pix[4*(x0+500*y0)+1]+dx*(1-dy)*pix[4*(x1+500*y0)+1]+(1-dx)*dy*pix[4*(x0+500*y1)+1]+dx*dy*pix[4*(x1+500*y1)+1];
        int blue=(1-dx)*(1-dy)*pix[4*(x0+500*y0)+0]+dx*(1-dy)*pix[4*(x1+500*y0)+0]+(1-dx)*dy*pix[4*(x0+500*y1)+0]+dx*dy*pix[4*(x1+500*y1)+0];
        return qRgb(red, green, blue);
    }
    else
    {
        return qRgb(0, 0, 0); // kolor tła
    }
}

void Ekran::przeksztalcenia(double x, double y, double alfa, double Sx, double Sy, double Shx, double Shy)
{
    QRgb color;
    im.fill(0); // Wypełnienie obrazu im kolorem czarnym

    // Przygotowanie macierzy transformacji T1, T2 i Txy
    double T2[3][3] = {{1, 0, -320}, {0, 1, -320}, {0, 0, 1}};
    double T1[3][3] = {{1, 0, 250}, {0, 1, 250}, {0, 0, 1}};
    double Txy[3][3] = {{1, 0, -x}, {0, 1, -y}, {0, 0, 1}};

    double a=alfa*(M_PI/180); // Obliczenie kąta alfa w radianach
    double R[3][3]={{cos(a), sin(a), 0}, {-sin(a), cos(a), 0}, {0, 0, 1}}; // Macierz rotacji

    double S[3][3]={{1.0/Sx, 0, 0}, {0, 1.0 / Sy, 0}, {0, 0, 1}}; // Macierz skalowania

    double Sh_x[3][3] = {{1, (-1)*Shx, 0}, {0, 1, 0}, {0, 0, 1}}; // Macierz pochylenia wzdłuż osi x
    double Sh_y[3][3] = {{1, 0, 0}, {(-1)*Shy, 1, 0}, {0, 0, 1}}; // Macierz pochylenia wzdłuż osi y

    double M[3][3]; // Macierz transformacji

    // Wykonanie operacji mnożenia macierzy M = T1 * R
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            M[i][j]=0;
            for(int k=0;k<3;k++)
            {
                M[i][j]+=T1[i][k]*R[k][j];
            }
        }
    }

    // Wykonanie operacji mnożenia macierzy M = M * S
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            double sum=0;
            for(int k=0;k<3;k++)
            {
                sum+=M[i][k]*S[k][j];
            }
            M[i][j]=sum;
        }
    }


    // Wykonanie operacji mnożenia macierzy M = M * Sh_y
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            double sum=0;
            for(int k=0;k<3;k++)
            {
                sum+=M[i][k]*Sh_y[k][j];
            }
            M[i][j]=sum;
        }
    }

    // Wykonanie operacji mnożenia macierzy M = M * Sh_x
    for (int i=0;i<3;i++)
    {
        for (int j=0;j<3;j++)
        {
            double sum=0;
            for (int k=0;k<3;k++)
            {
                sum+=M[i][k]*Sh_x[k][j];
            }
            M[i][j]=sum;
        }
    }

    // Wykonanie operacji mnożenia macierzy M = M * Txy
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            double sum=0;
            for(int k=0;k<3;k++)
            {
                sum+=M[i][k]*Txy[k][j];
            }
            M[i][j]=sum;
        }
    }

    // Wykonanie operacji mnożenia macierzy M = M * T2
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            double sum=0;
            for(int k=0;k<3;k++)
            {
                sum+=M[i][k]*T2[k][j];
            }
            M[i][j]=sum;
        }
    }

    // Przetwarzanie pikseli obrazu
    for(int i=0;i<im.height();i++)
    {
        for(int j=0;j<im.width();j++)
        {
            double P[3][1] = {{(double)i}, {(double)j}, {1}}; // Współrzędne piksela

            double Pprim[3][1]; // Przetworzone współrzędne piksela
            for(int i=0;i<3;i++)
            {
                for(int j=0;j<1;j++)
                {
                    Pprim[i][j] = 0;
                    for(int k=0;k<3;k++)
                    {
                        Pprim[i][j] += M[i][k] * P[k][j];
                    }
                }
            }
            if (Pprim[0][0] >= 0 && Pprim[0][0] < imageWidth - 1 && Pprim[1][0] >= 0 && Pprim[1][0] < imageHeight - 1)
            {
                color = interpolacja(&im2, Pprim[0][0], Pprim[1][0]); // Wywołanie funkcji interpolacji na obrazie im2 dla przetworzonych współrzędnych
            }
            else
            {
                color = qRgb(0, 0, 0); // Ustawienie koloru czarnego dla pikseli poza zakresem obrazu
            }
            drawPixel(i, j, qBlue(color), qGreen(color), qRed(color)); // Rysowanie piksela na ekranie

        }
    }
    update(); // Aktualizacja wyświetlanego obrazu na ekranie
}

void Ekran::setRotacja(int value)
{
    alfa = value;
    przeksztalcenia(Tx, Ty, value, Sx, Sy, Shx, Shy);
}
void Ekran::setLeftRight(int value)
{
    Tx = (-1)*value;
    przeksztalcenia(-value, Ty, alfa, Sx, Sy, Shx, Shy);
}
void Ekran::setTopDown(int value)
{
    Ty = (-1)*value;
    przeksztalcenia(Tx, -value, alfa, Sx, Sy, Shx, Shy);
}
void Ekran::setLeftRightSkalowanie(int value)
{
    Sx = (float)value/100;
    przeksztalcenia(Tx, Ty, alfa, Sx, Sy, Shx, Shy);
}
void Ekran::setTopDownSkalowanie(int value)
{
    Sy = (float)value/100;
    przeksztalcenia(Tx, Ty, alfa, Sx, Sy, Shx, Shy);
}
void Ekran::setLeftRightPochylenie(int value)
{
    Shx = (float)value/100;
    przeksztalcenia(Tx, Ty, alfa, Sx, Sy, Shx, Shy);
}
void Ekran::setTopDownPochylenie(int value)
{
    Shy = (float)value/100;
    przeksztalcenia(Tx, Ty, alfa, Sx, Sy, Shx, Shy);
}
void Ekran::reset()
{
    //Ustawiamy wszystko na stan poczatkowy
    im.fill(0);
    Tx = 0.0;
    Ty = 0.0;
    alfa = 0.0;
    Sx = 1.0;
    Sy = 1.0;
    Shx = 0.0;
    Shy = 0.0;
    przeksztalcenia(Tx, Ty, alfa, Sx, Sy, Shx, Shy);
}
