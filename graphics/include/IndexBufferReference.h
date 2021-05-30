#ifndef GENG_INDEX_BUFFER_REFERENCE_H
#define GENG_INDEX_BUFFER_REFERENCE_H

class IndexBufferReference
{
public:
	virtual ~IndexBufferReference() {}
    virtual void activate() = 0;
	virtual void deactivate() = 0;
    virtual void destroy() = 0;
    virtual int getIndexCount() = 0;
};

#endif
