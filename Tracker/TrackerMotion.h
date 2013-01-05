#ifndef TRACKERMOTION_H
#define	TRACKERMOTION_H

#include <string.h>

#include "TrackerBase.h"

#define MAP_MAX 8

#define MAP_X(x) (x)
#define MAP_Y(y) (MAP_MAX * (y))

class TrackerMotion: public TrackerBase {
public:
    TrackerMotion();
    virtual ~TrackerMotion();
    
    bool checkHorizontalLine(int y, int direction = 1, int xMin = 0, int xMax = MAP_MAX);
    bool checkVerticalLine(int x, int direction = 1, int yMin = 0, int yMax = MAP_MAX);
    
    unsigned getIntensity(int mapX, int mapY);
    unsigned getIntensityContinuous(int mapX, int mapY);
    
    void setArrowProperties(short r = 255, short g = 255, short b = 255, int xDiff = 0, int yDiff = 0, bool show = true);
    
    IplImage *getFrame();
protected:
    virtual void loop(const string &windowName, int wait = 33);
    void showWindow(const string &windowName, int wait = 33);
    virtual bool needLoop();
    virtual bool inLoop(int wait = 33);
    void distroyWindow(const string &windowName);
    virtual void destruct();
    virtual bool onKeyPress(char c);
    void showController();
    
    short arrowColor[3];
    bool showArrow;
    int arrowPosition[2];
    
    void updateMap();
    void showMap();
    
    
    void drawArrow(int x1, int y1, int x2, int y2, int thickness = 1, int cellWidth = 20, int cellHeight = 20);
    
    int map[MAP_MAX * MAP_MAX], mapContinuous[MAP_MAX * MAP_MAX];
    IplImage *oldFrame;
    IplImage *mapOutput;
    static const short MAP_OUTPUT_STEP = 100;
    
    int maxValueDifference;
    
    //FIXME:
    int D_diff;
    int D_div;
    int D_minus;
    
    bool passed;
    int key, neibhourKey;
    bool failValue, failDirection;
    int x;

private:
    IplImage *sourceFrame;
};

#endif	/* TRACKERMOTION_H */

