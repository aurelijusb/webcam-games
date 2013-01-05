#include <cv.h>
#include <iostream>
#include <fstream>
#include "Bubble.h"

using namespace std;

bool allegroInitialised = false;

Bubble::Bubble(int x, int y, int sizeX, int sizeY) {
    this->x = x;
    this->y = y;
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    scaleEffect = 0;
    state = normal;
    boomSound = NULL;
}

void Bubble::paint(IplImage *canvas) {
    scaleEffect += 20;
    if (scaleEffect > 360) {
        scaleEffect -= 360;
    }
    CvPoint point = cvPoint(x, y);
    double angeX = cos((float) scaleEffect / 180 * 3.14) * 5;
    double angeY = sin((float) scaleEffect / 180 * 3.14) * 5;
    CvSize size = cvSize(round(angeX) + sizeX, round(angeY) + sizeY);
    if (state == normal) {
        CvScalar color1 = CV_RGB(255, 255 - abs(angeX * 20), abs(angeY * 30));
        CvScalar color2 = CV_RGB(255, 255 - abs(angeX * 20), abs(angeY * 30));
        CvSize size2 = cvSize(size.width * 8 / 10, size.height * 8 / 10);
        cvEllipse(canvas, point, size, 0, 0, 360, color1);
        cvEllipse(canvas, point, size2, 0, 30, 120, color2);
        CvFont font;
        double hScale=1.0;
        double vScale=1.0;
        int    lineWidth=1;
        cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale, vScale,
                   0, lineWidth);
        cvPutText (canvas, "$", cvPoint(x,y), &font, color1);
    } else if (state == exploding) {
        sizeX *= 1.8;
        sizeY *= 1.1;
        for (int i = 0; i < 360; i += 25 + rand() % 40) {
            double aX = cos((float) (i+rand() % 13) / 180 * 3.14) *
                                    (rand() % sizeX / 2);
            double aY = sin((float) (i+rand() % 14) / 180 * 3.14) *
                                    (rand() % sizeY / 2);
            CvSize size = cvSize(round(angeX) + sizeX, round(angeY) + sizeY);
            CvSize size2 = cvSize(size.width * 4 / 10 + rand() % 12,
                                  size.height * 8 / 10 + rand() % 4);
            CvScalar color1 = CV_RGB(rand() % 128 + 128, 255 - rand() % 60, 255);
            cvEllipse(canvas, point, size, 0, i, i + rand() % 5, color1,1);
            cvEllipse(canvas, point, size2, 0, i + rand() % 2,
                                               i + 5 + rand() % 14, color1);
            cvLine(canvas, cvPoint(x, y), cvPoint(x + aX, y + aY), color1);
        }
        if (sizeX > 300) {
            state = dead;
        }
    }
}

const int Bubble::getX() {
    return x;
}

const int Bubble::getY() {
    return y;
}

const int Bubble::getSizeX() {
    return x;
}

const int Bubble::getSizeY() {
    return y;
}

void Bubble::translate(int x, int y) {
    this->x += x;
    this->y += y;
}

void Bubble::scale(float x, float y) {
    this->sizeX = round(this->sizeX * x);
    this->sizeY = round(this->sizeY * y);
}

bool Bubble::isInside(int x, int y) {
    int sizeMax = sizeX > sizeY ? sizeX : sizeY;
    return sqrt( (this->x - x)*(this->x - x) + (this->y - y)*(this->y - y) ) < sizeMax;
}

void Bubble::boom() {
    if (state == normal) {
        state = exploding;
//        FILE *stream;
//        stream = popen("mplayer Data/pokst.ogg", "r");
//        while ( fgetc(stream) );
//        pclose(stream);   
    }
}

bool Bubble::isDead() {
    return state == dead;
}