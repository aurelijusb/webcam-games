#include <opencv/cv.h>
#include "DebugView.h"
#include <iostream>

using namespace std;

DebugView::DebugView(double threshold1, double threshold2) {
    canvasDifference = NULL;
    canvasEdges = NULL;
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
    cvNamedWindow("DebugView: Difference", CV_WINDOW_NORMAL);
    cvNamedWindow("DebugView: Canny", CV_WINDOW_NORMAL);
    while (updateFrame()) {
        updateDifference();
        cvShowImage("DebugView: Difference", canvasDifference);
        updateEdges();
        cvShowImage("DebugView: Canny", canvasEdges);
        if (!checkKey()) {
            break;
        }
    }
}

bool DebugView::checkKey() {
    char c = cvWaitKey(1);
    switch (c) {
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