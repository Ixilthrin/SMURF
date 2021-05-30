#ifndef GENG_D3D_GRAPHICS_H
#define GENG_D3D_GRAPHICS_H

// Direct3D includes
// Project should be setup with associated libs: d3d9.lib, d3dx9.lib
// Add to LIB path: <directx9 base dir>/Lib/x86
// Set windows environment variables LIB and INCLUDE to point
// to <directx9 base dir>/Lib/x86 and <directx9 base dir>/Include respectively.
// $(INCLUDE) has been added to the project properties
// $(LIB) has been added to the project properties
// d3d9.lib and d3dx9.lib have been added to the link lib libraries list in properties
#include <windows.h>	
#include <d3d9.h>  // Add to include path: <directx9 base dir>/Include
#include <d3dx9.h>
#include <d3d9types.h>

#include <string>
#include <stack>
#include "Color.h"
#include "Graphics.h"

class ModelCreator;
class GuiImageCreator;
class FontGeometry;

class D3DGraphics : public Graphics
{
public:
	static D3DGraphics * getInstance();
	virtual bool usingDirect3D();
	virtual void bindOpenGLExtensions() {}
	virtual void checkError(const std::string & message = std::string());
	virtual void clearError(unsigned int errorCodeToClear);

    virtual ~D3DGraphics();
    virtual void setBackgroundColor(float r, float g, float b, float a);	
	virtual void clearDepthAndScreenBuffers();
    virtual void initialize();
    virtual void startRenderingPhase();
    virtual void stopRenderingPhase();
    virtual int getScreenWidth() { return screenWidth; }
    virtual int getScreenHeight() { return screenHeight; }
    virtual void setScreenWidth(int width) { screenWidth = width; }
    virtual void setScreenHeight(int height) { screenHeight = height; }
    virtual bool isFullScreen() { return fullScreen; }
    virtual void setFullScreen(bool useFullScreen);
    virtual ScreenCoords getAbsoluteScreenCoords(int guiOrientation, int offsetX, 
                                                 int offsetY, int width, int height);
    virtual ScreenPosition getAbsoluteScreenPosition(int x, int y);
    virtual bool ScreenCoordsContainsPosition(ScreenCoords coords, ScreenPosition pos);
    virtual void readFromBackBuffer(int x, int y, int width, int height, unsigned char *pixels);
    virtual void enableMode(int mode);
    virtual void disableMode(int mode);
    void swapBuffers();
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
    virtual ModelCreator *getModelCreator();
    virtual GuiImageCreator *getGuiImageCreator();
    virtual FontGeometry *createFontGeometry();
    D3DCOLOR getCurrentColor() { return currentColor; }
    virtual float *getModelViewProjectionMatrix();

    LPDIRECT3DDEVICE9 getDevice() { return graphicsDevice; }
    bool getShaderIsActive() { return shaderIsActive; }
    virtual void setShaderIsActive(bool value) { shaderIsActive = value; }

    D3DXMATRIX getWorldViewProj();
private:
    D3DGraphics();
    int screenWidth;
    int screenHeight;
    bool fullScreen;
    int cullingMode;
	Color backgroundColor;
	int matrixMode;
    std::stack<D3DXMATRIX> matrixStack;
    bool shaderIsActive;  // true if there is a currently active shader program

	D3DFORMAT currentScreenFormat;
	LPDIRECT3D9 graphicsObject;    //Used to create the graphics device.
	LPDIRECT3DDEVICE9 graphicsDevice; // Used for the major graphics operations.
	D3DPRESENT_PARAMETERS presentParameters;  // Window presentation properties.
	D3DXMATRIX projectionMatrix;
	D3DXMATRIX worldMatrix;
    D3DXMATRIX viewMatrix;  // Not used to conform to OpenGL.  Always set to identity matrix.

    ModelCreator *modelCreator;
    GuiImageCreator *guiImageCreator;
    D3DCOLOR currentColor;
};

#endif
