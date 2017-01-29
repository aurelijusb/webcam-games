#ifndef TRACKERMOTION_H
#define	TRACKERMOTION_H

#include <string.h>
#include "TrackerBase.h"

#define MAP_MAX 8
#define MAP_X(x) (x)
#define MAP_Y(y) (MAP_MAX * (y))

/**
 * Calculate basic motion if frames.
 */
class TrackerMotion: public TrackerBase {
public:
    TrackerMotion(int webCamDevice);
    virtual ~TrackerMotion();
    
    bool checkHorizontalLine(int y, int direction = 1, int xMin = 0,
                             int xMax = MAP_MAX);
    bool checkVerticalLine(int x, int direction = 1, int yMin = 0,
                          int yMax = MAP_MAX);
    
    unsigned getIntensity(int mapX, int mapY);
    unsigned getIntensityContinuous(int mapX, int mapY);
    
    void setArrowProperties(short r = 255, short g = 255, short b = 255,
                            int xDiff = 0, int yDiff = 0, bool show = true);

protected:
    IplImage *getFrame();
    virtual void loop(const string &windowName, int wait = 33);
    virtual bool needLoop();
    virtual bool inLoop(int wait = 33);
    
    void showWindow(const string &windowName, int wait = 33);
    virtual bool onKeyPress(char c);
    void distroyWindow(const string &windowName);
    virtual void destruct();
    
    void showController();
    void showMap(string name);
    IplImage *oldFrame;
private:
    static const int DIFFERENCE = 100;
    static const int DIVISION = 20;
    static const int MINUS = 20;
    static const short MAP_OUTPUT_STEP = 30;
    
    short arrowColor[3];
    bool showArrow;
    int arrowPosition[2];
    
    void updateMap();
    void drawArrow(int x1, int y1, int x2, int y2, int thickness = 1,
                   int cellWidth = 20, int cellHeight = 20);
    
    int map[MAP_MAX * MAP_MAX];
    int mapContinuous[MAP_MAX * MAP_MAX];
    IplImage *mapOutput;
    IplImage *sourceFrame;
    int maxValueDifference;
   
    bool passed;
    int key, neibhourKey;
    bool failValue, failDirection;
};

#endif	/* TRACKERMOTION_H */

