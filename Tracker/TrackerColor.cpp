#include "TrackerColor.h"

TrackerColor::TrackerColor() {
    picker = 0;
    initCaptureSize();
    showingTrackPoints = true;
    hystogram  = cvCreateImage(cvSize(100, 260), 8, 3);
    maxDifference = 20;
}

void TrackerColor::showPicker(unsigned x, unsigned y, const string &windowName, bool (*breakOnKeyPress)(char key), int wait) {
    if (!picker) {
        picker = cvCreateImage(cvSize(width, height), 8, 3);
    }
    short color[3];
    if (capture) {
        while ( cvGrabFrame(capture) ) {
            frame = cvQueryFrame(capture);
            if (flip) {
                cvFlip(frame, frame, 1);
            }
            char c = cvWaitKey(wait);
            if (!breakOnKeyPress(c)) {
                break;
            }
            cvCopy(frame, picker);
            color[0] = CV_R(frame, x, y);
            color[1] = CV_G(frame, x, y);
            color[2] = CV_B(frame, x, y);
            leaveDifference(color[0], color[1], color[2]);
            cvCircle(picker, cvPoint(x, y), 4, CV_RGB(255, 255, 255), 1);
            cvCircle(picker, cvPoint(x, y), 8, CV_RGB(color[0], color[1], color[2]), 3);
            cvCircle(picker, cvPoint(x, y), 12, CV_RGB(0, 0, 0), 1);
            cvShowImage(windowName.c_str(), picker);
            if (trackPoints.size()) {
                cvCopy(frame, picker);
                showTrackPoints(frame, picker);
                cvShowImage("Points", picker);
            }
        }
    }
}


void TrackerColor::leaveDifference(short pickedR, short pickedG, short pickedB) {
    if (frame && picker) {
        unsigned short r, g, b;
        for (int x = 0; x < frame->width; x++) {
            for (int y = 0; y < frame->height; y++) {
                r = CV_R(frame, x, y);
                g = CV_G(frame, x, y);
                b = CV_B(frame, x, y);
                if (IS_RGB_DIFFERENT(r, pickedR, g, pickedG, b, pickedB, maxDifference)) {
                    CV_RC(picker, x, y) = 255;
                    CV_GC(picker, x, y) = 255;
                    CV_BC(picker, x, y) = 0;
                } else {
                    CV_RC(picker, x, y) = r / 3;
                    CV_GC(picker, x, y) = g / 3;
                    CV_BC(picker, x, y) = b;
                }
            }
        }
    }
}

void TrackerColor::showTrackPoints(IplImage *dataFrame, IplImage *outputFrame) {
    int x, y, sizeX, sizeY;
    for (unsigned i = 0; i < trackPoints.size(); i++) {
        getPosition(dataFrame, i, x, y, sizeX, sizeY);
        cvCircle(outputFrame, cvPoint(x, y), 2, CV_RGB(trackPoints[i]->colorMax[0], trackPoints[i]->colorMax[1], trackPoints[i]->colorMax[2]), -1);
        cvCircle(outputFrame, cvPoint(x, y), 4, CV_RGB(trackPoints[i]->colorMin[0], trackPoints[i]->colorMin[1], trackPoints[i]->colorMin[2]), 2);
        cvLine(outputFrame, cvPoint(x-20, y), cvPoint(x+20, y), CV_RGB(255, 255, 255), 1);
        cvLine(outputFrame, cvPoint(x, y-20), cvPoint(x, y+20), CV_RGB(0, 0, 0), 1);
        
        //FIXME:
        IplImage *srcDifference = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);
        cvCopy(frame, srcDifference);
        int pickedR = (trackPoints[i]->colorMin[0] + trackPoints[i]->colorMax[0]) / 2;
        int pickedG = (trackPoints[i]->colorMin[1] + trackPoints[i]->colorMax[1]) / 2;
        int pickedB = (trackPoints[i]->colorMin[2] + trackPoints[i]->colorMax[2]) / 2;
        int xx = x;
        int yy = y;
        for( int y=0; y<frame->height; y++ ) {
            for( int x=0; x<frame->width; x++ ) {
                int short r1 = CV_R(frame, x, y);
                int short g1 = CV_G(frame, x, y);
                int short b1 = CV_B(frame, x, y);  
                if (IS_RGB_DIFFERENT(pickedR, r1, pickedG, g1, pickedB, b1, maxDifference)) {
                    CV_RC(srcDifference, x, y) = 0;
                    CV_GC(srcDifference, x, y) = 0;
                    CV_BC(srcDifference, x, y) = 0;
                } else {
                    CV_RC(srcDifference, x, y) = range( abs(pickedR-r1) + 120 );
                    CV_GC(srcDifference, x, y) = range( abs(pickedG-g1) + 120);
                    CV_BC(srcDifference, x, y) = range( abs(pickedB-b1) + 120);
                }
            }
        }
        cvLine(srcDifference, cvPoint(xx-40, yy), cvPoint(xx+40, yy), CV_RGB(0, 255, 0), 2);
        cvLine(srcDifference, cvPoint(xx, yy-40), cvPoint(xx, yy+40), CV_RGB(255, 0, 255), 2);
        string text = "diff" + stringify(i);
        cvShowImage(text.c_str(), srcDifference);
        cvReleaseImage(&srcDifference);
    }
}

