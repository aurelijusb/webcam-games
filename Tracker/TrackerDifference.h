/* 
 * File:   TrackerDifference.h
 * Author: aurelijus
 *
 * Created on September 7, 2010, 12:25 PM
 */

#ifndef TRACKERDIFFERENCE_H
#define	TRACKERDIFFERENCE_H

#include "TrackerBase.h"


class TrackerDifference: public TrackerBase {
public:
    TrackerDifference();
    virtual ~TrackerDifference();
    bool updateFrame();
    static const int step = 8;
protected:
    void calculateDifferences();
    IplImage *oldFrame;
    unsigned short *differences;
};

#endif	/* TRACKERDIFFERENCE_H */

