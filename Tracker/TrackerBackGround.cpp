#include <cxcore.h>
#include <highgui.h>
#include <iostream>
#include <sstream>
#include <string>

#include "TrackerBackGround.h"

TrackerBackGround::TrackerBackGround() {
    background = 0;
    difference = 0;
    maxColorDifference = 10;
    colorBalancePoint = -1;
    
    /* Color balance */
    initCaptureSize();
    if (width && height) {
        colorBalncePoints = new short*[COLOR_BALANCE_MAX_POINTS];
        colorBalnceBacground = new short*[COLOR_BALANCE_MAX_POINTS];
        colorBalnceRuntime = new short*[COLOR_BALANCE_MAX_POINTS];
        for (int i = 0; i < COLOR_BALANCE_MAX_POINTS; i++) {
            colorBalncePoints[i] = new short[2];
            colorBalnceBacground[i] = new short[3];
            colorBalnceRuntime[i] = new short[3];
        }
        colorBalncePoints[0][0] = colorBalncePoints[1][0]  = colorBalncePoints[2][0] = 0;
        colorBalncePoints[3][0] = colorBalncePoints[4][0]  = colorBalncePoints[5][0] = (width - colorBalanceSize) / 2;
        colorBalncePoints[6][0] = colorBalncePoints[7][0]  = colorBalncePoints[8][0] = width - colorBalanceSize;
        colorBalncePoints[0][1] = colorBalncePoints[3][1]  = colorBalncePoints[6][1] = 0;
        colorBalncePoints[1][1] = colorBalncePoints[4][1]  = colorBalncePoints[7][1] = (height - colorBalanceSize) / 2;
        colorBalncePoints[2][1] = colorBalncePoints[5][1]  = colorBalncePoints[8][1] = height - colorBalanceSize;
    } else {
        colorBalncePoints = 0;
        colorBalnceBacground = 0;
        colorBalnceRuntime = 0;
    }
    colorToBalance[0] = 0;
    colorToBalance[1] = 0;
    colorToBalance[2] = 0;
}

/** Background frame */
void TrackerBackGround::captureBackground() {
    if (frame) {
        if (!background) {
            background = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);
            difference = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);
        }
        cvCopy(frame, background);
        initBacgroundColors();
    }
}

void TrackerBackGround::showBackground(const string &windowName) {
    if (background) {
        cvShowImage(windowName.c_str(), background);
    }
}

/** Color balance */
void TrackerBackGround::initBalancing() {   
    for (int i = 0; i < COLOR_BALANCE_MAX_POINTS; i++) {
        setAverage(frame, colorBalnceRuntime[i], colorBalncePoints[i][0], colorBalncePoints[i][1], colorBalanceSize, colorBalanceSize);
    }
    if (colorBalancePoint >= 0) {
        colorToBalance[0] = colorBalnceBacground[colorBalancePoint][0] - colorBalnceRuntime[colorBalancePoint][0];
        colorToBalance[1] = colorBalnceBacground[colorBalancePoint][1] - colorBalnceRuntime[colorBalancePoint][1];
        colorToBalance[2] = colorBalnceBacground[colorBalancePoint][2] - colorBalnceRuntime[colorBalancePoint][2];
    }
}

void TrackerBackGround::setAverage(IplImage *image, short *colorPoint, int x, int y, int width, int height) {
    int r = 0;
    int b = 0;
    int g = 0;
    for (int xx = 0; xx < width; xx++) {
        for (int yy = 0; yy < height; yy++) {
            r += CV_R(image, x + xx, y + yy);
            g += CV_G(image, x + xx, y + yy);
            b += CV_B(image, x + xx, y + yy);
        }
    }
    r /= width * height;
    g /= width * height;
    b /= width * height;
    colorPoint[0] = range(r);
    colorPoint[1] = range(g);
    colorPoint[2] = range(b);
}

void TrackerBackGround::initBacgroundColors() {
    if (colorBalncePoints && colorBalnceBacground && width && height) {
        for (int i = 0; i < COLOR_BALANCE_MAX_POINTS; i++) {
            setAverage(background, colorBalnceBacground[i], colorBalncePoints[i][0], colorBalncePoints[i][1], colorBalanceSize, colorBalanceSize);
        }
    }
}

void TrackerBackGround::balancePoint(short value) {
    if ( (value >= -1) && (value < COLOR_BALANCE_MAX_POINTS) ) {
        colorBalancePoint = value;
    }
}
short TrackerBackGround::balancePoint() {
    return colorBalancePoint;
}

/** Difference */
void TrackerBackGround::showDifference(const string &windowName) {
    if (frame && background && difference) {
        leaveDifference();
        cvShowImage(windowName.c_str(), difference);
    }
}

