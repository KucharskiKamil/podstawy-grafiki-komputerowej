#include "ekran.h"

#include <QMouseEvent>
#include <QPainter>

Ekran::Ekran(QWidget *parent) :QWidget(parent){
    im = QImage(width(),height(),QImage::Format_RGB32);
    im.fill(0);
    punkty.append({220,176,0});
    punkty.append({220,176,50});
    punkty.append({440,176,0});
    punkty.append({440,176,50});
    punkty.append({220,352,0});
    punkty.append({220,352,50});
    punkty.append({440,352,0});
    punkty.append({440,352,50});
}


void Ekran::resizeEvent(QResizeEvent *){
    QImage imCopy;
    imCopy = im.copy();
    im = QImage(width(),height(),QImage::Format_RGB32);
    QPainter p(&im);
    p.drawImage(0,0,imCopy);

    liczMacierze();

}

void Ekran::paintEvent(QPaintEvent *){
    QPainter p(this);
    p.drawImage(0,0,im);
}


void Ekran::drawPixel(int x, int y){
    if(!(x< im.width() && x >= 0 && y >=0 && y< im.height())){
        return ;
    }
    uchar *pix = im.scanLine(y) ;
    pix[4*x]     = 255;
    pix[4*x + 1] = 255;
    pix[4*x + 2] = 255;
    update();
}
void Ekran::drawLine(int x1, int y1, int x2, int y2){
    // Obliczamy współczynnik kierunkowy linii
    double A = (double)(y2 - y1) / (x2 - x1);
    if(abs(A)>1){
        A = (double)(x2 - x1) / (y2 - y1);
        if (y2 < y1) {
            std::swap(y1, y2);
            std::swap(x1, x2);

        }
        double y = x1;
        for(int x = y1; x < y2; x++){
            drawPixel((int)floor(y),x);
            y += A;
        }
    }
    else{
        if (x2 < x1) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        double y = y1;

        // Przechodzimy przez każdy piksel linii i rysujemy go
        for (int x = x1; x <= x2; x++) {
            drawPixel(x, (int)floor(y));
            y += A;
        }
    }
    update();
}


void Ekran::dwaDtrzyD(QVector<punkt3D> pkt)
{
    punkty2D.clear();
    im.fill(0);

    for (int i = 0; i < pkt.size(); i++)
    {
        double x = pkt[i].x - width() / 2.0;
        double y = pkt[i].y - height() / 2.0;
        double z = pkt[i].z;

        int f=120;
        double projX = x / (z + f);
        double projY = y / (z + f);
        projX*=120;
        projY*=120;


        punkty2D.append(QPoint(projX + width() / 2.0, projY + height() / 2.0));
    }
    rysuj(punkty2D);
}

