#ifndef TRACKERDIFFERENCE_H
#define	TRACKERDIFFERENCE_H

#include "TrackerBase.h"

/**
 * Calculates differences between current and old frame.
 */
class TrackerDifference: public TrackerBase {
public:
    TrackerDifference(int webCamDevice);
    virtual ~TrackerDifference();
    bool updateFrame();
    static const int step = 8;
protected:
    void calculateDifferences();
    IplImage *oldFrame;
    unsigned short *differences;
};

#endif	/* TRACKERDIFFERENCE_H */

