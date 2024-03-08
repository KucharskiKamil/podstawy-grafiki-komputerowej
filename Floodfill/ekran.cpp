#include "ekran.h"
#include<qpainter.h>
#include<qlabel.h>
#include<QString>
#include <stack>
using namespace std;
int myRGBcolorR=255;
int myRGBcolorG=0;
int myRGBcolorB=0;
int drawMode=1;
int startX=-1;
int startY=-1;
int ellipsePoints=3;
vector<vector<int>> bezierPoints;
vector<vector<int>> bsplinePoints;
int bezierPointsAmount=0;
int bsplinePointsAmount=0;
int zmiennaGlobalnaPomocnicza=0;

/*

    1-pixele
    2-linia
    3-kolo
    4-elipsa
    5-krzywa beziera
    6-B-spline
    7-flood fill

 */
Ekran::Ekran(QWidget *parent)
    : QWidget{parent}
{
    im = QImage(851, 601, QImage::Format_RGB32);
    im.fill(0);
}
void Ekran::changeDrawColor(int n, int a, QLabel* label,QLabel* label2)
{
    if(a==1) myRGBcolorR=n;
    if(a==2) myRGBcolorG=n;
    if(a==3) myRGBcolorB=n;
    //Ustawia tlo labelu nizej na zmieniony kolor
    QString style = QString("background-color: rgb(%1, %2, %3)").arg(myRGBcolorR).arg(myRGBcolorG).arg(myRGBcolorB);
    label->setStyleSheet(style);
    //Ustawia text tabelu ponizej na aktualna wartosc(R,G,B)
    label2->setFont(QFont("Segoe UI", 11));
    label2->setAlignment(Qt::AlignCenter);
    label2->setText(QString("(%1,%2,%3)").arg(QString::number(myRGBcolorR), QString::number(myRGBcolorG), QString::number(myRGBcolorB)));
}
void Ekran::changeDrawMode(QListWidgetItem* item, QSlider* slider, QLabel* label) {
    QString nazwa = item->text(); // pobierz nazwę elementu
    QRegularExpression re("^\\d+\\."); // wyrażenie regularne pasujące do liczby zakończonej kropką
    QRegularExpressionMatch match = re.match(nazwa); // dopasuj wyrażenie regularne do nazwy przedmiotu
    if (match.hasMatch()) {
        QString numer = match.captured(0); // odczytaj dopasowaną liczbę
        numer.chop(1); // usuń kropkę z końca liczby
        drawMode = numer.toInt();
        qDebug() << "Wybrano nowy tryb rysowania: " << nazwa;
        if(drawMode==4)
        {
            slider->setVisible(1);
            label->setVisible(1);
        }
        else
        {
            slider->setVisible(0);
            label->setVisible(0);
        }
    } else {
        qDebug() << "Nie udało się odczytać numeru z nazwy elementu";
    }
}
void Ekran::changeEllipsePoints(int n,QLabel* label)
{
    ellipsePoints=n;
    label->setText(QString::number(n));
}
void Ekran::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
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
        pix[4*x]=myRGBcolorB;
        pix[4*x+1]=myRGBcolorG;
        pix[4*x+2]=myRGBcolorR;
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
void Ekran::drawCircle(int xK, int yK,int r)
{
    int cx = xK;
    int cy = yK;
    int x0 = 0;
    int y0 = r;
    int d = 1 - r;

    while (x0 <= y0)
    {
        drawPixel(cx + x0, cy + y0);
        drawPixel(cx - x0, cy + y0);
        drawPixel(cx + x0, cy - y0);
        drawPixel(cx - x0, cy - y0);
        drawPixel(cx + y0, cy + x0);
        drawPixel(cx - y0, cy + x0);
        drawPixel(cx + y0, cy - x0);
        drawPixel(cx - y0, cy - x0);

        if (d < 0)
        {
            d = d + 2 * x0 + 3;
        }
        else
        {
            d = d + 2 * (x0 - y0) + 5;
            y0--;
        }
        x0++;
    }
    update();
}
void Ekran::drawElipse(int x0, int y0, int x1, int y1)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int yE,xE;
    double alfa;
    int pomX;
    int pomY;
    // Rysowanie linii między kolejnymi wierzchołkami
    for (int i=0;i<=ellipsePoints;i++)
    {
        alfa = (i*2*M_PI)/ellipsePoints;//obliczenie kąta miedzy punktami
        xE = x0+dx*cos(alfa); //ruszanie pierwszej przekatnej wzgledem x
        yE = y0+dy*sin(alfa); //ruszanie drugiej przekatnej wzgledem y
        if(i!=0)
        {
            drawLine(xE, yE, pomX, pomY);
        }
        pomX = xE;
        pomY = yE;
    }
    update();
}
void Ekran::drawCurvePoints(vector<vector<int>> vect)
{
    for(uint i=0;i<vect.size();i++)
    {
        drawPixel(vect[i][0],vect[i][1]);
    }
    update();
}
void Ekran::consoleCurvePoints(vector<vector<int>> vect) //Funkcja wypisujaca aktualne punkty w naszym vektorze
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
    qDebug();
}
void Ekran::drawBezier()
{
    int k=10;
    if(bezierPoints.size()>3)
    {
        int poprzedniX, poprzedniY,punktX,punktY;

        for(uint i=0;i<bezierPoints.size()-3;i+=3)
        {

            for(int j=0;j<=k;j++)
            {
                float t = (float)j/(float)k;
                punktX=qPow(1-t,3) * bezierPoints[i][0] + qPow(1-t,2)*3*t*bezierPoints[i+1][0] + (1-t)*3*t*t*bezierPoints[i+2][0] + t*t*t * bezierPoints[i+3][0];
                punktY=qPow(1-t,3) * bezierPoints[i][1] + qPow(1-t,2)*3*t*bezierPoints[i+1][1] + (1-t)*3*t*t*bezierPoints[i+2][1] + t*t*t * bezierPoints[i+3][1];
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
    update();
}
void Ekran::drawBspline()
{
    int k=10;
    if(bsplinePoints.size()>3)
    {
        int poprzedniX, poprzedniY,punktX,punktY;
        for(uint i=0;i<bsplinePoints.size()-3;i++)
        {
            for(int j=0;j<=k;j++)
            {
                float t = (float)j/(float)k;
                punktX=(((-t*t*t+3*t*t-3*t+1)/6)*bsplinePoints[i][0])+(((3*t*t*t-6*t*t+4)/6)*bsplinePoints[i+1][0])+(((-3*t*t*t+3*t*t+3*t+1)/6)*bsplinePoints[i+2][0])+(((t*t*t)/6)*bsplinePoints[i+3][0]);
                punktY=(((-t*t*t+3*t*t-3*t+1)/6)*bsplinePoints[i][1])+(((3*t*t*t-6*t*t+4)/6)*bsplinePoints[i+1][1])+(((-3*t*t*t+3*t*t+3*t+1)/6)*bsplinePoints[i+2][1])+(((t*t*t)/6)*bsplinePoints[i+3][1]);
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
    update();
}
void Ekran::floodFill(int beginingX, int beginingY)
{
    stack<QPoint> floodStack;
    floodStack.push(QPoint(beginingX, beginingY));
    QColor targetColor = im.pixelColor(beginingX, beginingY);
    if((targetColor.red()==myRGBcolorR) && (targetColor.green()==myRGBcolorG) && (targetColor.blue()==myRGBcolorB))
    {
        return;
    }
    while (!floodStack.empty()) {
        QPoint current = floodStack.top();
        floodStack.pop();

        int x = current.x();
        int y = current.y();

        if (x < 0 || x >= im.width() || y < 0 || y >= im.height() || im.pixelColor(x, y) != targetColor)
        {
            continue;
        }

        drawPixel(x, y);

        floodStack.push(QPoint(x + 1, y));
        floodStack.push(QPoint(x - 1, y));
        floodStack.push(QPoint(x, y + 1));
        floodStack.push(QPoint(x, y - 1));
    }
    update();
}
void Ekran::mousePressEvent(QMouseEvent *e)
{
    startX=e->pos().x();
    startY=e->pos().y();
    if(startX<0 || startY<0 || startX>=im.width() || startY>=im.height()) return;
    //Pixel
    if(drawMode==1)
    {
        drawPixel(startX,startY);
        update();
    }
    //Linia
    if(drawMode==2)
    {
        im2=im;
    }
    //Kolo
    if(drawMode==3)
    {
        im2=im;
    }
    //Elipsa
    if(drawMode==4)
    {
        im2=im;
    }
    //Bezier
    if(drawMode==5)
    {
        if(e->button() == Qt::LeftButton)
        {
            if(e->pos().x() < 0 || e->pos().x() >= im.width() || e->pos().y()<0 || e->pos().y()>= im.height()) //Jesli jest w naszym miejscu do rysowania
            {
                return;
            }
            else
            {   //Zwiekszamy nasz vektor z punktami o 1 kolumne z 3 miejscami, do [0] wpisujemy X, [1] Y, [2] ID miejsca, a potem wszystko rysujemy
                bezierPoints.resize(bezierPointsAmount + 1);
                bezierPoints[bezierPointsAmount].resize(3);
                bezierPoints[bezierPointsAmount][0] = e->pos().x();
                bezierPoints[bezierPointsAmount][1] = e->pos().y();
                bezierPoints[bezierPointsAmount][2] = bezierPointsAmount;
                bezierPointsAmount++;
                consoleCurvePoints(bezierPoints);
                drawPixel(e->pos().x(),e->pos().y());
            }
            drawBezier();
            drawCurvePoints(bezierPoints);
        }
        if (e->button() == Qt::RightButton)
        {
            if(bezierPointsAmount==0)
            {
                qDebug("Brak punktow ktore mozna skasowac!");
                return;
            }
            else
            {
                float najm=99999999.9;
                int idNajm=0;
                for (uint i = 0; i < bezierPoints.size(); i++) //Obliczam ktory punkt ma najblizej do wcisnietego przeze mnie miejsca
                {
                    float odleglosc=((bezierPoints[i][0] - e->pos().x())*(bezierPoints[i][0] - e->pos().x()) + (bezierPoints[i][1] - e->pos().y())*(bezierPoints[i][1] - e->pos().y()));
                    if(odleglosc<najm)
                    {
                        najm=odleglosc;
                        idNajm=bezierPoints[i][2];
                    }
                }
                qDebug("Usuwam punkt %u\t x:%d y:%d", bezierPoints[idNajm][2], bezierPoints[idNajm][0], bezierPoints[idNajm][1]); //Usuwam punkt
                int R=myRGBcolorR;
                int G=myRGBcolorG;
                int B=myRGBcolorB;
                myRGBcolorR=0;
                myRGBcolorG=0;
                myRGBcolorB=0;
                drawPixel(bezierPoints[idNajm][0], bezierPoints[idNajm][1]);
                drawBezier();
                myRGBcolorR=R;
                myRGBcolorG=G;
                myRGBcolorB=B;
                for(uint i=idNajm;i<bezierPoints.size()-1;i++) //Dla tablicy punktow: kazdy z elementow po wybranym przez nas punkcie jest przesuwany w lewo o 1 a jego id zmniejszany (zeby zachowac ladna kolejnosc)
                {
                    bezierPoints[i][0]=bezierPoints[i+1][0];
                    bezierPoints[i][1]=bezierPoints[i+1][1];
                    bezierPoints[i][2]=bezierPoints[i+1][2];
                    bezierPoints[i][2]--;
                }
                bezierPointsAmount--;
                bezierPoints.resize(bezierPointsAmount); //Zmniejszam ilosc wierszy i rozmiast vektora 'punkty' przez co usuwa nam sie ostatni zbedny wiersz ktory jest kopia
                consoleCurvePoints(bezierPoints);
                drawBezier();
                drawCurvePoints(bezierPoints);
            }
        }
        if(e->button() == Qt::MiddleButton) //Przesuwanie pixela
        {
            int maxOdleglosc=9999999;
            for (uint i = 0; i < bezierPoints.size(); i++)
            {
                float odleglosc=((bezierPoints[i][0] - e->pos().x())*(bezierPoints[i][0] - e->pos().x()) + (bezierPoints[i][1] - e->pos().y())*(bezierPoints[i][1] - e->pos().y()));
                if(odleglosc<maxOdleglosc)
                {
                    zmiennaGlobalnaPomocnicza=bezierPoints[i][2];
                    maxOdleglosc=odleglosc;
                }
            }
            qDebug("Chwyciles punkt %d",zmiennaGlobalnaPomocnicza);
        }
    }
    //B-sklejane
    if(drawMode==6)
    {
        if(e->button() == Qt::LeftButton)
        {
            if(e->pos().x() < 0 || e->pos().x() >= im.width() || e->pos().y()<0 || e->pos().y()>= im.height()) //Jesli jest w naszym miejscu do rysowania
            {
                return;
            }
            else
            {   //Zwiekszamy nasz vektor z punktami o 1 kolumne z 3 miejscami, do [0] wpisujemy X, [1] Y, [2] ID miejsca, a potem wszystko rysujemy
                bsplinePoints.resize(bsplinePointsAmount + 1);
                bsplinePoints[bsplinePointsAmount].resize(3);
                bsplinePoints[bsplinePointsAmount][0] = e->pos().x();
                bsplinePoints[bsplinePointsAmount][1] = e->pos().y();
                bsplinePoints[bsplinePointsAmount][2] = bsplinePointsAmount;
                bsplinePointsAmount++;
                consoleCurvePoints(bsplinePoints);
                drawPixel(e->pos().x(),e->pos().y());
            }
            drawBspline();
            drawCurvePoints(bsplinePoints);
        }
        if (e->button() == Qt::RightButton)
        {
            if(bsplinePointsAmount==0)
            {
                qDebug("Brak punktow ktore mozna skasowac!");
                return;
            }
            else
            {
                float najm=99999999.9;
                int idNajm=0;
                for (uint i = 0; i < bsplinePoints.size(); i++) //Obliczam ktory punkt ma najblizej do wcisnietego przeze mnie miejsca
                {
                    float odleglosc=((bsplinePoints[i][0] - e->pos().x())*(bsplinePoints[i][0] - e->pos().x()) + (bsplinePoints[i][1] - e->pos().y())*(bsplinePoints[i][1] - e->pos().y()));
                    if(odleglosc<najm)
                    {
                        najm=odleglosc;
                        idNajm=bsplinePoints[i][2];
                    }
                }
                qDebug("Usuwam punkt %u\t x:%d y:%d", bsplinePoints[idNajm][2], bsplinePoints[idNajm][0], bsplinePoints[idNajm][1]); //Usuwam punkt
                int R=myRGBcolorR;
                int G=myRGBcolorG;
                int B=myRGBcolorB;
                myRGBcolorR=0;
                myRGBcolorG=0;
                myRGBcolorB=0;
                drawPixel(bsplinePoints[idNajm][0], bsplinePoints[idNajm][1]);
                drawBspline();
                myRGBcolorR=R;
                myRGBcolorG=G;
                myRGBcolorB=B;
                for(uint i=idNajm;i<bsplinePoints.size()-1;i++) //Dla tablicy punktow: kazdy z elementow po wybranym przez nas punkcie jest przesuwany w lewo o 1 a jego id zmniejszany (zeby zachowac ladna kolejnosc)
                {
                    bsplinePoints[i][0]=bsplinePoints[i+1][0];
                    bsplinePoints[i][1]=bsplinePoints[i+1][1];
                    bsplinePoints[i][2]=bsplinePoints[i+1][2];
                    bsplinePoints[i][2]--;
                }
                bsplinePointsAmount--;
                bsplinePoints.resize(bsplinePointsAmount); //Zmniejszam ilosc wierszy i rozmiast vektora 'punkty' przez co usuwa nam sie ostatni zbedny wiersz ktory jest kopia
                consoleCurvePoints(bsplinePoints);
                drawBspline();
                drawCurvePoints(bsplinePoints);
            }
        }
        if(e->button() == Qt::MiddleButton) //Przesuwanie pixela
        {
            int maxOdleglosc=9999999;
            for (uint i = 0; i < bsplinePoints.size(); i++)
            {
                float odleglosc=((bsplinePoints[i][0] - e->pos().x())*(bsplinePoints[i][0] - e->pos().x()) + (bsplinePoints[i][1] - e->pos().y())*(bsplinePoints[i][1] - e->pos().y()));
                if(odleglosc<maxOdleglosc)
                {
                    zmiennaGlobalnaPomocnicza=bsplinePoints[i][2];
                    maxOdleglosc=odleglosc;
                }
            }
            qDebug("Chwyciles punkt %d",zmiennaGlobalnaPomocnicza);
        }
    }
    //Fill
    if(drawMode==7)
    {
        floodFill(startX,startY);
    }
}
void Ekran::mouseMoveEvent(QMouseEvent *e)
{
    //Pixel
    if(drawMode==1)
    {
        drawPixel(e->pos().x(),e->pos().y());
        update();
    }
    //Linia
    if(drawMode==2)
    {
        im=im2;
        drawLine(startX,startY,e->pos().x(),e->pos().y());
    }
    //Kolo
    if(drawMode==3)
    {
        im=im2;
        int xK = e->pos().x();
        int yK = e->pos().y();
        int deltaX = startX - xK;
        int deltaY = startY - yK;
        int r = std::sqrt((float)(deltaX * deltaX) + deltaY * deltaY);
        drawCircle(startX, startY, r);
    }
    //Elipsa
    if(drawMode==4)
    {
        im=im2;
        drawElipse(startX,startY,e->pos().x(),e->pos().y());
    }

}
void Ekran::mouseReleaseEvent(QMouseEvent *e)
{
    //Bezier
    if(drawMode==5 && (e->button() == Qt::MiddleButton))
    {
        if(e->pos().x()==startX && e->pos().y()==startY)
        {
            return;
        }
        else
        {
            int R=myRGBcolorR;
            int G=myRGBcolorG;
            int B=myRGBcolorB;
            myRGBcolorR=0;
            myRGBcolorG=0;
            myRGBcolorB=0;
            drawPixel(bezierPoints[zmiennaGlobalnaPomocnicza][0], bezierPoints[zmiennaGlobalnaPomocnicza][1]);
            drawBezier();
            myRGBcolorR=R;
            myRGBcolorG=G;
            myRGBcolorB=B;
            bezierPoints[zmiennaGlobalnaPomocnicza][0]=e->pos().x();
            bezierPoints[zmiennaGlobalnaPomocnicza][1]=e->pos().y();
            drawBezier();
            drawCurvePoints(bezierPoints);
        }

    }
    if(drawMode==6 && (e->button() == Qt::MiddleButton))
    {
        if(e->pos().x()==startX && e->pos().y()==startY)
        {
            return;
        }
        else
        {
            int R=myRGBcolorR;
            int G=myRGBcolorG;
            int B=myRGBcolorB;
            myRGBcolorR=0;
            myRGBcolorG=0;
            myRGBcolorB=0;
            drawPixel(bsplinePoints[zmiennaGlobalnaPomocnicza][0], bsplinePoints[zmiennaGlobalnaPomocnicza][1]);
            drawBspline();
            myRGBcolorR=R;
            myRGBcolorG=G;
            myRGBcolorB=B;
            bsplinePoints[zmiennaGlobalnaPomocnicza][0]=e->pos().x();
            bsplinePoints[zmiennaGlobalnaPomocnicza][1]=e->pos().y();
            drawBspline();
            drawCurvePoints(bsplinePoints);
        }

    }
}


