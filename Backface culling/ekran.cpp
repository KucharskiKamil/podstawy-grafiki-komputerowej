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
int czyRysowac=0;
int czyTekstura=0;
void Ekran::resizeEvent(QResizeEvent *){
    QImage imCopy;
    imCopy = im.copy();
    im = QImage(width(),height(),QImage::Format_RGB32);
    obraz = QImage(width(), height(), QImage::Format_RGB32);
    obraz.load("C:/Users/KamilK/Desktop/3d od szymona/trzydelepsze/trzyde/image.jpg");
    QPainter p(&im);
    p.drawImage(0,0,imCopy);
    liczMacierze();

}

void Ekran::paintEvent(QPaintEvent *){
    QPainter p(this);
    p.drawImage(0,0,im);
}


void Ekran::drawPixel(int x, int y, int r,int g, int b)
{
    if(!(x< im.width() && x >= 0 && y >=0 && y< im.height())){
        return ;
    }
    uchar *pix = im.scanLine(y) ;
    pix[4*x]     = b;
    pix[4*x + 1] = g;
    pix[4*x + 2] = r;
    update();
}
void Ekran::drawLine(int x1, int y1, int x2, int y2, int r, int g, int b){
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
            drawPixel((int)floor(y),x,r,g,b);
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
            drawPixel(x, (int)floor(y),r,g,b);
            y += A;
        }
    }
    update();
}
bool Ekran::isWallVisible(punkt3D p0,punkt3D p1,punkt3D p2,punkt3D p3)
{
    //to jest na 100% dobrze
    float centerX = (p0.x + p1.x + p2.x + p3.x) / 4.0 ;
    float centerY = (p0.y + p1.y + p2.y + p3.y) / 4.0 ;
    float centerZ = (p0.z + p1.z + p2.z + p3.z) / 4.0 ;

    //    Vektor ze srodka sciany do mojego punktu "widoku"
    float wektor[3]={width()/2-centerX,(height()/2+(int)tY)-centerY,-110-centerZ};

    //od 0 do 4
    float w1[3]={(float)p2.x-p0.x,(float)p2.y-p0.y,(float)p2.z-p0.z};
    float w2[3]={(float)p1.x-p0.x,(float)p1.y-p0.y,(float)p1.z-p0.z};

    //obliczony wynik mnozenia wektorow
    float w3[3]={w1[1]*w2[2] - w2[1]*w1[2],w2[0]*w1[2] - w1[0]*w2[2],w1[0]*w2[1] - w2[0] * w1[1]};

    //n - sciana w -  od sciany do punktu obserwujacego
    float wynik=wektor[0]*w3[0] + wektor[1]*w3[1] + wektor[2]*w3[2];
    return wynik>0;
}
void Ekran::dwaDtrzyD(QVector<punkt3D> pkt)
{
    punkt3D p0 = pkt[0];
    punkt3D p1 = pkt[2];
    punkt3D p2 = pkt[4];
    punkt3D p3 = pkt[6];
    //Sciana przednia - 0,2,4,6
    //Lewa - 1,0,5,4
    //Prawa - 2,3,6,7
    //Tylnia - 3,1,7,5
    //Gorna - 1,3,0,2
    //Dolna - 4,6,5,7
    qDebug("\n\n\n\n\n\n\n\n");
    bool isVisiblePrzod=isWallVisible(p0,p1,p2,p3);
    if(isVisiblePrzod)
    {
        qDebug("Ściana Przednia: Widać");
    }
    else
    {
        qDebug("Ściana Przednia: Nie widać");
    }
    p0 = pkt[3];
    p1 = pkt[1];
    p2 = pkt[7];
    p3 = pkt[5];
    bool isVisibleTyl=isWallVisible(p0,p1,p2,p3);
    if(isVisibleTyl)
    {
        qDebug("Ściana Tylnia: Widać");
    }
    else
    {
        qDebug("Ściana Tylnia: Nie widać");
    }
    p0 = pkt[1];
    p1 = pkt[0];
    p2 = pkt[5];
    p3 = pkt[4];
    bool isVisibleLewa=isWallVisible(p0,p1,p2,p3);
    if(isVisibleLewa)
    {
        qDebug("Ściana Lewa: Widać");
    }
    else
    {
        qDebug("Ściana Lewa: Nie widać");
    }

    p0 = pkt[2];
    p1 = pkt[3];
    p2 = pkt[6];
    p3 = pkt[7];
    bool isVisiblePrawa=isWallVisible(p0,p1,p2,p3);
    if(isVisiblePrawa)
    {
        qDebug("Ściana Prawa: Widać");
    }
    else
    {
        qDebug("Ściana Prawa: Nie widać");
    }

    p0 = pkt[1];
    p1 = pkt[3];
    p2 = pkt[0];
    p3 = pkt[2];
    bool isVisibleGora=isWallVisible(p0,p1,p2,p3);
    if(isVisibleGora)
    {
        qDebug("Ściana Górna: Widać");
    }
    else
    {
        qDebug("Ściana Górna: Nie widać");
    }

    p0 = pkt[4];
    p1 = pkt[6];
    p2 = pkt[5];
    p3 = pkt[7];
    bool isVisibleDol=isWallVisible(p0,p1,p2,p3);
    if(isVisibleDol)
    {
        qDebug("Ściana Dolna: Widać");
    }
    else
    {
        qDebug("Ściana Dolna: Nie widać");
    }

    punkty2D.clear();
    im.fill(0);

    for (int i = 0; i < pkt.size(); i++)
    {
        double x = pkt[i].x - width() / 2.0;
        double y = pkt[i].y - height() / 2.0;
        double z = pkt[i].z;

        int f=110;
        double projX = x / (z + f);
        double projY = y / (z + f);
        projX*=110;
        projY*=110;
        punkty2D.append(QPoint(projX + width() / 2.0, projY + height() / 2.0));

    }
    if(czyRysowac)
    {
        if(isVisiblePrzod)
        {
            if(czyTekstura)
            {
                fillTriangle(punkty2D[0], punkty2D[4], punkty2D[6],1);
                fillTriangle(punkty2D[0], punkty2D[2], punkty2D[6],2);
            }
            drawLine(punkty2D[4].x(),punkty2D[4].y(),punkty2D[6].x(),punkty2D[6].y());
            drawLine(punkty2D[2].x(),punkty2D[2].y(),punkty2D[6].x(),punkty2D[6].y());
            drawLine(punkty2D[0].x(),punkty2D[0].y(),punkty2D[2].x(),punkty2D[2].y());
            drawLine(punkty2D[0].x(),punkty2D[0].y(),punkty2D[4].x(),punkty2D[4].y());
        }
        if(isVisibleTyl)
        {
            if(czyTekstura)
            {
                fillTriangle(punkty2D[1], punkty2D[3], punkty2D[7],1);
                fillTriangle(punkty2D[1], punkty2D[5], punkty2D[7],2);
            }

            drawLine(punkty2D[1].x(),punkty2D[1].y(),punkty2D[3].x(),punkty2D[3].y());
            drawLine(punkty2D[5].x(),punkty2D[5].y(),punkty2D[1].x(),punkty2D[1].y());
            drawLine(punkty2D[5].x(),punkty2D[5].y(),punkty2D[7].x(),punkty2D[7].y());
            drawLine(punkty2D[3].x(),punkty2D[3].y(),punkty2D[7].x(),punkty2D[7].y());
        }
        if(isVisiblePrawa)
        {
            if(czyTekstura)
            {
            fillTriangle(punkty2D[2], punkty2D[6], punkty2D[7],1);
            fillTriangle(punkty2D[2], punkty2D[3], punkty2D[7],2);
            }
            drawLine(punkty2D[3].x(),punkty2D[3].y(),punkty2D[7].x(),punkty2D[7].y());
            drawLine(punkty2D[2].x(),punkty2D[2].y(),punkty2D[3].x(),punkty2D[3].y());
            drawLine(punkty2D[2].x(),punkty2D[2].y(),punkty2D[6].x(),punkty2D[6].y());
            drawLine(punkty2D[6].x(),punkty2D[6].y(),punkty2D[7].x(),punkty2D[7].y());
        }
        if(isVisibleLewa)
        {
            if(czyTekstura)
            {
            fillTriangle(punkty2D[0], punkty2D[1], punkty2D[5],1);
            fillTriangle(punkty2D[0], punkty2D[4], punkty2D[5],2);
            }
            drawLine(punkty2D[1].x(),punkty2D[1].y(),punkty2D[0].x(),punkty2D[0].y());
            drawLine(punkty2D[1].x(),punkty2D[1].y(),punkty2D[5].x(),punkty2D[5].y());
            drawLine(punkty2D[5].x(),punkty2D[5].y(),punkty2D[4].x(),punkty2D[4].y());
            drawLine(punkty2D[0].x(),punkty2D[0].y(),punkty2D[4].x(),punkty2D[4].y());
        }
        if(isVisibleGora)
        {
            if(czyTekstura)
            {
            fillTriangle(punkty2D[1], punkty2D[0], punkty2D[2],1);
            fillTriangle(punkty2D[1], punkty2D[3], punkty2D[2],2);
            }
            drawLine(punkty2D[1].x(),punkty2D[1].y(),punkty2D[0].x(),punkty2D[0].y());
            drawLine(punkty2D[1].x(),punkty2D[1].y(),punkty2D[3].x(),punkty2D[3].y());
            drawLine(punkty2D[3].x(),punkty2D[3].y(),punkty2D[2].x(),punkty2D[2].y());
            drawLine(punkty2D[0].x(),punkty2D[0].y(),punkty2D[2].x(),punkty2D[2].y());
        }
        if(isVisibleDol)
        {
            if(czyTekstura){
            fillTriangle(punkty2D[4], punkty2D[5], punkty2D[6],1);
            fillTriangle(punkty2D[7], punkty2D[5], punkty2D[6],2);
            }
            drawLine(punkty2D[5].x(),punkty2D[5].y(),punkty2D[4].x(),punkty2D[4].y());
            drawLine(punkty2D[6].x(),punkty2D[6].y(),punkty2D[4].x(),punkty2D[4].y());
            drawLine(punkty2D[6].x(),punkty2D[6].y(),punkty2D[7].x(),punkty2D[7].y());
            drawLine(punkty2D[5].x(),punkty2D[5].y(),punkty2D[7].x(),punkty2D[7].y());
        }
    }
    else
    {
        rysuj(punkty2D);
    }
}
void Ekran::fillTriangle(QPoint p1, QPoint p2, QPoint p3,int a)
{
    QPoint cegla1 = {0,0};
    QPoint cegla2;
    if(a==1)
    {
        cegla2.setX(obraz.width()-1);
        cegla2.setY(0);
    }
    else
    {
        cegla2.setX(0);
        cegla2.setY(obraz.height()-1);
    }

    QPoint cegla3 = {obraz.width()-1,obraz.height()-1};
    QColor color;
    QVector<QPoint> points;
    points.append(p1);
    points.append(p2);
    points.append(p3);
    int najnizszy = points[0].y();
    int najwyzej = points[0].y();
    int najbardziejNaLewo = points[0].x();
    int najbardziejNaPrawo = points[0].x();

    for (int i = 1; i < (int)points.size(); i++)
    {
        if (points[i].y() < najnizszy)
            najnizszy = points[i].y();
        if (points[i].y() > najwyzej)
            najwyzej = points[i].y();
        if (points[i].x() < najbardziejNaLewo)
            najbardziejNaLewo = points[i].x();
        if (points[i].x() > najbardziejNaPrawo)
            najbardziejNaPrawo = points[i].x();
    }

    double wyznaczik = (points[0].y() - points[1].y()) * (points[2].x() - points[1].x()) + (points[1].x() - points[0].x()) * (points[2].y() - points[1].y());
    for (int y = najnizszy; y < najwyzej; y++)
    {
        for (int x = najbardziejNaLewo; x < najbardziejNaPrawo; x++)
        {
            double w = ((points[1].y() - points[2].y()) * (x - points[2].x()) + (points[2].x() - points[1].x()) * (y - points[2].y())) / wyznaczik;
            double v = ((points[2].y() - points[0].y()) * (x - points[2].x()) + (points[0].x() - points[2].x()) * (y - points[2].y())) / wyznaczik;
            double u = 1.0 - w - v;

            if (u >= 0 && u < 1 && v >= 0 && v < 1 && w >= 0 && w < 1)
            {
                double Punkt_x = u * cegla1.x() + v * cegla2.x() + w * cegla3.x();
                double Punkt_y = u * cegla1.y() + v * cegla2.y() + w * cegla3.y();

                color = obraz.pixelColor(Punkt_x, Punkt_y);
                if (x >= 0 && x < width() && y >= 0 && y < height())
                im.setPixelColor(x, y, color);
            }
        }
    }
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
            for (int y = 0; y < 4; ++y)
            {
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
void Ekran::stateChanged(int val)
{
    czyRysowac=val;
    liczMacierze();
    //2 wcisniete 0 nie
}
void Ekran::teksturaChanged(int val)
{
    //2 wcisniete, 0 nie
    czyTekstura=val;
    liczMacierze();
}


















