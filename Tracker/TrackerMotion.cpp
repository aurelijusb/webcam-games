#include <math.h>
#include <iostream>
#include "TrackerMotion.h"
//#include "../Output/WebCamVJ.h"
TrackerMotion::TrackerMotion() {
    for (int i = 0; i < MAP_MAX * MAP_MAX; i++) {
        map[i] = 0;
        mapContinuous[i] = 0;
    }
    oldFrame = 0;
//    mapOutput = cvCreateImage(cvSize(MAP_OUTPUT_STEP * MAP_MAX, MAP_OUTPUT_STEP * MAP_MAX), 8, 3);
    D_diff = 100;
    D_div = 20;
    D_minus = 20;
    
    arrowColor[0] = arrowColor[1] = arrowColor[2] = 255;
    showArrow = true;
    arrowPosition[0] = arrowPosition[1] = 0;
    
    maxValueDifference = 20;
}

void TrackerMotion::showWindow(const string &windowName, int wait) {
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
        if (!onKeyPress(c)) {
            break;
        }
        
        //FIXME:
        switch (c) {
            case 'q':
                if (D_diff > 0) {
                    D_diff--;
                }
                break;
            case 'w':
                D_diff++;
                break;
            case 'a':
                if (D_div > 0) {
                    D_div--;
                }
                break;
            case 's':
                D_div++;
                break;
            case 'x':
                if (D_minus > 0) {
                    D_minus--;
                }
                break;
            case 'z':
                D_minus++;
                break;
        }
        switch(c) {
            case 'q':
            case 'w':
                cout << "D DIFF: " << D_diff << endl;
                break;
            case 'a':
            case 's':
                cout << "D div : " << D_div << endl;
                break;
            case 'x':
            case 'z':
                cout << "D minus : " << D_minus << endl;
                break;
        }
        
        if (oldFrame) {
            updateMap();
//            showMap();
        } else {
            oldFrame = cvCreateImage(cvSize(frame->width, frame->height), 8, 3);
        }
//        cvShowImage(windowName.c_str(), frame);
        cvCopy(frame, oldFrame);
    }
    cvDestroyWindow(windowName.c_str());
}

bool TrackerMotion::onKeyPress(char c) {
    return true;
}

void TrackerMotion::updateMap() {
    short edgeX = frame->width / MAP_MAX;
    short edgeY = frame->height / MAP_MAX;
    int foundDifference;
    int xx, yy;
    for (int yBox = 0; yBox < frame->height; yBox += edgeY) {
        for (int xBox = 0; xBox < frame->width; xBox += edgeX) {
            foundDifference = 0;
            for (int y = 0; y < edgeY; y++) {
                for (int x = 0; x < edgeX; x++) {
                    yy = yBox + y;
                    xx = xBox + x;
                    if (!IS_RGB_DIFFERENT(CV_R(frame, xx, yy), CV_R(oldFrame, xx, yy),
                                         CV_G(frame, xx, yy), CV_G(oldFrame, xx, yy),
                                         CV_B(frame, xx, yy), CV_B(oldFrame, xx, yy),
                                         20)) {
                        foundDifference++;
                    }
                }
            }
            int key = MAP_X(xBox / edgeX) + MAP_Y(yBox / edgeY);
            map[key] = foundDifference / D_div;
            if (foundDifference > edgeX * edgeY / 3) {
                mapContinuous[key] = 255;
            }
            mapContinuous[key] -= D_minus;
            if (mapContinuous[key] < 0) {
                mapContinuous[key] = 0;
            }
            if (mapContinuous[key] > 255) {
                mapContinuous[key] = 255;
            }
        }
    }
}

unsigned TrackerMotion::getIntensity(int mapX, int mapY) {
    RANGE(mapX, 0, MAP_MAX);
    RANGE(mapY, 0, MAP_MAX);
    return map[MAP_X(mapX) + MAP_Y(mapY)];
}

unsigned TrackerMotion::getIntensityContinuous(int mapX, int mapY) {
    RANGE(mapX, 0, MAP_MAX);
    RANGE(mapY, 0, MAP_MAX);
    return map[MAP_X(mapX) + MAP_Y(mapY)];
}


void TrackerMotion::showMap() {
//    cvZero(mapOutput);
//    cout << endl;
//    for (int y = 0; y < MAP_MAX; y++) {
//        for (int x = 0; x < MAP_MAX; x++) {
//            int key = MAP_X(x) + MAP_Y(y);
//            int color = range(map[key]);
//            int color2 = range(mapContinuous[key]);
//            cvRectangle(mapOutput, cvPoint(x * MAP_OUTPUT_STEP + 2, y * MAP_OUTPUT_STEP + 2),
//                                   cvPoint((x + 1) * MAP_OUTPUT_STEP - 1, (y + 1) * MAP_OUTPUT_STEP - 1),
//                                   CV_RGB(color, color, color), -1);
//            cvRectangle(mapOutput, cvPoint(x * MAP_OUTPUT_STEP + 10, y * MAP_OUTPUT_STEP + 10),
//                                   cvPoint((x + 1) * MAP_OUTPUT_STEP - 10, (y + 1) * MAP_OUTPUT_STEP - 10),
//                                   CV_RGB(0, color2, color2), -1);
////            cout << mapContinuous[key] << '\t';
//        }
////        cout << endl;
//    }
    
//    cvShowImage("MAP OUTPUT", mapOutput);
}

