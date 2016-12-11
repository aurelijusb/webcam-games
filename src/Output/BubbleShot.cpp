#include <iostream>
#include <cv.h>
#include "BubbleShot.h"

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
            (*i)->scale(1.1, 1.1);
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

void BubbleShot::addBubbles() {
    int sourceX = frame->width / 2;
    int sourceY = frame->height;
    int firstSize = 10;
    timer++;
    if (timer % 14 == 0) {
        bubbles.push_back(new Bubble(sourceX, sourceY, firstSize, firstSize));
        bubbles.push_back(new Bubble(frame->width / 4, sourceY, firstSize,
                                     firstSize));
        bubbles.push_back(new Bubble(frame->width / 4 * 3, sourceY, firstSize,
                                     firstSize));
    }
    if (timer > 100) {
        timer = 0;
    }
}