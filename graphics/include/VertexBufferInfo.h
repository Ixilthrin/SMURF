#ifndef GENG_VERTEX_BUFFER_INFO_H
#define GENG_VERTEX_BUFFER_INFO_H

#include "Vertex1.h"
#include "Vertex2.h"
#include "Vertex3.h"
#include "Vertex4.h"
#include "Vertex5.h"
#include "Vertex6.h"
#include "Vertex7.h"
#include "Vertex8.h"

class VertexBufferInfo
{
public:
    VertexBufferInfo(int type, int count);
	~VertexBufferInfo();
	int getNumberOfVertices() { return numberOfVertices; }
	int getSizeOfVertex();
	void *getVertexData() { return vertexData; }
	void setVertexData(void *data) { vertexData = data; }
	int getVertexType() { return vertexType; }
    enum {
        VERTEX1,
        VERTEX2,
		VERTEX3,
		VERTEX4,
        VERTEX5,
        VERTEX6,
        VERTEX7,
        VERTEX8
    };
private:
	int vertexType;
	int numberOfVertices;
	void *vertexData;
};

#endif