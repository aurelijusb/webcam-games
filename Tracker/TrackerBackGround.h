#ifndef TRACKERBACKGROUND_H
#define	TRACKERBACKGROUND_H
#include "TrackerBase.h"
#include <math.h>

class TrackerBackGround: public TrackerBase {
public:
    TrackerBackGround();
    void captureBackground();
    void showBackground(const string &windowName);
    void showDifference(const string &windowName);
    void maxDifference(short value);
    short maxDifference();
    void balancePoint(short value);
    short balancePoint();
    virtual ~TrackerBackGround();
protected:
    static const short COLOR_BALANCE_MAX_POINTS = 9;
    static const short colorBalanceSize = 10;
    short **colorBalncePoints;
    short **colorBalnceBacground;
    short **colorBalnceRuntime;
    short colorToBalance[3];
    short colorBalancePoint;
    void setAverage(IplImage *image, short *colorPoint, int x, int y, int width, int height);
    void initBacgroundColors();
    void initBalancing();
    void balanceFrame();
    
    unsigned maxColorDifference;
    void leaveDifference(short bacgroundR = 0, short bacgroundG = 0, short bacgroundB = 0);
    
    IplImage *background;
    IplImage *difference;
};

#endif	/* TRACKERBACKGROUND_H */


