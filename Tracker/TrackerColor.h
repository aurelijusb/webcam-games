#ifndef TRACKERCOLOR_H
#define	TRACKERCOLOR_H
#include "TrackerBase.h"
#include <vector>

struct TrackerPoint {
    short balanceColor[3];
    short colorMax[3];
    short colorMin[3];
    int lastPos[2];
};

class TrackerColor: public TrackerBase {
public:
    TrackerColor();
    void showPicker(unsigned x, unsigned y, const string &windowName, bool (*breakOnKeyPress)(char key), int wait = 33);
    int addTrackPoint(unsigned x, unsigned y);
    virtual ~TrackerColor();
protected:
    void leaveDifference(short pickedR = 0, short pickedG = 0, short pickedB = 0);
    void showTrackPoints(IplImage *dataFrame, IplImage *outputFrame);
    void getPosition(IplImage *canvas, int trackPointIndex, int &maxX, int &maxY, int &avgX, int& avgY);
    void setAverage(IplImage *image, short *colorPoint, int x, int y, int width, int height, int maxWidth, int maxHeight);
    vector<TrackerPoint*> trackPoints;
    IplImage *picker;
    int maxDifference;
    bool showingTrackPoints;
    short around;
    
public:
    void debug_trackpounts() {
        for (unsigned i = 0; i < trackPoints.size(); i++) {
            cout << i << ": " << trackPoints[i]->colorMin[0] << " " << trackPoints[i]->colorMin[1] << " " << trackPoints[i]->colorMin[2] << endl;
        }
    }
    IplImage *hystogram;
    void debug_showHystogram() {
        if (trackPoints.size()) {
            
            cvZero(hystogram);
            TrackerPoint *point = trackPoints[0];
            cvLine(hystogram, cvPoint(10, 0), cvPoint(10, point->colorMin[0]), CV_RGB(255, 0, 0), 3);
            cvLine(hystogram, cvPoint(20, 0), cvPoint(20, point->colorMax[0]), CV_RGB(255, 100, 100), 4);
            
            cvLine(hystogram, cvPoint(30, 0), cvPoint(30, point->colorMin[1]), CV_RGB(0, 255, 0), 3);
            cvLine(hystogram, cvPoint(40, 0), cvPoint(40, point->colorMax[1]), CV_RGB(100, 255, 100), 4);
            
            cvLine(hystogram, cvPoint(50, 0), cvPoint(50, point->colorMin[2]), CV_RGB(0, 0, 255), 3);
            cvLine(hystogram, cvPoint(60, 0), cvPoint(60, point->colorMax[2]), CV_RGB(100, 100, 255), 4);
            
            cvRectangle(hystogram, cvPoint(0, 256), cvPoint(30, 260), CV_RGB(point->colorMin[0], point->colorMin[1], point->colorMin[2]), -1);
            cvRectangle(hystogram, cvPoint(40, 256), cvPoint(70, 260), CV_RGB(point->colorMax[0], point->colorMax[1], point->colorMax[2]), -1);
            
            cvShowImage("hystogram", hystogram);
            
        }
    }
    
    void debug_changeMin(short color, short difference = 2) {
        if (trackPoints.size() && color >= 0 && color < 3) {
            trackPoints[0]->colorMin[color] = range(trackPoints[0]->colorMin[color] + difference);
        }
    }
    void debug_changeMax(short color, short difference = 2) {
        if (trackPoints.size() && color >= 0 && color < 3) {
            trackPoints[0]->colorMax[color] = range(trackPoints[0]->colorMax[color] + difference);
        }
    }
};

#endif	/* TRACKERCOLOR_H */

