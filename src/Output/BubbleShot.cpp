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

BubbleShot::BubbleShot() {
    previous = NULL;
    canvas = NULL;
    timer = 0;
    released = 0;
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
    cvNamedWindow("BubbleShot", CV_WINDOW_NORMAL);
    cvSetWindowProperty("BubbleShot", CV_WND_PROP_FULLSCREEN,
                        CV_WINDOW_FULLSCREEN);
    while (updateFrame()) {
        calculateDifferences();
        addBubbles();
        organiseBubbles();
        paintBubbles();
        drawScore();
        cvShowImage("BubbleShot", canvas);
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
    CvFont font;
    double hScale=0.5;
    double vScale=0.5;
    int    lineWidth=1;
    cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale, vScale,
               0, lineWidth);
    cvPutText(canvas, IntToString(released), cvPoint(20, 20), &font, color);
    cvPutText(canvas, IntToString(exploded), cvPoint(canvas->width - 80, 20), &font, color);

    int x = (int) (canvas->width / (double) released * exploded);
    int width = 40;
    int height = 5;
    cvRectangle(canvas, cvPoint(x - width/2, 0), cvPoint(x + width/2, height), color, CV_FILLED, 8, 0);
    cvLine(canvas, cvPoint(x, height / 2), cvPoint(canvas->width / 2, height / 2), color, 1, 8, 0);


    if (released > 0) {
        double score = exploded / (double) released;
        if (0.4 < score && score < 0.6) {
            drawHappySmile(color, 20, 20);
        } else {
            drawSadSmile(color, 20, 20);
        }
    }
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
