#ifndef GENG_GUI_IMAGE_H
#define GENG_GUI_IMAGE_H

#include <string>
#include "GuiOverlay.h"
#include "VertexBufferReference.h"
#include "IndexBufferReference.h"

class GuiTexture;

class GuiImage : public GuiOverlay
{
public:
    GuiImage(
		int guiOrientation, 
		int offsetX, 
		int offsetY,
   		const std::string & imageFileName);
	virtual ~GuiImage();
    virtual void update();
	virtual void draw();
    virtual bool pick(int x, int y);
    void setIndexBufferReference(IndexBufferReference *ref) { indexBuffer = ref; }
    void setVertexBufferReference(VertexBufferReference *ref) { vertexBuffer = ref; }
    float getTextureCoordUMax() { return textureCoordUMax; }
    float getTextureCoordVMax() { return textureCoordVMax; }

private:
	GuiTexture * guiTexture;
    VertexBufferReference *vertexBuffer;
    IndexBufferReference *indexBuffer;
    float textureCoordUMax;  // Used when adding padding to image. ( = 1.0 if no padding)
    float textureCoordVMax;  // Used when adding padding to image. ( = 1.0 if no padding)
};

#endif
