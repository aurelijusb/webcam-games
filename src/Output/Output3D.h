#include <GL/gl.h>
#include <stdio.h>
#include <vector>
#include "../Objects3D/Objects3D.h"
#include "../Tracker/TrackerMotion.h"


#ifndef OUTPUT3D_H
#define	OUTPUT3D_H

#define RANGE_360(variable) { if (variable > 360) variable -= 360; if (variable < 0) variable += 360; }

using namespace std;
using namespace Objects3D;

/**
 * Loading, rendering and rotating 3D object based on camera motion.
 * 
 * Glut is used as OpenGl frontend. Cb based Glut event handlers does not
 * recognise class based methods, so singleton is used to connect handlers.
 */
class Output3D: public TrackerMotion {
public:
    Output3D(int width = 800, int height = 600);
    ~Output3D();
    void run();
    void render();
    void idle();
    bool onKeyPress(char c);
    static Output3D *singleton;
protected:
    void loop(const string &windowName, int wait);
private:
    void initialiseGlut();   
    void initialiseOpenGl();
    void initialiseOpenGlLights();
    GLfloat *lightAmbient;
    GLfloat *lightDiffuse;
    GLfloat *lightPosition;
    vector<Object3D*> models;
    vector<Material*> materials;
    void openGlPerspective(float fieldOfView, float aspectRation,
                           float nearPlane, float farPlane);
    void initialiseModel(const string directory, const string fileName);

    void setRotationX(float difference);
    void setRotationZ(float difference);
    void moveX(float difference);
    void moveY(float difference);
    void moveZ(float difference);
    float rotationX, rotationY, rotationZ;
    float translateX,translateY, translateZ;
    int windowWidth, windowHeight;
    int rzSpeed;
    int rxSpeed;
    
    void destructOpenGl();
    virtual void destruct();
};


#endif	/* OUTPUT3D_H */

