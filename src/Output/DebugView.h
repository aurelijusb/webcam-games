#ifndef DEBUG_VIEW_H
#define	DEBUG_VIEW_H

#include "../Tracker/TrackerMotion.h"
#include <list>

enum DebugScreen {
    Canny = 0,
    Difference,
    TimeHistory,
    Real,
};

/**
 * Debugging mode to ilustrate, how software works behind the scene
 */
class DebugView: public TrackerMotion {
public:
    DebugView(int webCamDevice, double threshold1, double threshold2);
    virtual ~DebugView();
    void run();
private:
    void loop(const string &windowName, int wait);
    bool onKeyPress(char c);
    void destruct();
    IplImage *canvasDifference;
    IplImage *canvasEdges;
    void updateDifference();
    void updateEdges();
    double threshold1;
    double threshold2;
    short screen;
};


#endif	/* DEBUG_VIEW_H */

