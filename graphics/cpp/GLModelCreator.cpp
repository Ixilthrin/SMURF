#include "GLModelCreator.h"

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
#include "Graphics.h"
#include "GraphicsSystem.h"
#include "IndexBufferInfo.h"
#include "IndexBufferReference.h"
#include "Mesh.h"
#include "stdlib.h"
#include "VertexBufferInfo.h"
#include "VertexBufferReference.h"
#include "Vertex1.h"
#include "Vertex2.h"
#include "Vertex3.h"
#include "Vertex4.h"
#include "Vertex5.h"
#include "util.h"

GLModelCreator::GLModelCreator()
{
}

Model *GLModelCreator::createModel(ModelInfo *info)
{
    if (false)
    {
        return createTestTriangle();
    }
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
    unsigned short *indexData = info->indices;
    int indexCount = info->indexCount;
    if (info->hasThirdTexture)
    {
        // Use Vertex8  
        model->setVertexType(VertexBufferInfo::VERTEX8);
        
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
        fatalError("Animation using 2nd texture not implemented for OpenGL");
    }
    else if (!info->hasColorValues && info->hasTexture)
    {
        // Use Vertex5
        model->setVertexType(VertexBufferInfo::VERTEX5);
        float *vertexPositions = info->vertexPositions;
        float *texCoords = info->textureCoords;
        float *normals = info->normals;
        Vertex5 *vertex5Data = new Vertex5[vertexCount];
        for (int i = 0; i < vertexCount; ++i)
        {
            Vertex5 vertex;
            vertex.x = vertexPositions[i * 3];
            vertex.y = vertexPositions[i * 3 + 1];
            vertex.z = vertexPositions[i * 3 + 2];
            vertex.u = texCoords[i * 2];
            vertex.v = texCoords[i * 2 + 1];
            if (info->hasNormals)
            {
                vertex.nx = normals[i * 3];
                vertex.ny = normals[i * 3 + 1];
                vertex.nz = normals[i * 3 + 2];
            }
            else
            {
                vertex.nx = 0;
                vertex.ny = 1;
                vertex.nz = 0;
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
        // TODO: Currently ignoring color values - need a new Vertex class for this
        model->setVertexType(VertexBufferInfo::VERTEX1);
        float *vertexPositions = info->vertexPositions;
        float *texCoords = info->textureCoords;
        Vertex1 *vertex1Data = new Vertex1[vertexCount];
        for (int i = 0; i < vertexCount; ++i)
        {
            Vertex1 vertex;
            vertex.x = vertexPositions[i * 3];
            vertex.y = vertexPositions[i * 3 + 1];
            vertex.z = vertexPositions[i * 3 + 2];
            vertex.u = texCoords[i * 2];
            vertex.v = texCoords[i * 2 + 1];
            vertex1Data[i] = vertex;
        }
        VertexBufferInfo *vbInfo = new VertexBufferInfo(VertexBufferInfo::VERTEX1, vertexCount);
        vbInfo->setVertexData(vertex1Data);
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
        // Use Vertex2 (not supported by D3DGraphics - this is one reason
        // we need D3D and GL specific Model creators)
        model->setVertexType(VertexBufferInfo::VERTEX2);
        
        float *colorValues = info->colorValues;
        float *vertexPositions = info->vertexPositions;
        Vertex2 *vertex2Data = new Vertex2[vertexCount];
        for (int i = 0; i < vertexCount; ++i)
        {
            Vertex2 vertex;
            vertex.x = -vertexPositions[i * 3];
            vertex.y = vertexPositions[i * 3 + 1];
            vertex.z = vertexPositions[i * 3 + 2];
            vertex.r = colorValues[i * 3];
            vertex.g = colorValues[i * 3 + 1];
            vertex.b = colorValues[i * 3 + 2];
            vertex.a = 1;  // If alpha is 0, nothing is drawn.
            vertex2Data[i] = vertex;
        }
        VertexBufferInfo *vbInfo = new VertexBufferInfo(VertexBufferInfo::VERTEX2, vertexCount);
        vbInfo->setVertexData(vertex2Data);
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
    
    model->setKeyframeAnimation(info->animation);
    return model;
}

Model *GLModelCreator::createTestTriangle()
{
    Model *model = new Model();
    
    
    int vertexCount = 40;
    int indexCount = 100;
    Vertex2 *vertices = new Vertex2[vertexCount];
    for (int i = 0; i < vertexCount; ++i)
    {
    vertices[i].x = static_cast<float>((rand() % 10) - 5);
    vertices[i].y = static_cast<float>((rand() % 10) - 5);
    vertices[i].z = static_cast<float>((rand() % 10) - 5);
    vertices[i].r = (float)(rand() % 10) / 10.0f;
    vertices[i].g = (float)(rand() % 10) / 10.0f;
    vertices[i].b = (float)(rand() % 10) / 10.0f;
    vertices[i].a = 1.0;
    }
    unsigned short *indices = new unsigned short[indexCount];
    for (int j = 0; j < indexCount; ++j)
    {
        indices[j] = rand() % vertexCount;
    }
    VertexBufferInfo *vbInfo = new VertexBufferInfo(VertexBufferInfo::VERTEX2, vertexCount);
    vbInfo->setVertexData(vertices);
    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    VertexBufferReference *vbRef = graphics->createVertexBuffer(vbInfo);

    IndexBufferInfo *ibInfo = new IndexBufferInfo(IndexBufferInfo::INT_16_BIT_INDICES, indexCount);
    ibInfo->setIndexData(indices);
    IndexBufferReference *ibRef = graphics->createIndexBuffer(ibInfo);

    Mesh *mesh = new Mesh();
    mesh->setIndexBuffer(ibRef);
    mesh->setVertexBuffer(vbRef);
    model->setMesh(mesh);


    delete [] vertices;
    delete [] indices;

    return model;
}
