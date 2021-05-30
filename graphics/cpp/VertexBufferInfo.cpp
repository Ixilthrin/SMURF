#include "VertexBufferInfo.h"

VertexBufferInfo::VertexBufferInfo(int type, int count)
: vertexType(type), numberOfVertices(count), vertexData(0)
{
}

VertexBufferInfo::~VertexBufferInfo()
{
}

int VertexBufferInfo::getSizeOfVertex()
{
	switch (vertexType)
	{
	case VERTEX1:
		return sizeof(Vertex1);
	case VERTEX2:
		return sizeof(Vertex2);
	case VERTEX3:
		return sizeof(Vertex3);
	case VERTEX4:
		return sizeof(Vertex4);
    case VERTEX5:
        return sizeof(Vertex5);
    case VERTEX6:
        return sizeof(Vertex6);
    case VERTEX7:
        return sizeof(Vertex7);
    case VERTEX8:
        return sizeof(Vertex8);
	};
	return 0;
}
