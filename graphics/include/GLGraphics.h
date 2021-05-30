#ifndef GENG_GL_GRAPHICS_H
#define GENG_GL_GRAPHICS_H
#include "Graphics.h"

#include "Camera.h"

#include <string>

class ModelCreator;
class GuiImageCreator;
class FontGeometry;

class GLGraphics : public Graphics
{
public:
	static GLGraphics * getInstance();
	virtual bool usingDirect3D();
	virtual void bindOpenGLExtensions();
	virtual void checkError(const std::string & message = std::string());
	virtual void clearError(unsigned int errorCodeToClear);

    virtual ~GLGraphics();
    virtual void setBackgroundColor(float r, float g, float b, float a);
	virtual void clearDepthAndScreenBuffers();
    virtual void initialize() { Camera::getInstance()->setScreenSize(screenWidth, screenHeight); 
	                            bindOpenGLExtensions(); }
    virtual void startRenderingPhase();
    virtual void stopRenderingPhase();
    virtual int getScreenWidth() { return screenWidth; }
    virtual int getScreenHeight() { return screenHeight; }
    virtual void setScreenWidth(int width) { screenWidth = width; }
    virtual void setScreenHeight(int height) { screenHeight = height; }
    virtual bool isFullScreen() { return fullScreen; }
    virtual void setFullScreen(bool useFullScreen) { fullScreen = useFullScreen; }
    virtual ScreenCoords getAbsoluteScreenCoords(int guiOrientation, int offsetX, 
                                                 int offsetY, int width, int height);
    virtual ScreenPosition getAbsoluteScreenPosition(int x, int y);
    virtual bool ScreenCoordsContainsPosition(ScreenCoords coords, ScreenPosition pos);
    virtual void readFromBackBuffer(int x, int y, int width, int height, unsigned char *pixels);
    virtual void readFromDepthBuffer(int x, int y, int width, int height, void *pixels);
    virtual void enableMode(int mode);
    virtual void disableMode(int mode);
    virtual TextureReference *createTexture(TextureInfo *data);
    virtual void pushMatrix();
    virtual void popMatrix();
    virtual void setViewPort(int x, int y, int width, int height);
    virtual void setPerspectiveProjection(float fovy, float aspect, float zNear, float zFar);
    virtual void setGuiProjection();  // Set the view for GUI mode.
    virtual void setMatrixMode(int mode);
    virtual void setIdentityMatrix();
    virtual void rotateX(float angle);
	virtual void rotateY(float angle);
	virtual void rotateZ(float angle);
    virtual void translate(float x, float y, float z);
    virtual void applyTransform(float matrix[16]);
    virtual VertexBufferReference *createVertexBuffer(VertexBufferInfo *info);
    virtual IndexBufferReference *createIndexBuffer(IndexBufferInfo *info); 
    virtual ShaderReference *createShader(ShaderInfo *info);
    virtual void setAttributesFor3DPicking(); 
    virtual void unsetAttributesFor3DPicking();
    virtual void setColor(float r, float g, float b, float a);
    virtual void setColor(unsigned int color);
	virtual void drawIndexedTriangles(int count, int startIndex);
    virtual float *getModelViewProjectionMatrix();

    void setIndexBufferType(int type) { indexBufferType = type; }

    virtual ModelCreator *getModelCreator();
    virtual GuiImageCreator *getGuiImageCreator();
    virtual FontGeometry *createFontGeometry();
private:
    GLGraphics();
    int screenWidth;
    int screenHeight;
    bool fullScreen;
    int cullingMode;
    int indexBufferType;  // Used in drawIndexedTriangles.  Set when index buffer is activated.
    ModelCreator *modelCreator;
    GuiImageCreator *guiImageCreator;
};

#endif
