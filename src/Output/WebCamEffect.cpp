#include <opencv/highgui.h>
#include <iostream>
#include "WebCamEffect.h"
#include <string>

#define CV_R( src, x, y ) (  (unsigned short) ( *((src)->imageData + (y) * (src)->widthStep + (x) * 3 + 2) & 0x00FF )  )
#define CV_G( src, x, y ) (  (unsigned short) ( *((src)->imageData + (y) * (src)->widthStep + (x) * 3 + 1) & 0x00FF )  )
#define CV_B( src, x, y ) (  (unsigned short) ( *((src)->imageData + (y) * (src)->widthStep + (x) * 3 + 3) & 0x00FF )  )

#define CV_RC( src, x, y ) ( *((src)->imageData + (y) * (src)->widthStep + (x) * 3 + 2) )
#define CV_GC( src, x, y ) ( *((src)->imageData + (y) * (src)->widthStep + (x) * 3 + 1) )
#define CV_BC( src, x, y ) ( *((src)->imageData + (y) * (src)->widthStep + (x) * 3 + 3) )

using namespace std;


WebCamEffect::WebCamEffect(effectType type, const std::string file) {
    this->type = type;
    intensivity = 0;
    nFrames = 0;
    capture = NULL;
    frame = NULL;
    maxIntensivity = -1;
    
    switch (type) {
        case movie:
            capture = cvCreateFileCapture(file.c_str());
            if (capture) {
                frame = cvQueryFrame(capture);
                nFrames = cvGetCaptureProperty(capture, 
                                               CV_CAP_PROP_FRAME_COUNT);
                position = 0;
            }    
            break;
        case image:
            frame = cvLoadImage(file.c_str());
            break;
        case imageSequence:
        {
            int slash = file.rfind('/');
            int dot = file.rfind('.');
            nFrames = atoi(file.substr(slash + 1, dot - slash - 1).c_str());
            extention = file.substr(dot);
            directory = file.substr(0, slash + 1);
            position = 1;
            string fullName = directory + zeroFill(position) + extention;
            frame = cvLoadImage(fullName.c_str());
            break;
        }
        case oneColor:
            extention = file;
        default:
            frame = NULL;
    }

}

void WebCamEffect::markControllAreas(IplImage *frame, int x, int y, int width,
                                     int height) {
    CvScalar color = CV_RGB(255, 255, 255);
    if (type == oneColor) {
        if (extention == "red") {
            color = CV_RGB(255, 0, 0);
        } else if (extention == "green") {
            color = CV_RGB(0, 255, 0);
        } else if (extention == "blue") {
            color = CV_RGB(0, 0, 255);
        }                
    }
    cvRectangle(frame, cvPoint(x, y), cvPoint(x + width, y + height), color, 1);
    if (type == rectangules) {
        cvLine(frame, cvPoint(x + width / 2, y),
                      cvPoint(x + width / 2, y + height), color);
        cvLine(frame, cvPoint(x, y + height / 2),
                      cvPoint(x + width, y + height / 2), color);
    }
}

void WebCamEffect::apply(IplImage *background) {
    
    if (maxIntensivity != -1 && intensivity != maxIntensivity) {
        int step = 5;
        if (intensivity < maxIntensivity) {
            intensivity += step;
            RANGE(intensivity, 0, maxIntensivity);
        } else {
            intensivity -= step;
            RANGE(intensivity, maxIntensivity, 255);
        }        
    }
    //TODO: tidy
    if (type == rectangules && intensivity > 20) {
        if (frame) {
            cvReleaseImage(&frame);
        }
        frame = cvCloneImage( background );
        quadro(frame, background);
    } else if (type == oneColor && intensivity > 20) {
        if (extention == "red") {
            color(background, intensivity, red);
        } else if (extention == "green") {
            color(background, intensivity, green);
        } else {
            color(background, intensivity);
        }
    } else {
        if (frame) {
            average(frame, background, intensivity);
        }
    }
    if (intensivity > 20) {
        if (type == imageSequence) {
            position++;
            string fullName = directory + zeroFill(position) + extention;
            cvReleaseImage(&frame);
            frame = cvLoadImage(fullName.c_str());
            if (position + 1 == nFrames) {
                position = 0;
            }
        } else {
            if (capture) {
                frame = cvQueryFrame(capture);
                if (position + 2 == nFrames) {
                    cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, 1);
                }
                position = cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);
            }
        }
    }
}

