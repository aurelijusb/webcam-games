#include <iostream>
#include <cv.h>

#include "TrackerRed.h"

TrackerRed::TrackerRed() {
    initCaptureSize();
    trackable = cvCreateImage(cvSize(width, height), 8, 3);
    scaled = cvCreateImage(cvSize(trackable->width/2, trackable->height/2), 8, 3);
    calibrator = cvCreateImage(cvSize(600, 600), 8, 3);
    mouseX = 0;
    diffX = 0;
    diffY = 0;
    pointX = 0;
    pointY = 0;
}

void TrackerRed::show(const string &windowName, bool (*breakOnKeyPress)(char key), int wait) {
    //FIXME:
    show(windowName, breakOnKeyPress, NULL, wait);
}
void TrackerRed::show(const string& windowName, bool(*breakOnKeyPress)(char), void(*onMouseEvent)(int,int,int,int,void*) , int wait) {
    bool first = true;
    while ( cvGrabFrame(capture) ) {
        frame = cvQueryFrame(capture);
        if (flip) {
            cvFlip(frame, frame, 1);
        }
        char c = cvWaitKey(wait);
        if (!breakOnKeyPress(c)) {
            break;
        }
        cvShowImage(windowName.c_str(), frame);
        showTrackable();
        if (first) {
            calibrate(0, 0, false);
            first = false;
        }
        cvZero(calibrator);
        cvRectangle(calibrator, cvPoint(550, 550), cvPoint(500, 500), CV_RGB(0, 255, 0), -1);
        cvRectangle(calibrator, cvPoint(550, 50), cvPoint(500, 100), CV_RGB(0, 0, 255), -1);
        cvCircle(calibrator, cvPoint(pointX, pointY), 8, CV_RGB(255, 0, 255), 2);
        
        cvSetMouseCallback("calibrate", onMouseEvent);
        
        cvShowImage("calibrate", calibrator);    
    }
    cvDestroyWindow(windowName.c_str());
}


void TrackerRed::calibrate(int x, int y, bool save) {
//    cvZero(calibrator);
    
//    cvRectangle(calibrator, cvPoint(50, 50), cvPoint(100, 100), CV_RGB(255, 0, 0), -1);

    if (x && y) {
        cvCircle(calibrator, cvPoint(x, y), 8, CV_RGB(0, 255, 255), -1);
        if (mouseX) {
            diffX = x - mouseX;
            diffY = y - mouseY;
        }
    }
    
}

void TrackerRed::showTrackable() {
    mouseX = 0;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (CV_R(frame, x, y) > 200 && CV_G(frame, x, y) < 100 && CV_B(frame, x, y) < 100) {
                CV_RC(trackable, x, y) = 255;
                CV_GC(trackable, x, y) = 0;
                CV_BC(trackable, x, y) = 0;
                pointX = diffX + x;
                pointY = diffY + y;
            } else if (CV_R(frame, x, y) < 150 && CV_G(frame, x, y) > 150 && CV_B(frame, x, y) < 150) {
                CV_RC(trackable, x, y) = 0;
                CV_GC(trackable, x, y) = 255;
                CV_BC(trackable, x, y) = 0;
            } else if (CV_R(frame, x, y) < 150 && CV_G(frame, x, y) < 150 && CV_B(frame, x, y) > 150) {
                CV_RC(trackable, x, y) = 0;
                CV_GC(trackable, x, y) = 0;
                CV_BC(trackable, x, y) = 255;
            } else if (CV_R(frame, x, y) < 150 && CV_G(frame, x, y) > 200 && CV_B(frame, x, y) > 200) {
                CV_RC(trackable, x, y) = 255;
                CV_GC(trackable, x, y) = 0;
                CV_BC(trackable, x, y) = 255;
                mouseX = x;
                mouseY = y;
            } else {
                CV_RC(trackable, x, y) = 0;
                CV_GC(trackable, x, y) = 0;
                CV_BC(trackable, x, y) = 0;                
            }
//            for (int xx = 0; xx < m; xx++) {
//                for (int yy = 0; yy < m; yy++) {
//                    CV_RC(scaled, x * m + xx, y * m + yy) = 0;
//                    CV_GC(scaled, x * m + xx, y * m + yy) = result;
//                    CV_BC(scaled, x * m + xx, y * m + yy) = result;
//                }
//            }
        }
    }
    cvPyrDown(trackable, scaled);
    cvShowImage("wall", trackable);
    cvShowImage("scaled", scaled);
}


TrackerRed::~TrackerRed() {
    if (trackable) {
        cvReleaseImage(&trackable);
    }
    if (scaled) {
        cvReleaseImage(&scaled);
    }
    if (calibrator) {
        cvReleaseImage(&calibrator);
    }
}