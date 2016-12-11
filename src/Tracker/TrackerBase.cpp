#include <iostream>
#include <opencv/highgui.h>
#include <iostream>

#include "TrackerBase.h"

TrackerBase::TrackerBase() {
    capture = 0;
    frame = 0;
    flip = false;
    capture = NULL;
    while (!capture) {
        capture = cvCreateCameraCapture(0);
    }
}

TrackerBase::TrackerBase(const TrackerBase& orig) {
    capture = orig.capture;
}

void TrackerBase::initCaptureSize() {
    width = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
    height = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
}

void TrackerBase::show(const string &windowName,
                       bool (*breakOnKeyPress)(char key), int wait) {
    initCaptureSize();
    IplImage *sourceFrame;
    frame = cvCreateImage(cvSize(width, height), 8, 3);
    while ( cvGrabFrame(capture) ) {
        sourceFrame = cvQueryFrame(capture);
        if (flip) {
            cvFlip(sourceFrame, frame, 1);
        } else {
            cvCopy(sourceFrame, frame);
        }
        char c = cvWaitKey(wait);
        if (!breakOnKeyPress(c)) {
            break;
        }
        cvShowImage(windowName.c_str(), frame);
    }
    cvDestroyWindow(windowName.c_str());
}

bool TrackerBase::setFlip() {
    flip = flip ? false : true;
    return flip;
}

TrackerBase::~TrackerBase() {
    destruct();
}

void TrackerBase::destruct() {
    if (capture) {
        cvReleaseCapture(&capture);
    }
    if (frame) {
        cvReleaseImage(&frame);
    }
}
