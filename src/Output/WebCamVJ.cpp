#include <opencv/highgui.h>
#include "WebCamVJ.h"
#include "../Tracker/TrackerMotion.h"

#define INTESITY(x, y) { setIntensivity(x, y, getIntensity(x, y)); }
#define MAX_INTESITY(x, y) { setMaxIntensivity(x, y, getIntensity(x, y)); }

WebCamVJ::WebCamVJ(int webCamDevice): TrackerMotion(webCamDevice) {
    for (int i=0; i < MAP_MAX * MAP_MAX; i++) {
        effects.push_back(NULL);
    }
    frame = 0;
    bigImage = cvCreateImage(cvSize(1280, 960), 8, 3);
}


/*
 * Configurations.
 */

void WebCamVJ::run() {
    setFlip();
    add(2, 3, image, "../Data/img_1169.jpg");
    add(5, 3, image, "../Data/img_1196.jpg");
    
    add(1, 4, image, "../Data/img_1449.jpg");
    add(6, 4, image, "../Data/img_1449.jpg");
    
    add(0, 5, image, "../Data/img_9335-mantiuk.jpg");
    add(7, 5, image, "../Data/img_9685.jpg");

    add(0, 0, oneColor, "red");
    add(7, 0, oneColor, "green");

    add(3, 0, rectangules, "");
    add(4, 0, rectangules, "");

    cvNamedWindow("Motion tracker", CV_WINDOW_NORMAL);
    cvSetWindowProperty("Motion tracker", CV_WND_PROP_FULLSCREEN,
                        CV_WINDOW_FULLSCREEN);
    loop("Motion tracker");
}

bool WebCamVJ::onKeyPress(char c) {
    switch (c) {
        case 27:
        case 'q':
            return false;
    }

    setMaxIntensivity(2, 3, 128);
    setMaxIntensivity(5, 3, 128);
    setMaxIntensivity(1, 4, 128);
    setMaxIntensivity(6, 4, 128);
    setMaxIntensivity(0, 5, 128);
    setMaxIntensivity(7, 5, 128);

    setMaxIntensivity(0, 0, 128);
    setMaxIntensivity(7, 0, 128);

    setMaxIntensivity(3, 0, 255);
    setMaxIntensivity(4, 0, 255);

    INTESITY(2, 3);
    INTESITY(5, 3);
    INTESITY(1, 4);
    INTESITY(6, 4);
    INTESITY(0, 5);
    INTESITY(7, 5);

    INTESITY(0, 0);
    INTESITY(7, 0);

    INTESITY(3, 0);
    INTESITY(4, 0);

    show(getFrame());
    return true;
}


/*
 * Rendering
 */

void WebCamVJ::show(IplImage *background) {
    /* About boxes */
    int edgeWidth = background->width / MAP_MAX;
    int edgeHeight = background->height / MAP_MAX;
    for (int x = 0; x < MAP_MAX; x++) {
        for (int y = 0; y < MAP_MAX; y++) {
            int i = MAP_X(x) + MAP_Y(y);
            if (effects[i]) {
                effects[i]->markControllAreas(background,
                                              x * edgeWidth, y * edgeHeight,
                                              edgeWidth, edgeHeight);
            }
        }
    }
    
    /* Efects */
    if (!frame) {
        frame = cvCloneImage(background);
    }
    cvCopy(background, frame);
    for (unsigned i = 0; i < effects.size(); i++) {
        if (effects[i]) {
            effects[i]->apply(frame);
        }
    }
    showFullScreen();
}

void WebCamVJ::showFullScreen() {
    cvZero(bigImage);
    cvResize(frame, bigImage, CV_INTER_LINEAR);
    cvNamedWindow("Images", CV_WINDOW_NORMAL);
    cvSetWindowProperty("Images", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    cvShowImage("Images", bigImage);
}


/*
 * State
 */

void WebCamVJ::add(int mapX, int mapY, effectType type, const std::string file) {
    int key = getKey(mapX, mapY);
    if (!effects[key]) {
        effects[key] = new WebCamEffect(type, file);
    } else {
        cerr << "Key allready exists: " << mapX << " " << mapY << endl;
    }
}

void WebCamVJ::setIntensivity(int mapX, int mapY, unsigned value) {
    int key = getKey(mapX, mapY);
    if (effects[key]) {
        effects[key]->setIntensivity(value);
    } else {
        cerr << "Bad coordinates: " << mapX << " " << mapY << endl;
    }
}
void WebCamVJ::setMaxIntensivity(int mapX, int mapY, unsigned value) {
    int key = getKey(mapX, mapY);
    if (effects[key]) {
        effects[key]->setMaxIntensivity(value);
    } else {
        cerr << "Bad maxIntensity coordinates: " << mapX << " " << mapY << endl;
    }
}

int WebCamVJ::getKey(int mapX, int mapY) {
    RANGE(mapX, 0, MAP_MAX - 1);
    RANGE(mapY, 0, MAP_MAX - 1);
    return MAP_X(mapX) + MAP_Y(mapY);
}


/*
 * Destruction
 */

WebCamVJ::~WebCamVJ() {
    WebCamEffect* effect;
    for (unsigned i = 0; i < effects.size(); i++) {
        effect = effects[i];       
        if (effect) {
            delete effect;
        }
    }
    effects.clear();
    if (frame) {
        cvReleaseImage(&frame);
    }
    if (bigImage) {
        cvReleaseImage(&bigImage);
    }
}
