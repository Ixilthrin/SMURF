#ifndef GENG_INDEX_BUFFER_INFO_H
#define GENG_INDEX_BUFFER_INFO_H

class IndexBufferInfo
{
public:
    IndexBufferInfo(int type, int indexCount);
	~IndexBufferInfo();
	int getNumberOfIndices() { return numberOfIndices; }
	void *getIndexData() { return indexData; }
	void setIndexData(void *data) { indexData = data; }
    int getIndexType() { return indexType; }
    enum 
    {
        INT_16_BIT_INDICES,
        INT_32_BIT_INDICES
    };
private:
	int numberOfIndices;
	void *indexData;
    int indexType;
};

#endif
