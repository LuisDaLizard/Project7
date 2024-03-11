#include "model.h"
#include "cyTriMesh.h"

Model::Model()
    : mMeshes(nullptr), mMaterials(nullptr), mNumMeshes(0)
{
}

Model::~Model()
{
    delete[] mMaterials;
    delete[] mMeshes;
}

bool Model::LoadFromFile(const char *fileName)
{
    cyTriMesh cyMesh;

    // get the path from filename
    char *directory = nullptr;
    char const *pathEnd = strrchr(fileName, '\\');
    if (!pathEnd) 
        pathEnd = strrchr(fileName, '/');
    if (pathEnd) 
    {
        int n = int(pathEnd - fileName) + 1;
        directory = new char[n + 1];
        strncpy(directory, fileName, n);
        directory[n] = '\0';
    }

    if (!cyMesh.LoadFromFileObj(fileName, true))
        return false;

    // Ensure model has normals
    if (!cyMesh.HasNormals())
        cyMesh.ComputeNormals();

    cyMesh.ComputeBoundingBox();

    cyVec3f boundMin = cyMesh.GetBoundMin();
    cyVec3f boundMax = cyMesh.GetBoundMax();

    mScale.x = boundMax.x - boundMin.x;
    mScale.y = boundMax.y - boundMin.y;
    mScale.z = boundMax.z - boundMin.z;

    mNumMeshes = (int)cyMesh.NM();

    if (!cyMesh.NM())
        mNumMeshes = 1;

    mMaterials = new Mesh::Material[mNumMeshes];
    mMeshes = new Mesh[mNumMeshes];

    if (cyMesh.NM())
    {
        for (int i = 0; i < mNumMeshes; i++)
        {
            int faceCount = cyMesh.GetMaterialFaceCount(i);
            int firstFace = cyMesh.GetMaterialFirstFace(i);

            auto *vertices = new Mesh::Vertex[faceCount * 3];

            for (int j = 0; j < faceCount; j++)
            {
                int fIndex = firstFace + j;
                int vIndex = j * 3;

                cyTriMesh::TriFace vFace = cyMesh.F(fIndex);

                vertices[vIndex + 0].position = cyMesh.V((int)vFace.v[0]);
                vertices[vIndex + 1].position = cyMesh.V((int)vFace.v[1]);
                vertices[vIndex + 2].position = cyMesh.V((int)vFace.v[2]);

                if (cyMesh.HasNormals())
                {
                    cyTriMesh::TriFace nFace = cyMesh.FN(fIndex);

                    vertices[vIndex + 0].normal = cyMesh.VN((int)nFace.v[0]);
                    vertices[vIndex + 1].normal = cyMesh.VN((int)nFace.v[1]);
                    vertices[vIndex + 2].normal = cyMesh.VN((int)nFace.v[2]);
                }

                if (cyMesh.HasTextureVertices())
                {
                    cyTriMesh::TriFace tFace = cyMesh.FT(fIndex);
                    vertices[vIndex + 0].texture = cyMesh.VT((int)tFace.v[0]).XY();
                    vertices[vIndex + 1].texture = cyMesh.VT((int)tFace.v[1]).XY();
                    vertices[vIndex + 2].texture = cyMesh.VT((int)tFace.v[2]).XY();
                }
            }

            mMeshes[i].Create(vertices, faceCount * 3);

            cyTriMesh::Mtl mat = cyMesh.M(i);

            // Convert float[3] to cyVec3f
            mMaterials[i].bAmbience = false;
            mMaterials[i].bDiffuse = false;
            mMaterials[i].bSpecular = false;
            mMaterials[i].kAmbience = *((cyVec3f *) mat.Ka);
            mMaterials[i].kDiffuse = *((cyVec3f *) mat.Kd);
            mMaterials[i].kSpecular = *((cyVec3f *) mat.Ks);
            mMaterials[i].kShininess = 20;

            if (mat.map_Kd)
            {
                char *diffusePath = new char[strlen(directory) + strlen(mat.map_Kd) + 1];
                memset(diffusePath, 0, strlen(directory) + strlen(mat.map_Kd) + 1);

                strncpy(diffusePath, directory, strlen(directory));
                strcat(diffusePath, mat.map_Kd);

                mMaterials[i].bDiffuse = true;
                mMaterials[i].tDiffuse = new Texture();
                mMaterials[i].tDiffuse->LoadFromFile(diffusePath);

                delete[] diffusePath;
            }

            if (mat.map_Ka)
            {
                char *ambiencePath = new char[strlen(directory) + strlen(mat.map_Ka) + 1];
                memset(ambiencePath, 0, strlen(directory) + strlen(mat.map_Ka) + 1);

                strncpy(ambiencePath, directory, strlen(directory));
                strcat(ambiencePath, mat.map_Ka);

                mMaterials[i].bAmbience = true;
                mMaterials[i].tAmbience = new Texture();
                mMaterials[i].tAmbience->LoadFromFile(ambiencePath);

                delete[] ambiencePath;
            }

            if (mat.map_Ks)
            {
                char *specularPath = new char[strlen(directory) + strlen(mat.map_Ks) + 1];
                memset(specularPath, 0, strlen(directory) + strlen(mat.map_Ks) + 1);

                strncpy(specularPath, directory, strlen(directory));
                strcat(specularPath, mat.map_Ks);

                mMaterials[i].bSpecular = true;
                mMaterials[i].tSpecular = new Texture();
                mMaterials[i].tSpecular->LoadFromFile(specularPath);

                delete[] specularPath;
            }

            delete[] vertices;
        }
    }
    else
    {
        int faceCount = cyMesh.NF();
        int firstFace = 0;

        auto *vertices = new Mesh::Vertex[faceCount * 3];

        for (int j = 0; j < faceCount; j++)
        {
            int fIndex = firstFace + j;
            int vIndex = j * 3;

            cyTriMesh::TriFace vFace = cyMesh.F(fIndex);

            vertices[vIndex + 0].position = cyMesh.V((int)vFace.v[0]);
            vertices[vIndex + 1].position = cyMesh.V((int)vFace.v[1]);
            vertices[vIndex + 2].position = cyMesh.V((int)vFace.v[2]);

            if (cyMesh.HasNormals())
            {
                cyTriMesh::TriFace nFace = cyMesh.FN(fIndex);

                vertices[vIndex + 0].normal = cyMesh.VN((int)nFace.v[0]);
                vertices[vIndex + 1].normal = cyMesh.VN((int)nFace.v[1]);
                vertices[vIndex + 2].normal = cyMesh.VN((int)nFace.v[2]);
            }

            if (cyMesh.HasTextureVertices())
            {
                cyTriMesh::TriFace tFace = cyMesh.FT(fIndex);
                vertices[vIndex + 0].texture = cyMesh.VT((int)tFace.v[0]).XY();
                vertices[vIndex + 1].texture = cyMesh.VT((int)tFace.v[1]).XY();
                vertices[vIndex + 2].texture = cyMesh.VT((int)tFace.v[2]).XY();
            }
        }

        mMeshes[0].Create(vertices, faceCount * 3);
        mMaterials[0].kAmbience = {1, 1, 1};
        mMaterials[0].kDiffuse = {1, 1, 1};
        mMaterials[0].kSpecular = {1, 1, 1};
        mMaterials[0].kShininess = 20;
        mMaterials[0].bAmbience = false;
        mMaterials[0].bDiffuse = false;
        mMaterials[0].bSpecular = false;

        delete[] vertices;
    }

    delete[] directory;

    return true;
}

void Model::Draw(Shader &shader, bool useMaterials)
{
    for (int i = 0; i < mNumMeshes; i++)
    {
        if (useMaterials)
            mMeshes[i].Draw(shader, mMaterials[i]);
        else
            mMeshes[i].Draw(shader);
    }
}

cyVec3f Model::GetSize()
{
    return mScale;
}
