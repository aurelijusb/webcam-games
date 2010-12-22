#ifndef WEBCAMVJ_H
#define	WEBCAMVJ_H
#include "WebCamEffect.h"
#include <vector>
#include "../Tracker/TrackerMotion.h"
#define RANGE(variable, min, max) {if ((variable) < (min)) (variable) = (min); if ((variable > max)) (variable) = max; }

class WebCamVJ: public TrackerMotion {
public:
    WebCamVJ();
    ~WebCamVJ();
    void run();
    void show(IplImage *background);
    void add(int mapX, int mapY, effectType type, const std::string file = "");
    void setIntensivity(int mapX, int mapY, unsigned value);
    void setMaxIntensivity(int mapX, int mapY, unsigned value);    
protected:
    void showBig();
    bool onKeyPress(char c);
    int getKey(int mapX, int mapY);
    std::vector<WebCamEffect*> effects;
    IplImage *frame;
    IplImage *bigImage;
        
};

#endif