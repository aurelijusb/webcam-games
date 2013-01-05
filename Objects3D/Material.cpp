/*
 * Encapsulating OpenGl material.
 */

#include <GL/gl.h>
#include "Objects3D.h"

using namespace Objects3D;

Material::Material(const string &name, float ambient[], float diffuse[],
                   float specular[], float emition[], float shininess) {
    this->materialName = name;
    for(int i = 0; i < 4; i++) {
        this->ambient[i] = ambient[i];
        this->diffuse[i] = diffuse[i];
        this->specular[i] = specular[i];
        this->emition[i] = emition[i];
        RANGE(ambient[i], 0, 1);
        RANGE(diffuse[i], 0, 1);
        RANGE(specular[i], 0, 1);
        RANGE(emition[i], 0, 1);
    }
    this->shininess = shininess;
    RANGE(shininess, 0, 128);
}

const void Material::apply(applyType type) {
    GLenum face;
    switch (type) {
        case FRONT:
            face = GL_FRONT;
            break;
        case BACK:
            face = GL_BACK;
            break;
        default:
            face = GL_FRONT_AND_BACK;
    }
    glMaterialfv(face, GL_AMBIENT, ambient);
    glMaterialfv(face, GL_DIFFUSE, diffuse);
    glMaterialfv(face, GL_EMISSION, emition);
    glMaterialf(face, GL_SHININESS, shininess);
}

const string &Material::name() {
    return materialName;
}



Material::~Material() {
    
}