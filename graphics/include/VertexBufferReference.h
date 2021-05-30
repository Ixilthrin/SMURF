#ifndef GENG_VERTEX_BUFFER_REFERENCE_H
#define GENG_VERTEX_BUFFER_REFERENCE_H

class VertexBufferReference
{
public:
	virtual ~VertexBufferReference() {};
	virtual void writeData(void *data, int dataSize)= 0;
    virtual void activate(bool picking) = 0;
	virtual void deactivate() = 0;
    virtual void destroy() = 0;
};

#endif