void TrackerBackGround::leaveDifference(short bacgroundR, short bacgroundG, short bacgroundB) {
    if (frame && background && difference) {
        colorToBalance[0] = 0;
        colorToBalance[1] = 0;
        colorToBalance[2] = 0;
        if (colorBalancePoint >= 0) {
            initBalancing();
        }
        unsigned short r, g, b;
        short xx, yy, xxx, yyy;
        for (int x = 0; x < background->width; x++) {
            for (int y = 0; y < background->height; y++) {
                r = range(CV_R(frame, x, y) + colorToBalance[0]);
                g = range(CV_G(frame, x, y) + colorToBalance[1]);
                b = range(CV_B(frame, x, y) + colorToBalance[2]);
                xx = x > 0 ? x - 1 : x;
                yy = y > 0 ? y - 1 : y;
                xxx = x < background->width ? x + 1 : x;
                yyy = y < background->height ? y + 1 : y;
                /* Pints around (x, y) */
                if ( (IS_RGB_DIFFERENT(CV_R(background, x, y), r, CV_G(background, x, y), g, CV_B(background, x, y), b, maxColorDifference)) || 
                     (IS_RGB_DIFFERENT(CV_R(background, xx, y), r, CV_G(background, xx, y), g, CV_B(background, xx, y), b, maxColorDifference))  ||
                     (IS_RGB_DIFFERENT(CV_R(background, x, yy), r, CV_G(background, x, yy), g, CV_B(background, x, yy), b, maxColorDifference))  ||
                     (IS_RGB_DIFFERENT(CV_R(background, xx, yy), r, CV_G(background, xx, yy), g, CV_B(background, xx, yy), b, maxColorDifference)) ||
                     (IS_RGB_DIFFERENT(CV_R(background, xxx, y), r, CV_G(background, xxx, y), g, CV_B(background, xxx, y), b, maxColorDifference))  ||
                     (IS_RGB_DIFFERENT(CV_R(background, x, yyy), r, CV_G(background, x, yyy), g, CV_B(background, x, yyy), b, maxColorDifference))  ||
                     (IS_RGB_DIFFERENT(CV_R(background, xxx, yyy), r, CV_G(background, xxx, yyy), g, CV_B(background, xxx, yyy), b, maxColorDifference)) ||
                     (IS_RGB_DIFFERENT(CV_R(background, xxx, yy), r, CV_G(background, xxx, yy), g, CV_B(background, xxx, yy), b, maxColorDifference)) ||
                     (IS_RGB_DIFFERENT(CV_R(background, xx, yyy), r, CV_G(background, xx, yyy), g, CV_B(background, xx, yyy), b, maxColorDifference))) {
                    CV_RC(difference, x, y) = bacgroundR;
                    CV_GC(difference, x, y) = bacgroundG;
                    CV_BC(difference, x, y) = bacgroundB;
                    CV_RC(difference, xx, y) = bacgroundR;
                    CV_GC(difference, xx, y) = bacgroundG;
                    CV_BC(difference, xx, y) = bacgroundB;
                    CV_RC(difference, x, yy) = bacgroundR;
                    CV_GC(difference, x, yy) = bacgroundG;
                    CV_BC(difference, x, yy) = bacgroundB;
                    CV_RC(difference, xx, yy) = bacgroundR;
                    CV_GC(difference, xx, yy) = bacgroundG;
                    CV_BC(difference, xx, yy) = bacgroundB;
                } else {
                    CV_RC(difference, x, y) = CV_R(frame, x, y);
                    CV_GC(difference, x, y) = CV_G(frame, x, y);
                    CV_BC(difference, x, y) = CV_B(frame, x, y);
                }
            }
        }
    }
}

void TrackerBackGround::maxDifference(short value) {
    if ( (value >= 0) && (value <= 255) ) {
        maxColorDifference = value;
    }
}
short TrackerBackGround::maxDifference() {
    return maxColorDifference;
}


TrackerBackGround::~TrackerBackGround() {
    if (background) cvReleaseImage(&background);
    if (difference) cvReleaseImage(&difference);
    
    /* Color balance */
    if (colorBalncePoints && colorBalnceBacground && colorBalnceRuntime) {
        for (int i = 0; i < COLOR_BALANCE_MAX_POINTS; i++) {
            delete[] colorBalncePoints[i];
            delete[] colorBalnceBacground[i];
            delete[] colorBalnceRuntime[i];
        }
        delete[] colorBalncePoints;
        delete[] colorBalnceBacground;
        delete[] colorBalnceRuntime;
    }
}
