#include <iostream>
#include <highgui.h>
#include <iostream>

#include "TrackerBase.h"

TrackerBase::TrackerBase() {
    capture = 0;
    debug = false;
    frame = 0;
    flip = false;
    capture = NULL;
    while (!capture) {
        capture = cvCreateCameraCapture(0);
    }
}

TrackerBase::TrackerBase(const TrackerBase& orig) {
    //TODO: all
    capture = orig.capture;
    debug = orig.debug;
}

void TrackerBase::initCaptureSize() {
    width = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
    height = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
    std::cout << "WIDTH: " << width << " | HEIGHT: " << height << endl;
}

void TrackerBase::show(const string &windowName, bool (*breakOnKeyPress)(char key), int wait) {
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

bool TrackerBase::setDebug() {
    debug = debug ? false : true;
    return debug;
}

TrackerBase::~TrackerBase() {
    if (capture) {
        cvReleaseCapture(&capture);
    }
    if (frame) {
        cvReleaseImage(&frame);
    }
}



