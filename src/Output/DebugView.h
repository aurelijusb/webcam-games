#ifndef DEBUG_VIEW_H
#define	DEBUG_VIEW_H

#include "../Tracker/TrackerDifference.h"
#include <list>

enum DebugScreen {
    Canny = 0,
    Difference,
    TimeHistory
};

/**
 * Debugging mode to ilustrate, how software works behind the scene
 */
class DebugView: public TrackerDifference {
public:
    DebugView(int webCamDevice, double threshold1, double threshold2);
    virtual ~DebugView();
    void run();
private:
    bool checkKey();
    IplImage *canvasDifference;
    IplImage *canvasEdges;
    void updateDifference();
    void updateEdges();
    double threshold1;
    double threshold2;
    short screen;
};


#endif	/* DEBUG_VIEW_H */