void WebCamEffect::average(IplImage* what, IplImage* where, int intensivity) {
    int minWidth = MIN(what->width, where->width);
    int minHeight = MIN(what->height, where->height);
    short antiIntensity = 255 - intensivity;
    uchar *ptr1, *ptr2;
    for(int y = 0; y < minHeight; y++ ) {
        ptr1 = (uchar*) (what->imageData + y * what->widthStep);
        ptr2 = (uchar*) (where->imageData + y * where->widthStep);
        for( int x = 0; x < minWidth; x++) {
            ptr2[3*x] = ptr2[3*x] * antiIntensity / 255 +
                        ptr1[3*x] * intensivity / 255;
            ptr2[3*x+1] = ptr2[3*x+1] * antiIntensity / 255 +
                          ptr1[3*x+1] * intensivity / 255;
            ptr2[3*x+2] = ptr2[3*x+2] * antiIntensity / 255 +
                          ptr1[3*x+2] * intensivity / 255;
        }
    }
}

void WebCamEffect::rotate(IplImage* src, IplImage* dest, int angle,
                          double factor) {
    float m[6];
    CvMat M = cvMat(2, 3, CV_32F, m);
    int w = src->width;
    int h = src->height;

    m[0] = (float)(factor*cos(-angle*CV_PI/180.));
    m[1] = (float)(factor*sin(-angle*CV_PI/180.));
    m[3] = -m[1];
    m[4] = m[0];
    m[2] = w*0.5f;  
    m[5] = h*0.5f;  

    cvGetQuadrangleSubPix( src, dest, &M);
}

void WebCamEffect::quadro(IplImage *src, IplImage* dest) {
    int xx, yy;
    int wHalf = dest->width / 2;
    int hHalf = dest->height / 2;
    for(int y = 0; y < dest->height; y++) {
        for( int x = 0; x < dest->width; x++) {
            if (y < hHalf && x < wHalf) {
                /* Left Top */
                xx = x * 2;
                yy = y * 2;
                CV_RC(dest, x, y) = CV_R(src, xx, yy);
                CV_GC(dest, x, y) = CV_G(src, xx, yy);
                CV_BC(dest, x, y) = CV_B(src, xx, yy);
            } else if (y < hHalf && x >= wHalf) {
                /* Right Top */
                xx = (dest->width - x) * 2 - 1;
                yy = y * 2;
                CV_RC(dest, x, y) = CV_R(src, xx, yy);
                CV_GC(dest, x, y) = CV_G(src, xx, yy);
                CV_BC(dest, x, y) = CV_B(src, xx, yy);
            } else if (y >= hHalf && x < wHalf) {
                /* Left Bottom */
                xx = x * 2;
                yy = (dest->height - y) * 2 - 1;
                CV_RC(dest, x, y) = CV_R(src, xx, yy);
                CV_GC(dest, x, y) = CV_G(src, xx, yy);
                CV_BC(dest, x, y) = CV_B(src, xx, yy);
            } else {
                /* Right Bottom */
                xx = (dest->width - x) * 2 - 1;
                yy = (dest->height - y) * 2 - 1;
                CV_RC(dest, x, y) = CV_R(src, xx, yy);
                CV_GC(dest, x, y) = CV_G(src, xx, yy);
                CV_BC(dest, x, y) = CV_B(src, xx, yy);
            }
        }
    }
}

//TODO: enum
void WebCamEffect::color(IplImage* dest, int intentsity, colorType type) {
    for(int y = 0; y < dest->height; y++) {
        for( int x = 0; x < dest->width; x++) {
            switch (type) {
                case red:
                    if (CV_R(dest, x, y) < intentsity) {
                        CV_RC(dest, x, y) = intentsity;
                    }
                    break;
                case green:
                    if (CV_G(dest, x, y) < intentsity) {
                        CV_GC(dest, x, y) = intentsity;
                    }
                    break;
                case white:
                    if (CV_R(dest, x, y) < intentsity) {
                        CV_RC(dest, x, y) = intentsity;
                    }
                    if (CV_G(dest, x, y) < intentsity) {
                        CV_GC(dest, x, y) = intentsity;
                    }
                    if (CV_B(dest, x, y) < intentsity) {
                        CV_BC(dest, x, y) = intentsity;
                    }
                    break;
                default:
                    if (CV_B(dest, x, y) < intentsity) {
                        CV_BC(dest, x, y) = intentsity;
                    }
            }
        }
    }
}

void WebCamEffect::setIntensivity(unsigned value) {
    RANGE(value, 0, 255);
    intensivity = value;
}

void WebCamEffect::setMaxIntensivity(unsigned value) {
    RANGE(value, 0, 255);
    maxIntensivity = value;
}

std::string WebCamEffect::zeroFill(int x, int width) {
   std::ostringstream o, zeros;
   int xOrg = x;
   for (int i = 0; i < width; i++) {
       if (x < 9) {
           zeros << '0';
       } else {
           x /= 10;
       }
   }
   o << zeros.str() << xOrg;
   return o.str(); 
}

WebCamEffect::~WebCamEffect() {
    if (frame) {
        cvReleaseImage(&frame);
    }
    if (capture) {
        cvReleaseCapture(&capture);
    }
}
