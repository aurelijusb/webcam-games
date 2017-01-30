#include <iostream>
#include <opencv/highgui.h>
#include <iostream>

#include "TrackerBase.h"

TrackerBase::TrackerBase(int webCamDevice) {
    capture = 0;
    frame = 0;
    flip = false;
    capture = NULL;
    displayFullScreen = NULL;

    while (!capture) {
        capture = cvCreateCameraCapture(webCamDevice);
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

void TrackerBase::fullScreen(string windowName, IplImage* image)
{
    cvNamedWindow(windowName.c_str(), CV_WINDOW_NORMAL);
    cvSetWindowProperty(windowName.c_str(), CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    double windowAspectRatio = cvGetWindowProperty(windowName.c_str(), CV_WND_PROP_ASPECTRATIO);
    double imageAspectRatio = image->width / (double) image->height;
    if (windowAspectRatio == imageAspectRatio) {
        cvShowImage(windowName.c_str(), image);
    } else {
        int width = image->width;
        int height = image->height;
        if (windowAspectRatio > imageAspectRatio) {
            width = (int) (windowAspectRatio * image->height);
        } else {
            height = (int) (image->width / windowAspectRatio);
        }
        if (
                !displayFullScreen
                || displayFullScreen->width != width || displayFullScreen->height != height
                || displayFullScreen->nChannels != image->nChannels || displayFullScreen->depth != image->depth
                || !image->width || !image->height
                ) {
            if (displayFullScreen) {
                cvReleaseImage(&displayFullScreen);
            }
            displayFullScreen = cvCreateImage(cvSize(width, height), image->depth, image->nChannels);
        }
        int x = (width - image->width) / 2;
        int y = (height - image->height) / 2;
        CvRect rect = cvRect(x, y, image->width, image->height);
        cvSet(displayFullScreen, CV_RGB(0,0,0));
        cvSetImageROI(displayFullScreen, rect);
        cvCopy(image, displayFullScreen);
        cvResetImageROI(displayFullScreen);
        cvShowImage(windowName.c_str(), displayFullScreen);
    }
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
