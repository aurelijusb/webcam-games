#include <opencv/cv.h>
#include "DebugView.h"

using namespace std;

DebugView::DebugView(int webCamDevice, double threshold1, double threshold2): TrackerMotion(webCamDevice) {
    canvasDifference = NULL;
    canvasEdges = NULL;
    screen = Real;
    this->threshold1 = threshold1;
    this->threshold2 = threshold2;
}

DebugView::~DebugView() {
    destruct();
}

void DebugView::destruct() {
    TrackerMotion::destruct();
    if (canvasDifference) {
        cvReleaseImage(&canvasDifference);
    }
    if (canvasEdges) {
        cvReleaseImage(&canvasEdges);
    }
}


void DebugView::run() {
    loop("DebugView", 1);
}

void DebugView::loop(const string &windowName, int wait) {
    cvNamedWindow(windowName.c_str(), CV_WINDOW_NORMAL);
    cvSetWindowProperty(windowName.c_str(), CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    showWindow(windowName, wait);
    setFlip();

    cout << "Use arrow keys (Up/Down) to change debug view" << endl;
    while (TrackerMotion::needLoop()) {
        if (TrackerMotion::inLoop(1)) {
            switch (screen) {
                case Canny:
                    updateEdges();
                    cvShowImage(windowName.c_str(), canvasEdges);
                    break;
                case Difference:
                    updateDifference();
                    cvShowImage(windowName.c_str(), canvasDifference);
                    break;
                case TimeHistory:
                    showMap(windowName);
                    break;
                default:
                    cvShowImage(windowName.c_str(), frame);

            }
        } else {
            destruct();
        }
    }
}

bool DebugView::onKeyPress(char c) {
    const char KEY_DOWN = 84;
    const char KEY_UP = 82;
    switch (c) {
        case KEY_DOWN:
            screen++;
            if (screen > Real) {
                screen = Canny;
            }
            break;
        case KEY_UP:
            screen -= 1;
            if (screen < 0) {
                screen = Real;
            }
            break;
        case 27:
        case 'q':
            return false;
        default:
            return true;
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