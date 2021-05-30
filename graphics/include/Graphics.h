#ifndef GENG_GRAPHICS_H
#define GENG_GRAPHICS_H

#define BUFFER_OFFSET(i) ((char *)0 +(i))

#include <string>
#include "ScreenCoords.h"
#include "ScreenPosition.h"

class IndexBufferInfo;
class IndexBufferReference;
class TextureReference;
class TextureInfo;
class VertexBufferInfo;
class VertexBufferReference;
class ModelCreator;
class GuiImageCreator;
class FontGeometry;
class ShaderInfo;
class ShaderReference;

class Graphics
{
public:
    // Error checking
	virtual void checkError(const std::string & message = std::string()) = 0;
	virtual void clearError(unsigned int errorCodeToClear) = 0;
    
    // Window functions
    virtual void initialize() = 0;  // Tasks performed after window creation.
    virtual void setBackgroundColor(float r, float g, float b, float a) = 0;
	virtual void clearDepthAndScreenBuffers() = 0;
    virtual int getScreenWidth() = 0;
    virtual void setScreenWidth(int width) = 0;
    virtual void setScreenHeight(int height) = 0;
    virtual void setFullScreen(bool useFullScreen) = 0;
    virtual int getScreenHeight() = 0;
    virtual bool isFullScreen() = 0;
    virtual void setViewPort(int x, int y, int width, int height) = 0;
    virtual ScreenCoords getAbsoluteScreenCoords(int guiOrientation, int offsetX, 
                                                 int offsetY, int width, int height) = 0;
    virtual ScreenPosition getAbsoluteScreenPosition(int x, int y) = 0;
    virtual bool ScreenCoordsContainsPosition(ScreenCoords coords, ScreenPosition pos) = 0;
    virtual void readFromBackBuffer(int x, int y, int width, int height, unsigned char *pixels) = 0;
	virtual void readFromDepthBuffer(int x, int y, int width, int height, void *pixelData) {}
    
    // Graphics object creation
    virtual TextureReference *createTexture(TextureInfo *info) = 0;
    virtual VertexBufferReference *createVertexBuffer(VertexBufferInfo *info) = 0;
    virtual IndexBufferReference *createIndexBuffer(IndexBufferInfo *info) = 0;
    virtual ShaderReference *createShader(ShaderInfo *info) = 0;
    
    // Graphics attributes
    virtual void enableMode(int mode) = 0;
    virtual void disableMode(int mode) = 0;
    virtual void setAttributesFor3DPicking() = 0;
    virtual void unsetAttributesFor3DPicking() = 0;
    virtual void setColor(float r, float g, float b, float a) = 0;
    virtual void setColor(unsigned int color) = 0;
    
    // Scene transformation functions
    virtual void setPerspectiveProjection(float fovy, float aspect, float zNear, float zFar) = 0;
    virtual void setGuiProjection() = 0;  // Set the view for GUI mode.
    virtual void pushMatrix() = 0;
    virtual void popMatrix() = 0;
    virtual void setMatrixMode(int mode) = 0;
    virtual void setIdentityMatrix() = 0;
    virtual void rotateX(float angle) = 0;  // degrees
	virtual void rotateY(float angle) = 0;  // degrees
	virtual void rotateZ(float angle) = 0;  // degrees
    virtual void translate(float x, float y, float z) = 0;

	// This is conceptually equivalent to glMultMatrix.  The matrix values
	// are required to be in column major order.
    // Use array of floats to maintain independence from a specific math library
    virtual void applyTransform(float matrix[16]) = 0;

    // Rendering functions
    virtual void startRenderingPhase() = 0;  // Call this before rendering graphics objects.
    virtual void stopRenderingPhase() = 0;  // Call this when finished rendering graphics objects.
	virtual void drawIndexedTriangles(int count, int startIndex) = 0; // Draws from currently active vertex and index buffers

	// Misc
    virtual float *getModelViewProjectionMatrix() = 0;  // row-major format
	virtual bool usingDirect3D() = 0;
	virtual void bindOpenGLExtensions() = 0;
    // OpenGL and Direct3D have different creators due to some
    // underlying differences in vertex formats.
    virtual ModelCreator *getModelCreator() = 0;
    virtual GuiImageCreator *getGuiImageCreator() = 0;
    virtual FontGeometry *createFontGeometry() = 0;
    virtual void setShaderIsActive(bool value) {}  // TODO: Make this pure virtual
    
    enum {
        ALPHA_TRANSPARENCY,
        CULLING,
        PROJECTION,
        MODELVIEW,
        TEXTURE_2D,
        DEPTH_TEST,
        ALPHA_TEST,
        BLEND,
		LIGHTING,
		WIREFRAME
    };
};

#endif
