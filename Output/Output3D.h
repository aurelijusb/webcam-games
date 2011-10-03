//#include <GL/gl.h>
//#include <stdio.h>
//#include "../Objects3D/Objects3D.h"
//#include "../Tracker/TrackerMotion.h"
//#include <vector>
//
//#ifndef OUTPUT3D_H
//#define	OUTPUT3D_H
//
//#define RANGE_360(variable) { if (variable > 360) variable -= 360; if (variable < 0) variable += 360; }
//
//using namespace std;
//using namespace Objects3D;
//
//class Output3D: public TrackerMotion {
//public:
//    Output3D(int width = 800, int height = 600);
//    ~Output3D();
//    void render();
//    void setRotationX(float difference);
//    void setRotationZ(float difference);
//    void moveX(float difference);
//    void moveY(float difference);
//    void moveZ(float difference);
//    void run();
//    bool onKeyPress(char c);
//protected:
//    float rotationX, rotationY, rotationZ;
//    float translateX,translateY, translateZ;
//    int windowWidth, windowHeight;
//    
//    void *display;
//    void initialiseAllegro();
//    void destructAllegro();
//    
//    GLfloat *lightAmbient;
//    GLfloat *lightDiffuse;
//    GLfloat *lightPosition;
//    vector<Object3D*> models;
//    vector<Material*> materials;
//    void initialiseOpenGl();
//    void initialiseOpenGlLights();
//    void initialiseModel(const string directory, const string fileName);
//    void openGlPerspective(float fieldOfView, float aspectRation, float nearPlane, float farPlane);
//    void openGlRender();
//    void openGlRenderObject();
//    void destructOpenGl();
//
//    int rzSpeed;
//    int rxSpeed;
//private:
//    void readLn(FILE *pointer);
//};
//
//
//#endif	/* OUTPUT3D_H */
//