/**
 * Checks if ther was motion in given area by given direction
 * 
 * @param y         row [0, max)
 * @param direction <- -1 | 1 -> 
 * @param xMin      starting column [0, max)
 * @param xMax      finishing column [min, max)
 * @return          true - motion was | false - was not
 */
bool TrackerMotion::checkHorizontalLine(int y, int direction, int xMin, int xMax) {
    bool passed = true;
    int key, neibhourKey;
    bool failValue, failDirection;
    if (xMin < 0) {
        xMin = 0;
    }
    if (xMax > MAP_MAX) {
        xMax = MAP_MAX;
    }
    if (y < 0) {
        y = 0;
    }
    if (y > MAP_MAX) {
        y = MAP_MAX;
    }
    for (int x = xMin; x < xMax; x++) {
        if (x < xMax - 1) {
            neibhourKey = MAP_X(x + 1) + MAP_Y(y);
        } else {
            neibhourKey = MAP_X(x) + MAP_Y(y);
        }
        key = MAP_X(x) + MAP_Y(y);
        failValue = mapContinuous[key] < maxValueDifference;
        failDirection = direction * mapContinuous[key] > direction * mapContinuous[neibhourKey];
        if (failValue || failDirection) {
            passed = false;
            break;
        }
    }

    int thickness = passed ? 3 : 1;
    short cellWidth = frame->width / MAP_MAX;
    short cellHeight = frame->height / MAP_MAX;
    if (direction > 0) {
        drawArrow(xMax - 1, y, xMin, y, thickness, cellWidth, cellHeight);
    } else {
        drawArrow(xMin, y, xMax - 1, y, thickness, cellWidth, cellHeight);
    }
    
    return passed;
}

bool TrackerMotion::checkVerticalLine(int x, int direction, int yMin, int yMax) {
    bool passed = true;
    int key, neibhourKey;
    bool failValue, failDirection;
    if (yMin < 0) {
        yMin = 0;
    }
    if (yMax > MAP_MAX) {
        yMax = MAP_MAX;
    }
    if (x < 0) {
        x = 0;
    }
    if (x > MAP_MAX) {
        x = MAP_MAX;
    }
    
    for (int y = yMin; y < yMax; y++) {
        if (y < yMax - 1) {
            neibhourKey = MAP_X(x) + MAP_Y(y + 1);
        } else {
            neibhourKey = MAP_X(x) + MAP_Y(y);
        }
        key = MAP_X(x) + MAP_Y(y);
        failValue = mapContinuous[key] < maxValueDifference;
        failDirection = direction * mapContinuous[key] > direction * mapContinuous[neibhourKey];
//        cout << "[" << key << "] " << mapContinuous[key] << "[" << neibhourKey << "] " << mapContinuous[neibhourKey] << " D " << direction << " FD " << failDirection  << " FV " << failValue << " PS " << passed << endl;
        if (failValue || failDirection) {
            passed = false;
            break;
        }
    }
    
    /* Arrow */
    int thickness = passed ? 3 : 1;
    short cellWidth = frame->width / MAP_MAX;
    short cellHeight = frame->height / MAP_MAX;
    if (direction > 0) {
        drawArrow(x, yMax -1, x, yMin, thickness, cellWidth, cellHeight);
    } else {
        drawArrow(x, yMin, x, yMax - 1, thickness, cellWidth, cellHeight);        
    }
    
    return passed;
}

void TrackerMotion::setArrowProperties(short r, short g, short b, int xDiff, int yDiff, bool show) {
    arrowColor[0] = range(r);
    arrowColor[1] = range(g);
    arrowColor[2] = range(b);
    arrowPosition[0] = xDiff;
    arrowPosition[1] = yDiff;
    showArrow = show;
}

void TrackerMotion::drawArrow(int x1, int y1, int x2, int y2, int thickness, int cellWidth, int cellHeight) {
    x1 = cellWidth * x1 + cellWidth / 2 + arrowPosition[0];
    y1 = cellHeight * y1 + cellHeight / 2 + arrowPosition[1];
    x2 = cellWidth * x2 + cellWidth / 2 + arrowPosition[0];
    y2 = cellHeight * y2 + cellHeight / 2 + arrowPosition[1];
    
    CvScalar color = CV_RGB(arrowColor[0], arrowColor[1], arrowColor[2]);
    cvLine(frame, cvPoint(x1, y1), cvPoint(x2, y2), color, thickness);
    int arrowLength = 20;
    int arrowWidth = 10;
    if (x1 == x2) {
        int y3;
        if (y1 > y2) {
            y3 = y1 - arrowLength;
        } else {
            y3 = y1 + arrowLength;
        }
        cvLine(frame, cvPoint(x1, y1), cvPoint(x2 - arrowWidth, y3), color, thickness);
        cvLine(frame, cvPoint(x1, y1), cvPoint(x2 + arrowWidth, y3), color, thickness);
    } else {
        int x3;
        if (x1 > x2) {
            x3 = x1 - arrowLength;
        } else {
            x3 = x1 + arrowLength;
        }
        cvLine(frame, cvPoint(x1, y1), cvPoint(x3, y2 - arrowWidth), color, thickness);
        cvLine(frame, cvPoint(x1, y1), cvPoint(x3, y2 + arrowWidth), color, thickness);
    }
}

IplImage *TrackerMotion::getFrame() {
    return frame;
}

TrackerMotion::~TrackerMotion() {
    if (oldFrame) {
        cvReleaseImage(&oldFrame);
    }
//    if (mapOutput) {
//        cvReleaseImage(&mapOutput);
//    }
}