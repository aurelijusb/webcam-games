#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>
#include <GL/gl.h>
#include <allegro5/display.h>
#include <string.h>
#include <string>
#include <math.h>
#include <iostream>

#include "Output3D.h"

#include <iostream>
#include <math.h>

using namespace std;

Output3D::Output3D(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    rotationX = rotationY = rotationZ = 0;
    translateX = translateY = 0;
    translateZ = -6;
    initialiseAllegro();
    initialiseOpenGl();
    
    rzSpeed = 0;
    rxSpeed = 0;
}


void Output3D::initialiseAllegro() {
    al_init();
    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL);
    display = al_create_display(windowWidth, windowHeight);
}

void Output3D::destructAllegro() {
    al_destroy_display((ALLEGRO_DISPLAY *) display);
}

void Output3D::initialiseOpenGl() {
    glEnable(GL_DEPTH_TEST);
    initialiseOpenGlLights();
    //FIXME: name
    initialiseModel("Data/", "untitled.obj");
}


void Output3D::initialiseOpenGlLights() {
    lightAmbient = new GLfloat[4];
    lightAmbient[0] = lightAmbient[1] = lightAmbient[2] = lightAmbient[3] = 0;
    
    lightDiffuse = new GLfloat[4];
    lightDiffuse[0] = lightDiffuse[1] = lightDiffuse[2] = lightDiffuse[3] = 1;
    
    lightPosition = new GLfloat[4];
    lightPosition[0] = lightPosition[1] = lightPosition[2] = 1;
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


void Output3D::render() {
    openGlRender();
}

void Output3D::openGlRender() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    openGlPerspective(45.0f, (GLfloat) windowWidth / (GLfloat) windowHeight, 0.1, 100);
    
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
    
    glTranslatef(translateX, translateY, translateZ);
    glRotatef(rotationX, 1, 0, 0);
    glRotatef(rotationZ, 0, 0, 1);
    openGlRenderObject();
    
    glFlush();
    al_flip_display();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Output3D::openGlPerspective(float fieldOfView, float aspectRation, float nearPlane, float farPlane) {
    double left, right, bottom, top;
    top = tan (fieldOfView*M_PI/360.0)*nearPlane;
    bottom = -top;
    left = aspectRation*bottom;
    right = aspectRation*top;
    glFrustum (left, right, bottom, top, nearPlane, farPlane);
}

void Output3D::openGlRenderObject() {
    for (unsigned i=0; i < models.size(); i++) {
        models[i]->render();
    }
}


void Output3D::destructOpenGl() {
    Object3D *model;
    while (!models.empty()) {
        model = models.back();
        cout << "FREEING: " << model->name() << endl;
        delete model;
        models.pop_back();
    }
    Material *material;
    while (!materials.empty()) {
        material = materials.back();
        cout << "FREEING: " << material->name() << endl;
        delete material;
        materials.pop_back();
    }
    delete lightAmbient;
    delete lightDiffuse;
}

Output3D::~Output3D() {
    destructOpenGl();
    destructAllegro();
}

void Output3D::run() {
    setFlip();
    showWindow("Motion tracker");
}

bool Output3D::onKeyPress(char c) {
    switch (c) {
        case 27:
            return false;
    }
    
    int stepRotation = 16;
    float stepMovementZ = 0.2;
    float stepMovementX = 0.2;
    
    
    setArrowProperties(255, 0, 255, 0, 0);
    bool horizontalTopRight = checkHorizontalLine(0, 1);   
    bool horizontalTopLeft = checkHorizontalLine(0, -1);
    
    
    setArrowProperties(255, 255, 0, 0, 0);
    bool verticalLeftDown = checkVerticalLine(0, 1);
    bool verticalLeftUp = checkVerticalLine(0, -1);
    
    setArrowProperties(0, 255, 0, 0, -5);
    bool centerRight = checkHorizontalLine(3, 1, 4, 8);
    bool centerLeft = checkHorizontalLine(3, -1, 0, 4);
    bool rightCenter = checkHorizontalLine(3, -1, 4, 8);
    bool leftCenter = checkHorizontalLine(3, 1, 0, 4);
    
    setArrowProperties(100, 255, 0, 0, 5);
    bool leftCenterRight = checkHorizontalLine(3, 1);
    bool rightCenterLeft = checkHorizontalLine(3, -1);
    
    setArrowProperties(0, 255, 100, -5, 0);
    bool topCenterBottom = checkVerticalLine(4, 1);
    bool bottomCenterTop = checkVerticalLine(4, -1);
    
    
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

    if (rightCenterLeft) {
        moveX(-stepMovementX);
    } else if (leftCenterRight) {
        moveX(stepMovementX);
    }
    
    if (leftCenter && rightCenter) {
        moveZ(-stepMovementZ);
    } else if (centerLeft && centerRight) {
        moveZ(stepMovementZ);
    }

    if (bottomCenterTop) {
        moveY(stepMovementX);
    } else if (topCenterBottom) {
        moveY(-stepMovementX);
    }


    rxSpeed /= 2;
    rzSpeed /= 2;
    
    
    setRotationZ(rzSpeed);
    setRotationX(rxSpeed);
    render();
    
    return true;
}
