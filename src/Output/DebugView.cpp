#include <opencv/cv.h>
#include "DebugView.h"
#include <iostream>

using namespace std;

DebugView::DebugView(int webCamDevice, double threshold1, double threshold2): TrackerDifference(webCamDevice) {
    canvasDifference = NULL;
    canvasEdges = NULL;
    screen = Canny;
    this->threshold1 = threshold1;
    this->threshold2 = threshold2;
}

DebugView::~DebugView() {
    if (canvasDifference) {
        cvReleaseImage(&canvasDifference);
    }
    if (canvasEdges) {
        cvReleaseImage(&canvasEdges);
    }
}

void DebugView::run() {
    setFlip();
    string windowName = "DebugView";
    cvNamedWindow(windowName.c_str(), CV_WINDOW_NORMAL);
    cvSetWindowProperty(windowName.c_str(), CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    cout << "Use arrow keys (Up/Down) to change debug view" << endl;
    while (updateFrame()) {
        switch (screen) {
            case Canny:
                updateDifference();
                cvShowImage(windowName.c_str(), canvasDifference);
                break;
            default:
                updateEdges();
                cvShowImage(windowName.c_str(), canvasEdges);
        }
        if (!checkKey()) {
            break;
        }
    }
}

bool DebugView::checkKey() {
    char c = cvWaitKey(1);
    const char KEY_DOWN = 84;
    const char KEY_UP = 82;
    switch (c) {
        case KEY_DOWN:
            screen++;
            if (screen > TimeHistory) {
                screen = Canny;
            }
            break;
        case KEY_UP:
            screen -= 1;
            if (screen < 0) {
                screen = TimeHistory;
            }
            break;
        case 27:
        case 'q':
            return false;
    }
    return true;
}

void DebugView::updateDifference() {
    if (canvasDifference == NULL) {
        canvasDifference = cvCreateImage(cvSize(frame->width, frame->height), frame->depth, frame->nChannels);
    }
    cvAbsDiff(oldFrame, frame, canvasDifference);
}
void DebugView::updateEdges() {
    if (canvasEdges == NULL) {
        canvasEdges = cvCreateImage(cvSize(frame->width, frame->height), frame->depth, 1);
    }
    cvCanny(frame, canvasEdges, this->threshold1, this->threshold2);
}