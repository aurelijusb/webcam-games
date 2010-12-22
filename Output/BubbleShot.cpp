#include "BubbleShot.h"
#include <iostream>


#include <cv.h>

using namespace std;

BubbleShot::BubbleShot() {
    previous = NULL;
    canvas = NULL;
    timer = 0;
}

BubbleShot::~BubbleShot() {
    if (canvas) {
        cvReleaseImage(&canvas);
    }
//    if (previous) {
//        cvReleaseImage(&previous);
//    }
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
        cvShowImage("BubbleShot", canvas);
        if (!checkKey()) {
            break;
        }
//        if (frame) {
//            if (!previous) {
//                previous = cvCreateImage(cvSize(frame->width, frame->height), frame->depth, frame->nChannels);
//            }
//            cvCopy(frame, previous);
//        }
    }
}

bool BubbleShot::checkKey() {
    char c = cvWaitKey(3);
    switch (c) {
        case 27:
            return false;
    }
    return true;
}

void BubbleShot::deleteBubbles(list<Bubble*>& container) {
    list<Bubble*>::iterator i = container.begin();
    while (i != container.end()) {
        if (outOfRange(*i) || (*i)->isDead()) {
            container.remove(*i);
            delete *i;
            i = container.begin();
        }
        ++i;
    }
}

bool BubbleShot::outOfRange(Bubble* object) {
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
            (*i)->scale(1.1, 1.1);
        }
        nr++;
    }
    deleteBubbles(bubbles);

    if (!canvas) {
        canvas = cvCreateImage(cvSize(frame->width, frame->height), frame->depth, frame->nChannels);
    }
    cvCopy(frame, canvas);
    int diff;
    for(short x = 0; x < frame->width; x+=step) {
        for(short y = 0; y < frame->height; y+=step) {
//            diff = 0;
//            for (short xx = x; xx < x + step; xx++) {
//                for (short yy = y; yy < y + step; yy++) {
//                    diff += differences[xx + yy * frame->width];
//                }
//            }
            diff = differences[x + y * frame->width];
            if (diff > sensetive) {
                for (list<Bubble*>::iterator i = bubbles.begin(); i != bubbles.end(); ++i) {
                    if ((*i)->isInside(x, y)) {
                        (*i)->boom();
                    }
                }
//                debugMotion(x, y, 10, 10);
            }
        }
    }
//    debugMotion();
}

void BubbleShot::debugMotion() {
    if (previous) {
        IplImage *diff = cvCreateImage(cvSize(previous->width, previous->height), previous->depth, previous->nChannels);
        cvSub(previous, frame, diff);
        IplImage *diffBig = cvCreateImage(cvSize(previous->width*2, previous->height*2), previous->depth, previous->nChannels);
        cvResize(diff, diffBig, 1);
        cvShowImage("Diff", diffBig);
        cvReleaseImage(&diff);
        cvReleaseImage(&diffBig);
    }
}

void BubbleShot::paintBubbles() {
    for (list<Bubble*>::iterator i = bubbles.begin(); i != bubbles.end(); ++i) {
        (*i)->paint(canvas);
    }
    
    
//    IplImage *sub = cvCreateImage(cvSize(frame->width, frame->height), frame->depth, frame->nChannels);
//    cvSub(frame, oldFrame, sub);
//    cvShowImage("DEBUG", sub);
//    cvReleaseImage(&sub);
}

void BubbleShot::addBubbles() {
    int sourceX = frame->width / 2;
    int sourceY = frame->height;
    int firstSize = 10;
    timer++;
    if (timer % 14 == 0) {
        bubbles.push_back(new Bubble(sourceX, sourceY, firstSize, firstSize));
        bubbles.push_back(new Bubble(frame->width / 4, sourceY, firstSize, firstSize));
        bubbles.push_back(new Bubble(frame->width / 4 * 3, sourceY, firstSize, firstSize));
    }
    if (timer > 100) {
        timer = 0;
    }
}

void BubbleShot::debugMotion(int x, int y, int sizeX, int sizeY) {
    //TODO: elipse
    cvCircle(canvas, cvPoint(x, y), sizeX, CV_RGB(255, 0, 255), 1, 1);
}



