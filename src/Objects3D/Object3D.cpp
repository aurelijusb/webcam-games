/*
 * Encapsulating OpenGl object.
 */

#include <iostream>
#include <GL/gl.h>

#include "Objects3D.h"

using namespace std;
using namespace Objects3D;

/**
 * 3D model
 * 
 * @param name                      name of object
 * @param vertixes                  vertix corrdinates. array[0..nv*3] = {x1, y1, z1, x2, y2, z2, ...} as array[vertex][coordinate]
 * @param nv                        number of different vertixes. (1 vertex = array(x, y, z) )
 * @param facesNormals              faces normals. array[0..nfn*3] = {x1, y1, z1, x2, y2, z2, ...}
 * @param nfn                       number of different face's norlmals. (1 face = array(x, y, z) )
 * @param triangles                 model trinangles. array[0..nf*3] = {I[0], I[1], I[2], ...} as array[face][element], where {I[0], I[1], I[2]} - triangle; I[i] index of vertex coordinate from vertixes array.
 * @param nf                        number of model trinagels (face per trinagle)
 * @param normals                   model faces' normals. array[0..nf] = {I[0], ...}, where I[i] index of normal from facesNormals array.
 * @param material                  material of model
 * @param freeVertixDataOnDestruct  if true, vertixes, facesNormals, faces and normals will be freed on object destruction
 * 
 *              triangle         } nf*3  ~~~~~  normal      } nf
 *              /   |   \                         |                     } vertex = trinagle[index]
 *         vertex vertex vertex  } nv         facesNormal   } nfn
 *         / | \  / | \  / | \                  / | \                   } x = vertex[index * 3 + 0]
 *         x y z  x y z  x y z   } nv*3         x y z       } nfn*3
 */
Object3D::Object3D(const string &name, float *vertixes, int nv, float *facesNormals, int nfn, int *triangles, int nf, int *normals, Material *material, bool freeVertixDataOnDestruct) {
    this->modelName = name;
    this->vertixes = vertixes;
    this->nv = nv;
    this->facesNormals = facesNormals;
    this->nfn = nfn;
    this->triangles = triangles;
    this->nf = nf;
    this->normals = normals;
    this->material = material;
}

const string &Object3D::name() {
    return modelName;
}

void Object3D::render() {
    if (material) {
        material->apply();
    }
    
    glBegin(GL_TRIANGLES);
    int vertexKey, normalsKey;
    for (int faceKey = 0; faceKey < nf; faceKey++) {
        if (nfn) {
            normalsKey = normals[faceKey];
            glNormal3fv(&facesNormals[normalsKey * 3]);
        }
        for (int i = 0; i < 3; i++) {
            vertexKey = triangles[faceKey * 3 + i];
            glVertex3fv(&vertixes[vertexKey * 3]);
        }
    }
    glEnd();
}

Object3D::~Object3D() {
    if (freeVertixDataOnDestruct) {
        delete  vertixes;
        delete  facesNormals;
        delete  triangles;
        delete  normals;
    }
}
