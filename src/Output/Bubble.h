#ifndef BUBBLE_H
#define	BUBBLE_H

#include <opencv/cv.h>

enum BubbleState {
    normal,
    exploding,
    dead
};

/**
 * Animated graphical object.
 */
class Bubble {
public:
    Bubble(int x, int y, int sizeX = 10, int sizeY = 10);
    void paint(IplImage *canvas);
    const int getX();
    const int getY();
    void translate(int x, int y);
    void scale(float x, float y);
    const int getSizeX();
    const int getSizeY();
    bool isInside(int x, int y);
    void boom();
    bool isDead();
    bool isAlive();
protected:
    int x;
    int y;
    int sizeX;
    int sizeY;
    int scaleEffect;
    BubbleState state;
    void *boomSound;
};

#endif	/* BUBBLE_H */

