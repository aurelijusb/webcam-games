#ifndef WEBCAMEFFECT_H
#define	WEBCAMEFFECT_H

#include <string>
#include <cv.h>
#define RANGE(variable, min, max) {if ((variable) < (min)) (variable) = (min); if ((variable > max)) (variable) = max; }

enum effectType {
    none,
    movie,
    image,
    imageSequence,
    rectangules,
    oneColor
};

/**
 * Adding effect to OpenCV image.
 */
class WebCamEffect {
public:
    WebCamEffect(effectType type = movie, const std::string file = "");
    ~WebCamEffect();
    void apply(IplImage *background);
    void setIntensivity(unsigned value);
    void setMaxIntensivity(unsigned value);
    void markControllAreas(IplImage *frame, int x, int y, int width,
                           int height);
protected:
    enum colorType {
        red,
        green,
        blue,
        white
    };
    
    void average(IplImage *what, IplImage *where, int intensivity);
    void rotate(IplImage* src, IplImage* dest, int angle, double factor = 1);
    void quadro(IplImage *src, IplImage* dest);
    void color(IplImage *dest, int intentsity, colorType type = blue);
    effectType type;
    int intensivity;
    int maxIntensivity;
    struct CvCapture *capture;
    IplImage *frame;
    int position;
    int nFrames;
    std::string extention;
    std::string directory;
    std::string zeroFill(int x, int width = 4);
};

#endif