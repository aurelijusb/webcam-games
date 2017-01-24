#ifndef BUBBLESHOT_H
#define	BUBBLESHOT_H

#include "../Tracker/TrackerDifference.h"
#include <list>

#include "Bubble.h"

/**
 * Camara movement based game.
 * Bubbles are raising and blows when movement near buble is deteched.
 */
class BubbleShot: public TrackerDifference {
public:
    BubbleShot();
    virtual ~BubbleShot();
    void run();
private:
    bool checkKey();
    void organiseBubbles();
    void paintBubbles();
    void addBubbles();
    void deleteBubbles(list<Bubble*>& container);
    void drawScore();
    bool isOutOfRange(Bubble *object);
    static const int sensetive = 300;
    list<Bubble *> bubbles;
    IplImage *canvas;
    IplImage *previous;
    int timer;
    long released;
    long exploded;
};


#endif	/* BUBBLESHOT_H */

