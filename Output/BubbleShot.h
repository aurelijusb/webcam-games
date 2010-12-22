/* 
 * File:   BubbleShot.h
 * Author: aurelijus
 *
 * Created on September 7, 2010, 11:48 AM
 */

#ifndef BUBBLESHOT_H
#define	BUBBLESHOT_H

#include "../Tracker/TrackerDifference.h"
#include <list>

#include "Bubble.h"

class BubbleShot: public TrackerDifference {
public:
    BubbleShot();
    virtual ~BubbleShot();
    void run();
protected:
    bool checkKey();
    void organiseBubbles();
    void paintBubbles();
    void debugMotion(int x, int y, int sizeX, int sizeY);
    void addBubbles();
    void deleteBubbles(list<Bubble*>& container);
    bool outOfRange(Bubble *object);
    static const int sensetive = 300;
    list<Bubble *> bubbles;
    IplImage *canvas;
private:
    int timer;
};


#endif	/* BUBBLESHOT_H */

