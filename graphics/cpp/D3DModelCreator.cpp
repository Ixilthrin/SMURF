#ifdef WIN32

#include "D3DModelCreator.h"

#include <windows.h>
#include <D3D9types.h>
#include "Vertex1.h"
#include "Vertex2.h"
#include "Vertex3.h"
#include "Vertex4.h"
#include "Vertex5.h"
#include "Vertex7.h"
#include "Vertex8.h"
#include "VertexBufferInfo.h"
#include "VertexBufferReference.h"
#include "IndexBufferInfo.h"
#include "IndexBufferReference.h"
#include "GraphicsSystem.h"
#include "Graphics.h"
#include "Mesh.h"
#include "ModelInfo.h"
//#include "SceneTexture.h"
#include "AnimationTest1.h"
#include "AnimationTest2.h"
#include "AnimationTest3.h"
#include "AnimationTest4.h"
#include "AnimationTest5.h"
#include "AnimationTest6.h"
#include "AnimationTest7.h"
#include "AnimationTest8.h"
#include "GeneralModel.h"
#include "ShaderTest1.h"
#include "TerrainSample1.h"
#include "util.h"

D3DModelCreator::D3DModelCreator()
{
}

Model *D3DModelCreator::createModel(ModelInfo *info)
{
    Model *model = 0;
    if (info->getFileName() == std::string("animation_test_1"))
    {
        model = new AnimationTest1();
    }
    else if (info->getFileName() == std::string("animation_test_2"))
    {
        model = new AnimationTest2();
    }
    else if (info->getFileName() == std::string("animation_test_3"))
    {
        model = new AnimationTest3();
    }
    else if (info->getFileName() == std::string("animation_test_4"))
    {
        model = new AnimationTest4();
    }
    else if (info->getFileName() == std::string("animation_test_5"))
    {
        model = new AnimationTest5();
    }
    else if (info->getFileName() == std::string("animation_test_6"))
    {
        model = new AnimationTest6();
    }
    else if (info->getFileName() == std::string("animation_test_7"))
    {
        model = new AnimationTest7();
    }
    else if (info->getFileName() == std::string("animation_test_8"))
    {
        model = new AnimationTest8();
    }
    else if (info->getFileName() == std::string("shader_test_1"))
    {
        model = new ShaderTest1();
    }
    else if (info->getFileName().substr(0, 13) == std::string("general_model"))
    {
        model = new GeneralModel();
    }
    else if (info->getFileName() == std::string("terrain_sample_1"))
    {
        model = new TerrainSample1();
    }
    else
    {
        model = new Model();
    }
    int vertexCount = info->vertexCount;
//    void *vertexData;                      this is unused
    void *indexData = info->indices;
    int indexCount = info->indexCount;

    // TODO: Lots of refactoring here

    //if (false)
    if (info->hasThirdTexture)
    {
        // Use Vertex8  
        model->setVertexType(VertexBufferInfo::VERTEX8);
        int colorValueCount = info->colorValueCount;
        //float *colorValues = info->colorValues;
        float *vertexPositions = info->vertexPositions;
        Vertex8 *vertex8Data = new Vertex8[vertexCount];
        float *textureCoordinates2 = info->textureCoords2;
        float *textureCoordinates3 = info->textureCoords3;
        float *normals = info->normals;
        for (int i = 0; i < vertexCount; ++i)
        {
            Vertex8 vertex;
            vertex.x = vertexPositions[i * 3];
            vertex.y = vertexPositions[i * 3 + 1];
            vertex.z = vertexPositions[i * 3 + 2];
            //int red = static_cast<int>(colorValues[i * 3] * 255.0f);
            //int green = static_cast<int>(colorValues[i * 3 + 1] * 255.0f);
            //int blue = static_cast<int>(colorValues[i * 3 + 2] * 255.0f);
            //unsigned int color = D3DCOLOR_ARGB(0, red, green, blue);
            //vertex.color = color;
            vertex.u2 = textureCoordinates2[i * 2];
            vertex.v2 = textureCoordinates2[i * 2 + 1];
            vertex.u3 = textureCoordinates3[i * 2];
            vertex.v3 = textureCoordinates3[i * 2 + 1];
            if (info->hasTexture)
            {
                vertex.u1 = info->textureCoords[i * 2];
                vertex.v1 = info->textureCoords[i * 2 + 1];
            }
            if (normals)
            {
                vertex.nx = info->normals[i * 3];
                vertex.ny = info->normals[i * 3 + 1];
                vertex.nz = info->normals[i * 3 + 2];
            }
            vertex8Data[i] = vertex;
        }

        //if (info->hasTexture)
        //{
        //    SceneTexture *sceneTexture = SceneTexture::load(info->textureFile);
        //    model->setTextureReference(sceneTexture->getTextureReference());
        //}

        VertexBufferInfo *vbInfo = new VertexBufferInfo(VertexBufferInfo::VERTEX8, vertexCount);
        vbInfo->setVertexData(vertex8Data);
        Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
        VertexBufferReference *vbRef = graphics->createVertexBuffer(vbInfo);

        IndexBufferInfo *ibInfo = new IndexBufferInfo(IndexBufferInfo::INT_16_BIT_INDICES, indexCount);
        ibInfo->setIndexData(indexData);
        IndexBufferReference *ibRef = graphics->createIndexBuffer(ibInfo);

        Mesh *mesh = new Mesh();
        mesh->setIndexBuffer(ibRef);
        mesh->setVertexBuffer(vbRef);
        model->setMesh(mesh);

        delete vbInfo;
        delete ibInfo;
    }
    else if (info->hasSecondTexture)
    {
        // Use Vertex7  
        model->setVertexType(VertexBufferInfo::VERTEX7);
        int colorValueCount = info->colorValueCount;
        float *colorValues = info->colorValues;
        float *vertexPositions = info->vertexPositions;
        Vertex7 *vertex7Data = new Vertex7[vertexCount];
        float *textureCoordinates2 = info->textureCoords2;
        float *normals = info->normals;
        for (int i = 0; i < vertexCount; ++i)
        {
            Vertex7 vertex;
            vertex.x = vertexPositions[i * 3];
            vertex.y = vertexPositions[i * 3 + 1];
            vertex.z = vertexPositions[i * 3 + 2];
            int red = static_cast<int>(colorValues[i * 3] * 255.0f);
            int green = static_cast<int>(colorValues[i * 3 + 1] * 255.0f);
            int blue = static_cast<int>(colorValues[i * 3 + 2] * 255.0f);
            unsigned int color = D3DCOLOR_ARGB(0, red, green, blue);
            vertex.color = color;
            vertex.u2 = textureCoordinates2[i * 2];
            vertex.v2 = textureCoordinates2[i * 2 + 1];
            if (info->hasTexture)
            {
                vertex.u1 = info->textureCoords[i * 2];
                vertex.v1 = info->textureCoords[i * 2 + 1];
            }
            if (normals)
            {
                vertex.nx = info->normals[i * 3];
                vertex.ny = info->normals[i * 3 + 1];
                vertex.nz = info->normals[i * 3 + 2];
            }
            vertex7Data[i] = vertex;
        }

        //if (info->hasTexture)
        //{
            //SceneTexture *sceneTexture = SceneTexture::load(info->textureFile);
            //model->setTextureReference(sceneTexture->getTextureReference());
        //}

        VertexBufferInfo *vbInfo = new VertexBufferInfo(VertexBufferInfo::VERTEX7, vertexCount);
        vbInfo->setVertexData(vertex7Data);
        Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
        VertexBufferReference *vbRef = graphics->createVertexBuffer(vbInfo);

        IndexBufferInfo *ibInfo = new IndexBufferInfo(IndexBufferInfo::INT_16_BIT_INDICES, indexCount);
        ibInfo->setIndexData(indexData);
        IndexBufferReference *ibRef = graphics->createIndexBuffer(ibInfo);

        Mesh *mesh = new Mesh();
        mesh->setIndexBuffer(ibRef);
        mesh->setVertexBuffer(vbRef);
        model->setMesh(mesh);

        delete vbInfo;
        delete ibInfo;
    }
    else if (!info->hasColorValues && info->hasTexture)
    {
        // Use Vertex5
        model->setVertexType(VertexBufferInfo::VERTEX5);
        float *vertexPositions = info->vertexPositions;
        float *texCoords = info->textureCoords;
        Vertex5 *vertex5Data = new Vertex5[vertexCount];
        float *normals = info->normals;
        for (int i = 0; i < vertexCount; ++i)
        {
            Vertex5 vertex;
            vertex.x = vertexPositions[i * 3];
            vertex.y = vertexPositions[i * 3 + 1];
            vertex.z = vertexPositions[i * 3 + 2];
            vertex.u = texCoords[i * 2];
            vertex.v = texCoords[i * 2 + 1];
            if (normals)
            {
                vertex.nx = info->normals[i * 3];
                vertex.ny = info->normals[i * 3 + 1];
                vertex.nz = info->normals[i * 3 + 2];
            }
            vertex5Data[i] = vertex;
        }
        VertexBufferInfo *vbInfo = new VertexBufferInfo(VertexBufferInfo::VERTEX5, vertexCount);
        vbInfo->setVertexData(vertex5Data);
        Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
        VertexBufferReference *vbRef = graphics->createVertexBuffer(vbInfo);

        IndexBufferInfo *ibInfo = new IndexBufferInfo(IndexBufferInfo::INT_16_BIT_INDICES, indexCount);
        ibInfo->setIndexData(indexData);
        IndexBufferReference *ibRef = graphics->createIndexBuffer(ibInfo);

        Mesh *mesh = new Mesh();
        mesh->setIndexBuffer(ibRef);
        mesh->setVertexBuffer(vbRef);
        model->setMesh(mesh);

        //SceneTexture *sceneTexture = SceneTexture::load(info->textureFile);
        //model->setTextureReference(sceneTexture->getTextureReference());

        delete vbInfo;
        delete ibInfo;
    }
    else if (info->hasColorValues && info->hasTexture)
    {  
        // Use Vertex6 (not supported by GLGraphics - this is one reason
        // we need D3D and GL specific Model creators)
        model->setVertexType(VertexBufferInfo::VERTEX6);
        int colorValueCount = info->colorValueCount;
        float *colorValues = info->colorValues;
        float *vertexPositions = info->vertexPositions;
        float *texCoords = info->textureCoords;
        Vertex6 *vertex6Data = new Vertex6[vertexCount];
        for (int i = 0; i < vertexCount; ++i)
        {
            Vertex6 vertex;
            vertex.x = vertexPositions[i * 3];
            vertex.y = vertexPositions[i * 3 + 1];
            vertex.z = vertexPositions[i * 3 + 2];
            int red = static_cast<int>(colorValues[i * 3] * 255.0f);
            int green = static_cast<int>(colorValues[i * 3 + 1] * 255.0f);
            int blue = static_cast<int>(colorValues[i * 3 + 2] * 255.0f);
            unsigned int color = D3DCOLOR_ARGB(0, red, green, blue);
            vertex.color = color;
            vertex.tu = texCoords[i * 2];
            vertex.tv = texCoords[i * 2 + 1];
            vertex6Data[i] = vertex;
        }
        VertexBufferInfo *vbInfo = new VertexBufferInfo(VertexBufferInfo::VERTEX6, vertexCount);
        vbInfo->setVertexData(vertex6Data);
        Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
        VertexBufferReference *vbRef = graphics->createVertexBuffer(vbInfo);

        IndexBufferInfo *ibInfo = new IndexBufferInfo(IndexBufferInfo::INT_16_BIT_INDICES, indexCount);
        ibInfo->setIndexData(indexData);
        IndexBufferReference *ibRef = graphics->createIndexBuffer(ibInfo);

        Mesh *mesh = new Mesh();
        mesh->setIndexBuffer(ibRef);
        mesh->setVertexBuffer(vbRef);
        model->setMesh(mesh);

        //SceneTexture *sceneTexture = SceneTexture::load(info->textureFile);
        //model->setTextureReference(sceneTexture->getTextureReference());

        delete vbInfo;
        delete ibInfo;
    }
    else if (info->hasColorValues)
    {   
        // Use Vertex3 (not supported by GLGraphics - this is one reason
        // we need D3D and GL specific Model creators)
        model->setVertexType(VertexBufferInfo::VERTEX3);
        int colorValueCount = info->colorValueCount;
        float *colorValues = info->colorValues;
        float *vertexPositions = info->vertexPositions;
        Vertex3 *vertex3Data = new Vertex3[vertexCount];
        for (int i = 0; i < vertexCount; ++i)
        {
            Vertex3 vertex;
            vertex.x = vertexPositions[i * 3];
            vertex.y = vertexPositions[i * 3 + 1];
            vertex.z = vertexPositions[i * 3 + 2];
            int red = static_cast<int>(colorValues[i * 3] * 255.0f);
            int green = static_cast<int>(colorValues[i * 3 + 1] * 255.0f);
            int blue = static_cast<int>(colorValues[i * 3 + 2] * 255.0f);
            unsigned int color = D3DCOLOR_ARGB(0, red, green, blue);
            vertex.color = color;
            vertex3Data[i] = vertex;
        }
        VertexBufferInfo *vbInfo = new VertexBufferInfo(VertexBufferInfo::VERTEX3, vertexCount);
        vbInfo->setVertexData(vertex3Data);
        Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
        VertexBufferReference *vbRef = graphics->createVertexBuffer(vbInfo);

        IndexBufferInfo *ibInfo = new IndexBufferInfo(IndexBufferInfo::INT_16_BIT_INDICES, indexCount);
        ibInfo->setIndexData(indexData);
        IndexBufferReference *ibRef = graphics->createIndexBuffer(ibInfo);

        Mesh *mesh = new Mesh();
        mesh->setIndexBuffer(ibRef);
        mesh->setVertexBuffer(vbRef);
        model->setMesh(mesh);

        delete vbInfo;
        delete ibInfo;
    }
    else
    {
        fatalError("Unsupported model information format.");
    }
    if (info->hasTexture)
    {
        model->setIsTextured(true);
    }
    model->setKeyframeAnimation(info->animation);
    return model;
}

#endif

