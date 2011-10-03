#include <highgui.h>
#include "WebCamVJ.h"
#include "../Tracker/TrackerMotion.h"
WebCamVJ::WebCamVJ() {
    for (int i=0; i < MAP_MAX * MAP_MAX; i++) {
        effects.push_back(NULL);
    }
    frame = 0;
    bigImage = cvCreateImage(cvSize(1280, 960), 8, 3);
}

void WebCamVJ::show(IplImage *background) {
    /* About boxes */
    int edgeWidth = background->width / MAP_MAX;
    int edgeHeight = background->height / MAP_MAX;
    for (int x = 0; x < MAP_MAX; x++) {
        for (int y = 0; y < MAP_MAX; y++) {
            int i = MAP_X(x) + MAP_Y(y);
            if (effects[i]) {
                effects[i]->aboutBox(background, x * edgeWidth, y * edgeHeight, edgeWidth, edgeHeight);
            }
        }
    }
    
    /* Efects */
    if (!frame) {
        frame = cvCloneImage(background);
    }
    cvCopy(background, frame);
//    cvZero(frame);
    for (unsigned i = 0; i < effects.size(); i++) {
        if (effects[i]) {
            effects[i]->apply(frame);
        }
    }
//    cvShowImage("VJ", frame);
    showBig();
}

void WebCamVJ::showBig() {
    cvZero(bigImage);
    cvResize(frame, bigImage, CV_INTER_LINEAR);
    cvNamedWindow("Nieko bendro", CV_WINDOW_NORMAL);
    cvSetWindowProperty("Nieko bendro", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    cvShowImage("Nieko bendro", bigImage);
    
//    GdkWindow *w = gtk_widget_get_parent_window(cvGetWindowHandle("didelė"));
//    gdk_window_fullscreen(w); 
    
    //TODO: unclutter -display :0.0 -idle 1
}

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

#define INTESITY(x, y) { setIntensivity(x, y, getIntensity(x, y)); }
#define MAX_INTESITY(x, y) { setMaxIntensivity(x, y, getIntensity(x, y)); }

void WebCamVJ::run() {
    setFlip();
    add(2, 3, image, "Data/23.jpg");
    add(5, 3, image, "Data/53.jpg");   
    
//    add(0, 0, imageSequence, "Data/Sequesnce/muziejus/00089.jpg");
    
    add(3, 2, image, "Data/remejams.jpg");
    add(4, 2, image, "Data/remejams.jpg");
    
    add(1, 4, image, "Data/14.jpg");
    add(6, 4, image, "Data/64.jpg");
    
    add(0, 5, image, "Data/05.jpg");
    add(7, 5, image, "Data/75.jpg");

    cvNamedWindow("Motion tracker", CV_WINDOW_NORMAL);
    cvSetWindowProperty("Motion tracker", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    showWindow("Motion tracker");
}

bool WebCamVJ::onKeyPress(char c) {
    switch (c) {
        case 27:
            return false;
    }
    
//    setArrowProperties(0, 255, 0, 0, -5);
//    bool centerRight = checkHorizontalLine(3, 1, 4, 8);
//    bool centerLeft = checkHorizontalLine(3, -1, 0, 4);
//    bool rightCenter = checkHorizontalLine(3, -1, 4, 8);
//    bool leftCenter = checkHorizontalLine(3, 1, 0, 4);
    
//    if (centerLeft && centerRight) {
//        setMaxIntensivity(2, 3, 128);
//        setMaxIntensivity(5, 3, 0);
//    }
//
//    if (leftCenter && rightCenter) {
//        setMaxIntensivity(2, 3, 0);
        setMaxIntensivity(2, 3, 255);
        setMaxIntensivity(5, 3, 255);
        setMaxIntensivity(3, 2, 255);
        setMaxIntensivity(4, 2, 255);
        setMaxIntensivity(1, 4, 255);
        setMaxIntensivity(6, 4, 255);
        setMaxIntensivity(0, 5, 255);
        setMaxIntensivity(7, 5, 255);
        
//    }
    
    INTESITY(2, 3);
    INTESITY(5, 3);

//    setMaxIntensivity(0, 0, 128);    

    INTESITY(3, 2);
    INTESITY(4, 2);
    
    INTESITY(1, 4);
    INTESITY(6, 4);
////    
    INTESITY(0, 5);
    INTESITY(7, 5);
    
//    INTESITY(0, 5);
//    INTESITY(7, 5);
//    
//    MAX_INTESITY(0, 0);
//    MAX_INTESITY(3, 0);
//    MAX_INTESITY(6, 0);
    
//    if (rightCenter) {
//        vj.setIntensivity(4, 4, 255);
//    }
//    
//    INTESITY(1, 1);
//    INTESITY(4, 1);
//    INTESITY(4, 4);
//    INTESITY(4, 6);
//    INTESITY(6, 1);
//    INTESITY(6, 4);
//    INTESITY(6, 6);
//    INTESITY(4, 3);
//    
    
    show(getFrame());
    
    return true;
}