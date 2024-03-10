#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include <cyTriMesh.h>

class Model
{
private:
    Mesh *mMeshes;
    Mesh::Material *mMaterials;
    int mNumMeshes;

    cyVec3f mScale;

public:
    Model();
    ~Model();

    Model(const Model&) = delete;
    Model(Model&&) = delete;
    Model& operator=(const Model&) = delete;
    Model& operator=(Model&&) = delete;

    bool LoadFromFile(const char* modelDirectory);
    void Draw(Shader &shader, bool useMaterials);

    cyVec3f GetSize();
};

#endif //MODEL_H