#include <math.h>
#include <iostream>
#include "TrackerDifference.h"

using namespace std;

TrackerDifference::TrackerDifference() {
    initCaptureSize();
    differences = new unsigned short[width*height];
    oldFrame = NULL;
}

TrackerDifference::~TrackerDifference() {
    if (oldFrame) {
        cvReleaseImage(&oldFrame);
    }
    delete[] differences;
}

bool TrackerDifference::updateFrame() {
    bool result = cvGrabFrame(capture);
    if (result) {
        if (!oldFrame) {
            oldFrame = cvCreateImage(cvSize(width, height), 8, 3);
        }
        if (frame) {
            cvCopy(frame, oldFrame);
        }
        frame = cvQueryFrame(capture);
        if (flip) {
            cvFlip(frame, frame, 1);
        }
        return true;
    } else {
        return result;
    }
}

void TrackerDifference::calculateDifferences() {
    for(int x = 0; x < frame->width; x+=step) {
        for(int y = 0; y < frame->height; y+=step) {
            differences[x + y*frame->width] =
                    abs(CV_R(frame, x, y) - CV_R(oldFrame, x, y)) + 
                    abs(CV_G(frame, x, y) - CV_G(oldFrame, x, y)) +
                    abs(CV_B(frame, x, y) - CV_B(oldFrame, x, y));

        }
    }
}