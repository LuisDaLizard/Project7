#ifndef MESH_H
#define MESH_H

#include <cyVector.h>

#include "texture.h"
#include "shader.h"

class Mesh
{
public:
    struct Vertex
    {
        cyVec3f position;
        cyVec3f normal;
        cyVec2f texture;
    };

    struct Material
    {
        cyVec3f kAmbience;
        cyVec3f kDiffuse;
        cyVec3f kSpecular;
        float   kShininess;

        bool    bDiffuse;
        Texture *tDiffuse;

        bool    bAmbience;
        Texture *tAmbience;

        bool    bSpecular;
        Texture *tSpecular;
    };

private:
    unsigned int mVAO;
    unsigned int mVBO;
    int mNumVertices;

public:
    Mesh();
    ~Mesh();

    void Create(const Vertex *vertices, int numVertices);
    void Draw(Shader &shader, Material &material) const;
    void Draw(Shader &shader) const;
};

namespace Meshes
{
    const Mesh::Vertex PlaneMeshVertices[] =
            {
                    {{-1, 1, 0}, {0, 0, -1}, {0, 1}},
                    {{1, 1, 0}, {0, 0, -1}, {1, 1}},
                    {{1, -1, 0}, {0, 0, -1}, {1, 0}},

                    {{1, -1, 0}, {0, 0, -1}, {1, 0}},
                    {{-1, -1, 0}, {0, 0, -1}, {0, 0}},
                    {{-1, 1, 0}, {0, 0, -1}, {0, 1}}
            };

}

#endif //MESH_H