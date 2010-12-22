#ifndef TRACKERBASE_H
#define	TRACKERBASE_H
#include <iostream>
#include <cxtypes.h>
#include <highgui.h>

#define CV_R( src, x, y ) (  (unsigned short) ( *((src)->imageData + (y) * (src)->widthStep + (x) * 3 + 2) & 0x00FF )  )
#define CV_G( src, x, y ) (  (unsigned short) ( *((src)->imageData + (y) * (src)->widthStep + (x) * 3 + 1) & 0x00FF )  )
#define CV_B( src, x, y ) (  (unsigned short) ( *((src)->imageData + (y) * (src)->widthStep + (x) * 3 + 3) & 0x00FF )  )

#define CV_RC( src, x, y ) ( *((src)->imageData + (y) * (src)->widthStep + (x) * 3 + 2) )
#define CV_GC( src, x, y ) ( *((src)->imageData + (y) * (src)->widthStep + (x) * 3 + 1) )
#define CV_BC( src, x, y ) ( *((src)->imageData + (y) * (src)->widthStep + (x) * 3 + 3) )

#define IS_RGB_DIFFERENT(R1, R2, G1, G2, B1, B2, MAX_DIFF) (((R1)<(R2)+(MAX_DIFF)) && ((R1)>(R2)-(MAX_DIFF)) && ((G1)<(G2)+(MAX_DIFF)) && ((G1)>(G2)-(MAX_DIFF)) && ((B1)<(B2)+(MAX_DIFF)) && ((B1)>(B2)-(MAX_DIFF)))

#define RANGE(variable, min, max) {if ((variable) < (min)) (variable) = (min); if ((variable > max)) (variable) = max; }

inline short range(int value, int from = 0, int to = 255) {
    if (value < from) {
        return from;
    } else if (value > to) {
        return to;
    } else {
        return value;
    }
}

inline std::string stringify(int x) {
   std::ostringstream o;
   o << x;
   return o.str();
} 

using namespace std;

class TrackerBase {
public:
    TrackerBase();
    TrackerBase(const TrackerBase& orig);
    void show(const string &windowName, bool (*breakOnKeyPress)(char key), int wait = 33);
    bool setFlip();
    bool setDebug();
    virtual ~TrackerBase();
protected:
    void initCaptureSize();
    bool flip;
    struct CvCapture *capture;
    IplImage *frame;
    int width;
    int height;
    bool debug;
};

#endif	/* TRACKERBASE_H */

