#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMouseEvent>
#include <QPainter>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    im_source = QImage(ui->lewyFrame->width(), ui->lewyFrame->height(), QImage::Format_RGB32);
    im_destination = QImage(ui->prawyFrame->width(), ui->prawyFrame->height(), QImage::Format_RGB32);

    connect(ui->selectKot, &QPushButton::clicked, this, [this]()
    {
        changeLayer(1);
    });
    connect(ui->selectPies, &QPushButton::clicked, this, [this]()
    {
        changeLayer(2);
    });

    ui->suwak->setRange(0,frames);
    connect(ui->suwak,SIGNAL(valueChanged(int)),this,SLOT(suwak(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(ui->lewyFrame->x(), ui->lewyFrame->y(), im_source);
    p.drawImage(ui->prawyFrame->x(), ui->prawyFrame->y(), im_destination);
}

void MainWindow::drawPixel(QImage *im, QPoint p, QColor color){
    if(p.x()<im->width() && p.x()>=0 && p.y()<im->height() && p.y()>=0)
    {
        uchar *pix = im->scanLine(p.y())+4*p.x();
        pix[0]=color.blue();
        pix[1]=color.green();
        pix[2]=color.red();
    }
}

void MainWindow::drawLine(QImage *im, QPoint start, QPoint end, QColor color){
    int x, y;
    if ((start.x()==end.x()) && (start.y()==end.y())){
        drawPixel(im, start, color);
        return;
    }
    else if(abs(end.y()-start.y()) <= abs(end.x()-start.x())){
        if (start.x() > end.x()){
            QPoint temp=start;
            start=end;
            end=temp;
        }
        for(x=start.x(); x<=end.x(); x++){
            y=start.y()+(x-start.x())*(end.y()-start.y())/(end.x()-start.x());
            drawPixel(im, {x, y}, color);
        }
    }
    else{
        if (start.y() > end.y()){
            QPoint temp=start;
            start=end;
            end=temp;
        }
        for(y=start.y(); y<=end.y(); y++)
        {
            x=start.x()+(y-start.y())*(end.x()-start.x())/(end.y()-start.y());
            drawPixel(im, {x, y}, color);
        }
    }
}


void MainWindow::mousePressEvent(QMouseEvent *e)
{
    QPoint sourceTopLeft = ui->lewyFrame->pos();
    int startX=e->pos().x()- sourceTopLeft.x();
    int startY=e->pos().y()- sourceTopLeft.y();
    QPoint pom(startX,startY);
    if(startX<ui->lewyFrame->width() && startX>=0 && startY<ui->lewyFrame->width() && startY>=0)
    {
        if (e->button() == Qt::LeftButton)
        {
            if(selectedImage==1)
            {
                if(sourcePoints1.size()<3) sourcePoints1.append(pom);
            }
            else if(selectedImage==2)
            {
                if(sourcePoints2.size()<3) sourcePoints2.append(pom);
            }
        }
        else if (e->buttons() & Qt::MiddleButton)
        {
            if(selectedImage==1)
            {
                iSourcePoints1 = findNearbyPoint(pom, sourcePoints1);
            }
            if(selectedImage==2)
            {
                iSourcePoints2 = findNearbyPoint(pom, sourcePoints2);
            }
        }
    }
    draw();
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    QPoint sourceTopLeft = ui->lewyFrame->pos();
    int startX=e->pos().x()- sourceTopLeft.x();
    int startY=e->pos().y()- sourceTopLeft.y();

   if(startX<ui->lewyFrame->width() && startX>=0 && startY<ui->lewyFrame->width() && startY>=0)
   {

        endPos = e->pos() - sourceTopLeft;
        if (e->buttons() & Qt::MiddleButton)
        {
            if(iSourcePoints1!=-1 && selectedImage==1){
                sourcePoints1.replace(iSourcePoints1, endPos);
            }
            else if(iSourcePoints2!=-1 && selectedImage==2){
                sourcePoints2.replace(iSourcePoints2, endPos);
            }
        }
    }
    draw();
}
void MainWindow::draw()
{
    //Ladujemy zdjecia na poczatku
    if(selectedImage==1)
    {
        im_source.load("C:/Users/student/Desktop/xd/Morphing/kot.jpg");
        im1=im_source;
    }
    else if(selectedImage==2)
    {
        im_source.load("C:/Users/student/Desktop/xd/Morphing/pies.jpg");
        im2=im_source;
    }
    //Wypelniamy nasz prawy obszar na czarno
    im_destination.fill(0);

    //Rysujemy trojkat zalezny od naszego wybranego "tla"
    if(selectedImage==1)
    {
        drawTriangle(&im_source, sourcePoints1);
    }
    else if(selectedImage==2)
    {
        drawTriangle(&im_source, sourcePoints2);
    }
    //Jesli wszystkie punkty sa to
    if(sourcePoints1.size()==3 && sourcePoints2.size()==3)
    {
        destinationPoints.clear();
        for(int i=0; i<3; i++){
            float x, y;
            x = (1.0f-frame/(double)frames)*sourcePoints1[i].x() + (frame/(double)frames)*sourcePoints2[i].x();
            y = (1.0f-frame/(double)frames)*sourcePoints1[i].y() + (frame/(double)frames)*sourcePoints2[i].y();
            destinationPoints.append({(int)round(x), (int)round(y)});
        }
        fill();
        drawTriangle(&im_destination, destinationPoints);
    }
    else if(sourcePoints1.size()==3 && frame==0)//Mamy wybrane lewy obrazek
    {
        destinationPoints=sourcePoints1;
        fill();
        drawTriangle(&im_destination, destinationPoints);
    }
    else if(sourcePoints2.size()==3 && frame==frames) //Mamy wybrany prawy obrazek
    {
        destinationPoints=sourcePoints2;
        fill();
        drawTriangle(&im_destination, destinationPoints);
    }
    update();
}

void MainWindow::drawTriangle(QImage *im, QVector<QPoint> points)
{
    for(int i=0; i<points.size(); i++){
        if(points.size()==3)
        {
            drawLine(im, points[i], points[(i+1)%3], Qt::white);
        }
    }
}
void MainWindow::fill()
{
    if(destinationPoints.size()<2) return;
    int yMax=destinationPoints[0].y(), yMin=destinationPoints[0].y();
    for(int i=0; i<destinationPoints.size(); i++)
    {
        if(destinationPoints[i].y()>yMax) yMax=destinationPoints[i].y();
        if(destinationPoints[i].y()<yMin) yMin=destinationPoints[i].y();
    }
    QVector<int> X;
    for(int y=yMin; y<=yMax; y++)
    {
        for(int i=0; i<destinationPoints.size(); i++)
        {
            int p1=i, p2=(i+1)%destinationPoints.size();//Dla kazdej pary punktow 0,1  1,2  2,0

            if(destinationPoints[p1].y()>destinationPoints[p2].y()) std::swap(p1, p2);// Zmiemiamy kolejnosc dla ulatwienia obliczen

            if (y >= destinationPoints[p1].y() && y < destinationPoints[p2].y() && destinationPoints[p1].y() != destinationPoints[p2].y()) //Jesli przecina to dokladamy punkt
            {
                X.push_back(destinationPoints[p1].x()+(y-destinationPoints[p1].y())*(destinationPoints[p2].x()-destinationPoints[p1].x())/(destinationPoints[p2].y()-destinationPoints[p1].y()));
            }
        }
        std::sort(X.begin(), X.end());// Sortujemy otrzymane dwa X
        if(X.size()>1)
        {
            for(int x=X[0]; x<X[1]; x++)
            {
                double W, Wv, Ww, u, v, w;
                double xt1, yt1, xt2, yt2;
                double alpha=frame/(double)frames;
                int red, green, blue;
                uchar *pix1, *pix2;
                QPoint a=destinationPoints[0], b=destinationPoints[1], c=destinationPoints[2];

                W = (b.x() - a.x())*(c.y() - a.y()) - (b.y() - a.y())*(c.x() - a.x());
                Wv = ((double)x - a.x())*(c.y() - a.y()) - ((double)y - a.y())*(c.x() - a.x());
                Ww = (b.x() - a.x())*((double)y - a.y()) - (b.y() - a.y())*((double)x - a.x());
                v=Wv/W;
                w=Ww/W;
                u=1-v-w; //Wspolrzedne barycentryczne


                if((frame>0 && frame<frames) || frame==0)
                {
                    QPoint at1=sourcePoints1[0], bt1=sourcePoints1[1], ct1=sourcePoints1[2];
                    xt1=u*at1.x() + v*bt1.x() + w*ct1.x();
                    yt1=u*at1.y() + v*bt1.y() + w*ct1.y();
                    pix1 = im1.scanLine((int)yt1)+4*(int)xt1;
                }
                if((frame>0 && frame<frames) || frame==frames)
                {
                    QPoint at2=sourcePoints2[0], bt2=sourcePoints2[1], ct2=sourcePoints2[2];
                    xt2=u*at2.x() + v*bt2.x() + w*ct2.x();
                    yt2=u*at2.y() + v*bt2.y() + w*ct2.y();
                    pix2 = im2.scanLine((int)yt2)+4*(int)xt2;
                }
                if(frame>0 && frame<frames)
                {
                    red = (int)(alpha * pix2[2] + (1 - alpha) * pix1[2]);
                    green = (int)(alpha * pix2[1] + (1 - alpha) * pix1[1]);
                    blue = (int)(alpha * pix2[0] + (1 - alpha) * pix1[0]);
                }
                else if(frame==0)
                {
                    red = pix1[2];
                    green = pix1[1];
                    blue = pix1[0];
                }
                else
                {
                    red = pix2[2];
                    green = pix2[1];
                    blue = pix2[0];
                }
                im_destination.setPixel(x, y, qRgb(red, green, blue));
            }
        }
        X.clear();
    }
}

int MainWindow::findNearbyPoint(QPoint p, QVector<QPoint> points)
{
    int minDist = -1;
    int n = -1;
    for (int i = 0; i < points.size(); i++){
        int dist = (points.at(i) - p).manhattanLength();
        if (minDist == -1 || dist < minDist) {
            minDist = dist;
            n = i;
        }
    }
    return n;
}

void MainWindow::changeLayer(int a)
{
    if(a==1)
    {
        selectedImage = 1;
    }
    else
    {
        selectedImage = 2;
    }
    draw();
}
void MainWindow::suwak(int frame)
{
    this->frame=frame;
    draw();
}
