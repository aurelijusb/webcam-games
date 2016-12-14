#include <GL/glut.h>
#include <GL/gl.h>
#include <string.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "Output3D.h"
#include "Bubble.h"

using namespace std;

/*
 * Initialising
 */

Output3D::Output3D(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    rotationX = rotationY = rotationZ = 0;
    translateX = translateY = 0;
    translateZ = -6;
    rzSpeed = 0;
    rxSpeed = 0;
}

void Output3D::initialiseGlut() {
    char fakeParam[] = "fake";
    char *fakeargv[] = { fakeParam, NULL };
    int fakeargc = 1;
    glutInit(&fakeargc, fakeargv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(windowWidth,windowHeight);
    glutCreateWindow("3D viewer");
}

void Output3D::initialiseOpenGl() {
    glEnable(GL_DEPTH_TEST);
    initialiseOpenGlLights();
    initialiseModel("Data/", "m1.obj");
    initialiseModel("Data/", "m2.obj");
    initialiseModel("Data/", "m3.obj");
    initialiseModel("Data/", "m4.obj");

    glMatrixMode(GL_PROJECTION);
    openGlPerspective(45.0f, (GLfloat) windowWidth / (GLfloat) windowHeight,
                      0.8, 100);
    glMatrixMode(GL_MODELVIEW);
}


void Output3D::initialiseOpenGlLights() {
    lightAmbient = new GLfloat[4];
    lightAmbient[0] = lightAmbient[1] = lightAmbient[2] = lightAmbient[3] = 0.9;

    lightDiffuse = new GLfloat[4];
    lightDiffuse[0] = lightDiffuse[1] = lightDiffuse[2] = lightDiffuse[3] = 1;
    
    lightPosition = new GLfloat[4];
    lightPosition[0] = lightPosition[1] = 1;
    lightPosition[2] = 5;
    lightPosition[3] = 0;
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
}

void Output3D::initialiseModel(const string directory, const string fileName) {
    Objects3D::importFromObj(directory, fileName, models, materials);
}


/*
 * Rendering
 */

void Output3D::render() {
    glLoadIdentity();
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glClearColor(252 / 255.0f, 122 / 255.0f, 45 / 255.0f, 255 / 255.0f);

    glTranslatef(translateY, translateX, translateZ);
    glRotatef(rotationX, 1, 0, 0);
    glRotatef(rotationZ, 0, 0, 1);
    glScalef(2, 2, 2);
    for (unsigned i=0; i < models.size(); i++) {
        models[i]->render();
    }
    
    glutSwapBuffers();
}

void Output3D::openGlPerspective(float fieldOfView, float aspectRation,
                                 float nearPlane, float farPlane) {
    double left, right, bottom, top;
    top = tan (fieldOfView*M_PI/360.0)*nearPlane;
    bottom = -top;
    left = aspectRation*bottom;
    right = aspectRation*top;
    glFrustum (left, right, bottom, top, nearPlane, farPlane);
}


/*
 * 3D space transformation.
 */

void Output3D::setRotationX(float difference) {
    rotationX += difference;
    RANGE_360(rotationX);
}

void Output3D::setRotationZ(float difference) {
    rotationZ += difference;
    RANGE_360(rotationZ);
}
void Output3D::moveX(float difference) {
    translateX += difference;
}

void Output3D::moveY(float difference) {
    translateY += difference;
}

void Output3D::moveZ(float difference) {
    translateZ += difference;
}

void Output3D::idle() {
    if (TrackerMotion::needLoop()) {
        if (TrackerMotion::inLoop(1)) {
            showController();
        } else {
            destruct();
        }
    }
}


/*
 * Runing and events.
 */

Output3D *Output3D::singleton = NULL;

void renderSingleton() {
    Output3D::singleton->render();
}

void idleSingleton() {
    Output3D::singleton->idle();
}

void keySingleton(unsigned char key, int x, int y) {
    Output3D::singleton->onKeyPress(key);
}

void Output3D::run() {
    loop("Motion tracker", 1);
}

void Output3D::loop(const string &windowName, int wait) {
    initialiseGlut();
    initialiseOpenGl();
    Output3D::singleton = this;
    setFlip();
    glutDisplayFunc(renderSingleton);
    glutIdleFunc(idleSingleton);
    glutKeyboardFunc(keySingleton);
    showWindow(windowName, wait);
    glutMainLoop();
    TrackerMotion::distroyWindow(windowName);
}

bool Output3D::onKeyPress(char c) {
    switch (c) {
        case 27:
        case 'q':
            destruct();
            exit(0);
            return false;
    }
    
    int stepRotation = 16;
    
    setArrowProperties(255, 0, 255, 0, 0);
    bool horizontalTopRight = checkHorizontalLine(0, 1);   
    bool horizontalTopLeft = checkHorizontalLine(0, -1);
    
    
    setArrowProperties(255, 255, 0, 0, 0);
    bool verticalLeftDown = checkVerticalLine(0, 1);
    bool verticalLeftUp = checkVerticalLine(0, -1);
    
    
    if (horizontalTopRight) {
        rzSpeed -= stepRotation;
    } else if (horizontalTopLeft) {
        rzSpeed += stepRotation;
    }

    if (verticalLeftDown) {
        rxSpeed += stepRotation;
    } else if (verticalLeftUp) {
        rxSpeed -= stepRotation;
    }

    rxSpeed /= 2;
    rzSpeed /= 2;
    
    
    setRotationZ(rzSpeed);
    setRotationX(rxSpeed);
    render();
    
    showMap();
    
    return true;
}

/*
 * Destruction.
 */

void Output3D::destructOpenGl() {
    Object3D *model;
    while (!models.empty()) {
        model = models.back();
        delete model;
        models.pop_back();
    }
    Material *material;
    while (!materials.empty()) {
        material = materials.back();
        delete material;
        materials.pop_back();
    }
    delete lightAmbient;
    delete lightDiffuse;
}

void Output3D::destruct() {
    TrackerMotion::destruct();
    destructOpenGl();
}

Output3D::~Output3D() {
    destruct();
}