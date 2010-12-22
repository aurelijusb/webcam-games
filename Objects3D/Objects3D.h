#ifndef OBJECTS3D_H
#define	OBJECTS3D_H

#include <string>
#include <vector>

using namespace std;

#define RANGE(variable, min, max) {if ((variable) < (min)) (variable) = (min); if ((variable > max)) (variable) = max; }

namespace Objects3D {
    enum applyType {
        FRONT,
        BACK,
        FRONT_AND_BACK
    };
    
    class Material {
        public:
            Material(const string &name, float ambient[], float diffuse[], float specular[], float emition[], float shininess = 0);
            const string &name();
            const void apply(applyType type = FRONT_AND_BACK);
            ~Material();
            
            static const Material *getMaterialByName(const vector<Material*> &materialStorage, const string &name);
        protected:
            string materialName;
            float ambient[4];
            float diffuse[4];
            float specular[4];
            float emition[4];
            float shininess;
            //TODO texture
    };
    
    class Object3D {
        public:
            Object3D(const string &name, float *vertixes, int nv, float *facesNormals, int nfn, int *triangles, int nf, int *normals, Material *material, bool freeVertixDataOnDestruct = true);
            const string &name();
            void render();
            ~Object3D();
        protected:
            string modelName;
            float *vertixes;
            int nv;
            float *facesNormals;
            int nfn;
            int *triangles;
            int nf;
            int *normals;
            Material *material;
            bool freeVertixDataOnDestruct;
    };
    
    void importFromObj(const string directory, const string fileName, vector<Object3D*> &objectStorage, vector<Material*> &materialStorage);
    void importFromMtl(const string directory, const string fileName, vector<Material*> &materialStorage);
    Material *getMaterialByName(const vector<Material*> &materialStorage, const string &name);
}

#endif	/* OBJECTS3D_H */