void Ekran::liczMacierze(){


    //Tx,ty,tz - przesuniecia

    //Przesuwamy obiekt na srodki
    double srodekX = (im.width() / 2.0) + tX;
    double srodekY = (im.height() / 2.0) + tY;
    double srodekZ = tZ;

    //Translacja
    double T[4][4] = {{1,0,0,tX},
                      {0,1,0,tY},
                      {0,0,1,tZ},
                      {0,0,0,1}};
    //Wynik = T * {{x}{y}{z}{1}}



    //Skalowanie względem punktu (0,0,0)
    double S[4][4] = {{sX,0,0,0},
                      {0,sY,0,0},
                      {0,0,sZ,0},
                      {0,0,0,1}};
    //Wynik S * {{x}{y}{z}{1}}


    //Rotacja odpowiednio dla X,Y,Z
    double Rx[4][4] = {{1,0,0,0},
                       {0,cos(alphaX* (M_PI / 180.0)),-sin(alphaX* (M_PI / 180.0)),0},
                       {0,sin(alphaX* (M_PI / 180.0)),cos(alphaX* (M_PI / 180.0)),0},
                       {0,0,0,1}};
    double Ry[4][4] = {{cos(alphaY* (M_PI / 180.0)),0,sin(alphaY* (M_PI / 180.0)),0},
                       {0,1,0,0},
                       {-sin(alphaY* (M_PI / 180.0)),0,cos(alphaY* (M_PI / 180.0)),0},
                       {0,0,0,1}};
    double Rz[4][4] = {{cos(alphaZ* (M_PI / 180.0)),-sin(alphaZ* (M_PI / 180.0)),0,0},
                       {sin(alphaZ* (M_PI / 180.0)),cos(alphaZ* (M_PI / 180.0)),0,0},
                       {0,0,1,0},
                       {0,0,0,1}};

    QVector<punkt3D> nowePunkty;
    double punkt[4] = {0.0,0.0,0.0,1.0};// Tablica przechowujaca przesuniete wspolrzedne punktu
    int tab1[4];
    int tab2[4];//Dwie tablice potrzebne do


    for(int i = 0; i < 8 ; i++ )
    {
        punkt[0] = punkty[i].x - srodekX ;
        punkt[1] = punkty[i].y - srodekY;
        punkt[2] = punkty[i].z - srodekZ;


        for(int x = 0; x < 4 ; x++)
        { //przesuniecie
            tab1[x] = 0;
            for (int y = 0; y < 4; ++y)
            {
                tab1[x] += punkt[y] * T[x][y];
            }
        }

        for(int x = 0; x < 4 ; x++) // skalowanie
        {
            tab2[x] = 0;
            for (int y = 0; y < 4; ++y) {
                tab2[x] += tab1[y] * S[x][y];
            }
        }

        for(int x = 0; x < 4 ; x++)//rotacja X
        {
            tab1[x] = 0;
            for (int y = 0; y < 4; ++y) {
                tab1[x] += tab2[y] * Rz[x][y];
            }

        }
        for(int x = 0; x < 4 ; x++) //rotacja Y
        {
            tab2[x] = 0;
            for (int y = 0; y < 4; ++y) {
                tab2[x] += tab1[y] * Ry[x][y];

            }
        }
        for(int x = 0; x < 4 ; x++) //rotacja Z
        {
             tab1[x] = 0;
            for (int y = 0; y < 4; ++y) {
                 tab1[x] += tab2[y] * Rx[x][y];
            }
        }

        nowePunkty.append({tab1[0]+(int)srodekX ,tab1[1]+ (int)srodekY, tab1[2] + (int) srodekZ});
    }
    dwaDtrzyD(nowePunkty);


}
void Ekran::rysuj(QVector<QPoint>punkty)
{
    drawLine(punkty[0].x(),punkty[0].y(),punkty[1].x(),punkty[1].y());
    drawLine(punkty[0].x(),punkty[0].y(),punkty[2].x(),punkty[2].y());
    drawLine(punkty[0].x(),punkty[0].y(),punkty[4].x(),punkty[4].y());
    drawLine(punkty[1].x(),punkty[1].y(),punkty[3].x(),punkty[3].y());
    drawLine(punkty[1].x(),punkty[1].y(),punkty[5].x(),punkty[5].y());
    drawLine(punkty[2].x(),punkty[2].y(),punkty[3].x(),punkty[3].y());
    drawLine(punkty[2].x(),punkty[2].y(),punkty[6].x(),punkty[6].y());
    drawLine(punkty[3].x(),punkty[3].y(),punkty[7].x(),punkty[7].y());
    drawLine(punkty[5].x(),punkty[5].y(),punkty[4].x(),punkty[4].y());
    drawLine(punkty[5].x(),punkty[5].y(),punkty[7].x(),punkty[7].y());
    drawLine(punkty[4].x(),punkty[4].y(),punkty[6].x(),punkty[6].y());
    drawLine(punkty[6].x(),punkty[6].y(),punkty[7].x(),punkty[7].y());
}
void Ekran::transX(int val)
{
    tX = val ;
    liczMacierze();
}
void Ekran::transY(int val){
    tY = val ;
    liczMacierze();
}
void Ekran::transZ(int val){
    tZ = val ;
    liczMacierze();
}

void Ekran::scaleX(int val){
    sX = val/100.0;
    liczMacierze();
}
void Ekran::scaleY(int val){
    sY = val/100.0;
    liczMacierze();
}
void Ekran::scaleZ(int val){
    sZ = val/100.0;
    liczMacierze();
}
void Ekran::rotX(int val){
    alphaX = val;
    liczMacierze();
}
void Ekran::rotY(int val){
    alphaY = val;
    liczMacierze();
}
void Ekran::rotZ(int val){
    alphaZ = val;
    liczMacierze();
}



















