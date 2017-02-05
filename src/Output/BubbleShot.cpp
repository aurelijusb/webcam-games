#include <iostream>
#include <iostream>
#include <opencv/cv.h>
#include "BubbleShot.h"
#include <sstream>

#define INT_TO_STRING( x ) static_cast< std::ostringstream & >(std::ostringstream() << std::dec << x ) ).str()

using namespace std;

const char * IntToString (int a)
{
    ostringstream temp;
    temp<<a;
    return temp.str().c_str();
}

BubbleShot::BubbleShot(int webCamDevice): TrackerDifference(webCamDevice) {
    previous = NULL;
    canvas = NULL;
    timer = 0;
    released = 0;
    lastExploded = 0;
    exploded = 0;
}

BubbleShot::~BubbleShot() {
    if (canvas) {
        cvReleaseImage(&canvas);
    }
    for (list<Bubble*>::iterator i = bubbles.begin(); i != bubbles.end(); ++i) {
        delete *i;
    }
    bubbles.clear();
}

void BubbleShot::run() {
    setFlip();
    while (updateFrame()) {
        calculateDifferences();
        addBubbles();
        organiseBubbles();
        paintBubbles();
        drawScore();
        fullScreen("BubbleShot", canvas);
        if (!checkKey()) {
            break;
        }
    }
}

bool BubbleShot::checkKey() {
    char c = cvWaitKey(3);
    switch (c) {
        case 27:
        case 'q':
            return false;
    }
    return true;
}

void BubbleShot::deleteBubbles(list<Bubble*>& container) {
    list<Bubble*>::iterator i = container.begin();
    while (i != container.end()) {
        if (isOutOfRange(*i) || (*i)->isDead()) {
            container.remove(*i);
            delete *i;
            i = container.begin();
        }
        ++i;
    }
}

bool BubbleShot::isOutOfRange(Bubble* object) {
    if ( (object->getX() < 0) ||
         (object->getY() < 0) ||
         (object->getX() >= frame->width) ||
         (object->getX() >= frame->height) ) {
        return true;
    } else {
        return false;
    }
}

void BubbleShot::organiseBubbles() {
    int nr = 0;
    for (list<Bubble*>::iterator i = bubbles.begin(); i != bubbles.end(); ++i) {
        (*i)->translate(rand() % 15 - 7, -rand() % 2 - 3);
        if (timer % 3 == 0) {
            (*i)->scale(1.05, 1.05);
        }
        nr++;
    }
    deleteBubbles(bubbles);

    if (!canvas) {
        canvas = cvCreateImage(cvSize(frame->width, frame->height),
                                      frame->depth, frame->nChannels);
    }
    cvCopy(frame, canvas);
    int diff;
    for(short x = 0; x < frame->width; x+=step) {
        for(short y = 0; y < frame->height; y+=step) {
            diff = differences[x + y * frame->width];
            if (diff > sensetive) {
                for (list<Bubble*>::iterator i = bubbles.begin();
                     i != bubbles.end(); ++i) {
                    if ((*i)->isInside(x, y)) {
                        if ((*i)->isAlive()) {
                            exploded++;
                        }
                        (*i)->boom();


                    }
                }
            }
        }
    }
}

void BubbleShot::paintBubbles() {
    for (list<Bubble*>::iterator i = bubbles.begin(); i != bubbles.end(); ++i) {
        (*i)->paint(canvas);
    }
}



void BubbleShot::drawScore() {
    int score256 = (int) ((double) exploded / released * 128);
    CvScalar color = CV_RGB(255 - score256, score256 + 128, score256 + 128);
    CvScalar colorBoom = CV_RGB(255, 255, 255);
    if (lastExploded != exploded) {
        color = colorBoom;
    }
    int x = (int) (canvas->width / (double) released * exploded);
    int width = 40;
    int height = 5;
    cvLine(canvas, cvPoint(x, height / 2), cvPoint(canvas->width / 2, height / 2), color, 1, 8, 0);
    cvRectangle(canvas, cvPoint(x - width/2, 0), cvPoint(x + width/2, height), exploded == lastExploded ? color : colorBoom, CV_FILLED, 8, 0);
    int direction = (exploded / (double) released) > 0.5 ? 1 : -1;
    cvLine(canvas, cvPoint(canvas->width / 2 + (height * direction), 0), cvPoint(canvas->width / 2, height / 2), color, 1, 8, 0);
    cvLine(canvas, cvPoint(canvas->width / 2 + (height * direction), height), cvPoint(canvas->width / 2, height / 2), color, 1, 8, 0);

    if (released > 0) {
        double score = exploded / (double) released;
        if (0.4 < score && score < 0.6) {
            drawHappySmile(color, 20, 20);
        } else {
            drawSadSmile(color, 20, 20);
        }
    }

    lastExploded = exploded;
}

void BubbleShot::drawHappySmile(const CvScalar& color, int top, int size) {
    CvSize big = cvSize(size, size);
    CvSize small = cvSize(size, size / 2);
    CvPoint point1 = cvPoint(canvas->width / 2, top);
    cvEllipse(canvas, point1, small, 0, 200, 230, color, 2);
    cvEllipse(canvas, point1, small, 0, 310, 340, color, 2);
    cvEllipse(canvas, point1, big, 0, 30, 150, color, 2);
}

void BubbleShot::drawSadSmile(const CvScalar& color, int top, int size) {
    CvSize big = cvSize(size, size);
    CvSize small = cvSize(size, size / 2);
    CvPoint point1 = cvPoint(canvas->width / 2, top);
    CvPoint point2 = cvPoint(canvas->width / 2, top + (size * 1.5));
    cvEllipse(canvas, point1, small, 0, 200, 230, color, 2);
    cvEllipse(canvas, point1, small, 0, 310, 340, color, 2);
    cvEllipse(canvas, point2, big, 180, 30, 150, color, 2);
}


void BubbleShot::addBubbles() {
    int sourceX = frame->width / 2;
    int sourceY = frame->height;
    int firstSize = 10;
    timer++;
    if (timer % 14 == 0) {
        bubbles.push_back(new Bubble(sourceX, sourceY, firstSize, firstSize));
        bubbles.push_back(new Bubble(frame->width / 8, sourceY, firstSize,
                                     firstSize));
        bubbles.push_back(new Bubble(frame->width / 8 * 3, sourceY, firstSize,
                                     firstSize));
        released += 3;
    }
    if (timer > 100) {
        timer = 0;
    }
}
