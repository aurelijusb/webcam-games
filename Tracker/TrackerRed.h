#ifndef TRACKRED_H
#define	TRACKRED_H

#include "TrackerBase.h"


class TrackerRed: public TrackerBase {
public:
    TrackerRed();
    virtual ~TrackerRed();
    void show(const string &windowName, bool (*breakOnKeyPress)(char key), int wait = 33);
    void show(const string &windowName, bool (*breakOnKeyPress)(char key), void (*onMouseEvent)(int event, int x, int y, int flags, void* param), int wait = 33);
    void calibrate(int x, int y, bool save = false);
protected:
    void showTrackable();
    IplImage *trackable;
    IplImage *scaled;
    IplImage *calibrator;
    int mouseX, mouseY;
    int diffX, diffY;
    int pointX, pointY;
};


#endif	/* TRACKRED_H */