#define IS_COLOR_PART_IN(trackPoint, canvas, x, y, index, colorText) ((trackPoint->colorMin[index] < (colorText)) && ((colorText) < trackPoint->colorMax[index]))
#define IS_COLOR_IN(trackPoint, canvas, x, y) (IS_COLOR_PART_IN(trackPoint, canvas, x, y, 0, CV_R(canvas, x, y)) && IS_COLOR_PART_IN(trackPoint, canvas, x, y, 1, CV_G(canvas, x, y)) && IS_COLOR_PART_IN(trackPoint, canvas, x, y, 2, CV_B(canvas, x, y)))
void TrackerColor::getPosition(IplImage* canvas, int trackPointIndex, int& maxX, int& maxY, int& avgX, int& avgY) {
    maxX = 0;
    maxY = 0;
    avgX = -1;
    avgY = -1;
    int column;
    int maxColumn = 0;
    int row;
    int maxRow = 0;   
    int size = 8;
    TrackerPoint *point = trackPoints[trackPointIndex];     
    
    /* First column */
    for (int y = 0; y < canvas->height; y++) {
        if (IS_COLOR_IN(point, canvas, 0, y)) {
            maxColumn += size;
        }
    }
    
    /* Columns */
    for (int x = 1; x < canvas->width; x++) {
        column = 0;
        for (int y = 0; y < canvas->height; y++) {
            if (IS_COLOR_IN(point, canvas, x, y)) {
                column += size;
            }
        }
        if (maxColumn < column) {
            maxColumn = column;
            maxX = x;
        }
        avgX += column;
//        CV_GC(canvas, x, 0) = column;
    }
    avgX /= canvas->width;
    
    
    /* First row */
    for (int x = 0; x < canvas->width; x++) {
        if (IS_COLOR_IN(point, canvas, x, 0)) {
            maxRow += size;
        }
    }
    
    /* Rows */
    for (int y = 1; y < canvas->height; y++) {
        row = 0;
        for (int x = 0; x < canvas->width; x++) {
            if (IS_COLOR_IN(point, canvas, x, y)) {
                row += size;
            }
        }
        if (maxRow < row) {
            maxRow = row;
            maxY = y;
        }
        avgY += row;
//        CV_GC(canvas, 0, y) = row;
    }
    avgY /= canvas->height;
}

void TrackerColor::setAverage(IplImage *image, short *colorPoint, int x, int y, int width, int height, int maxWidth, int maxHeight) {
    int r = 0;
    int b = 0;
    int g = 0;
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (x + width > maxWidth) {
        width = maxWidth - x;
    }
    if (y + height > maxHeight) {
        height = maxHeight - y;
    }
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

int TrackerColor::addTrackPoint(unsigned x, unsigned y) {    
    TrackerPoint *newTrackPoint = new TrackerPoint();
    short colorAverage[3];
    setAverage(frame, colorAverage, x - 5, y - 5, 10, 10, frame->width, frame->height);
    newTrackPoint->lastPos[0] = x;
    newTrackPoint->lastPos[1] = y;
    newTrackPoint->balanceColor[0] = 0;//TODO: corner point
    newTrackPoint->balanceColor[1] = 0;
    newTrackPoint->balanceColor[2] = 0;
    newTrackPoint->colorMax[0] = range(colorAverage[0] + maxDifference);
    newTrackPoint->colorMax[1] = range(colorAverage[1] + maxDifference);
    newTrackPoint->colorMax[2] = range(colorAverage[2] + maxDifference);
    newTrackPoint->colorMin[0] = range(colorAverage[0] - maxDifference);
    newTrackPoint->colorMin[1] = range(colorAverage[1] - maxDifference);
    newTrackPoint->colorMin[2] = range(colorAverage[2] - maxDifference);
    trackPoints.push_back(newTrackPoint);
    return trackPoints.size() - 1;
}

TrackerColor::~TrackerColor() {
    for (unsigned i = 0; i < trackPoints.size(); i++) {
        delete trackPoints[i];
    }
    trackPoints.clear();
    if (picker) {
        cvReleaseImage(&picker);
    }
    if (hystogram) {
        cvReleaseImage(&hystogram);
    }
}