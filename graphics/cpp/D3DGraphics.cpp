#include <string>
#include <sstream>
#include "util.h"
#include "Gui.h"
#include "D3DGraphics.h"
#include "Camera.h"
#include "TextureInfo.h"
#include "VertexBufferInfo.h"
#include "IndexBufferInfo.h"
#include "D3DVertexBufferReference.h"
#include "D3DIndexBufferReference.h"
#include "D3DShaderReference.h"
#include "D3DTextureReference.h"
#include "D3DModelCreator.h"
#include "D3DGuiImageCreator.h"
#include "dxerr.h"
#include "D3DFontGeometry.h"
#include "ShaderInfo.h"
#include "ShaderReference.h"

D3DGraphics * D3DGraphics::getInstance()
{
	static D3DGraphics instance;
	return &instance;
}

bool D3DGraphics::usingDirect3D()
{
	return true;
}

void D3DGraphics::checkError(const std::string & message)
{
}

void D3DGraphics::clearError(unsigned int errorCodeToClear)
{
}

D3DGraphics::D3DGraphics()
: backgroundColor(), matrixMode(Graphics::MODELVIEW), matrixStack(), shaderIsActive(0)
{
	// This should be set after checking system capabilities.
	currentScreenFormat = D3DFMT_X8R8G8B8;

    modelCreator = new D3DModelCreator();
    guiImageCreator = new D3DGuiImageCreator();
}

D3DGraphics::~D3DGraphics()
{
	if (graphicsDevice)
		graphicsDevice->Release();
	if (graphicsObject)
		graphicsObject->Release();

    delete modelCreator;
    delete guiImageCreator;
}

void D3DGraphics::setBackgroundColor(float r, float g, float b, float a)
{
	backgroundColor.red = r;
	backgroundColor.green = g;
	backgroundColor.blue = b;
	backgroundColor.alpha = a;
}

void D3DGraphics::clearDepthAndScreenBuffers()
{
	graphicsDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		//D3DCOLOR_COLORVALUE(backgroundColor.red,backgroundColor.green,
		//backgroundColor.blue,backgroundColor.alpha), 1.0f, 0 );
		D3DCOLOR_COLORVALUE(.1,.1, .5,backgroundColor.alpha), 1.0f, 0 );
}

void D3DGraphics::setFullScreen(bool useFullScreen)
{
    fullScreen = useFullScreen;
    if (fullScreen)
    {
        screenWidth = GetSystemMetrics(SM_CXSCREEN);
        screenHeight = GetSystemMetrics(SM_CYSCREEN);
    }
}

void D3DGraphics::initialize()
{
    graphicsObject = Direct3DCreate9(D3D_SDK_VERSION);
    if(graphicsObject == NULL)
    {
	exit(21);
       MessageBox(GetActiveWindow(),L"Could not create Direct3D Object", L"D3D_OBJ ERR",MB_OK);
    }
    ZeroMemory(&presentParameters, sizeof(presentParameters));

	if (fullScreen)
        presentParameters.Windowed = false;
	else
        presentParameters.Windowed = true;

    //presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    presentParameters.SwapEffect = D3DSWAPEFFECT_COPY;
    presentParameters.EnableAutoDepthStencil = true;
    presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
    presentParameters.hDeviceWindow = GetActiveWindow();
    if (fullScreen)
    {
        screenWidth = GetSystemMetrics(SM_CXSCREEN);
        screenHeight = GetSystemMetrics(SM_CYSCREEN);
        presentParameters.BackBufferWidth = screenWidth;
        presentParameters.BackBufferHeight = screenHeight;
        //presentParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
        presentParameters.BackBufferFormat = currentScreenFormat;
    }
    else
    {
        presentParameters.BackBufferWidth = screenWidth;
        presentParameters.BackBufferHeight = screenHeight;
        presentParameters.BackBufferFormat = currentScreenFormat;
    }
    presentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
    presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    HWND theWindow = GetActiveWindow();
    if (!theWindow)
    {
        theWindow = GetForegroundWindow();
    }
    if (!theWindow)
    {
        fatalError("No window was created or window is not available.");
    }


	if( FAILED(graphicsObject->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
           theWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING,
           &presentParameters, &graphicsDevice)))
    {
        fatalError("Could not create Direct3D Device");
        //MessageBox(GetActiveWindow(),L"Could not create Direct3D Device",L"D3D_DEV ERR",MB_OK);
    }
	Camera::getInstance()->setScreenSize(screenWidth, screenHeight);
}

void D3DGraphics::startRenderingPhase()
{
	clearDepthAndScreenBuffers();
	this->setMatrixMode(Graphics::MODELVIEW);
	D3DXMatrixIdentity(&worldMatrix);
    
    // Always set to identity.  Only use worldMatrix.
    // This reflects the OpenGL paradigm.
    // We are using a right-handed coordinate system here
    // to be consistent with OpenGL.  Normally this would
    // require the negation of the z-coordinates in the
    // view matrix.  Since we are using a single model-view 
    // matrix like openGL, we do not need to modify the 
    // DirectX view matrix.
	D3DXMatrixIdentity(&viewMatrix);

	HRESULT result = graphicsDevice->BeginScene();

    // This call must occur after call to BeginScene().
    // But only call it once since the view matrix will
    // always be the identity.
	graphicsDevice->SetTransform(D3DTS_VIEW, &viewMatrix);
}

void D3DGraphics::stopRenderingPhase()
{
	HRESULT result = graphicsDevice->EndScene();
}

void D3DGraphics::enableMode(int mode)
{
    switch (mode)
    {
        case Graphics::CULLING:
            //graphicsDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
            break;
        case Graphics::TEXTURE_2D:
            
            break;
        case Graphics::DEPTH_TEST:
            graphicsDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
            graphicsDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
            //graphicsDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);  // probably the default
            graphicsDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);  // probably the default
            break;
        case Graphics::ALPHA_TEST:
            graphicsDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_TRUE);
            break;
        case Graphics::BLEND:
            graphicsDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, D3DZB_TRUE);
            break;
		case Graphics::LIGHTING:
			graphicsDevice->SetRenderState(D3DRS_LIGHTING, true);
			break;
		case Graphics::ALPHA_TRANSPARENCY:
            graphicsDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_TRUE);
            graphicsDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, D3DZB_TRUE);
            graphicsDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            graphicsDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);

            // Color transparency
            //graphicsDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
            //graphicsDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
			break;
		case Graphics::WIREFRAME:
			graphicsDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			break;
    }
}

void D3DGraphics::disableMode(int mode)
{
    switch (mode)
    {
        case Graphics::CULLING:
            graphicsDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
            break;
        case Graphics::TEXTURE_2D:
            
            break;
        case Graphics::DEPTH_TEST:
            graphicsDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
            
            break;
        case Graphics::ALPHA_TEST:
            graphicsDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_FALSE);
            break;
        case Graphics::BLEND:
            graphicsDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, D3DZB_FALSE);
            break;
		case Graphics::LIGHTING:
			graphicsDevice->SetRenderState(D3DRS_LIGHTING, false);
			break;
		case Graphics::ALPHA_TRANSPARENCY:
            graphicsDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_FALSE);
            graphicsDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, D3DZB_FALSE);
			break;
		case Graphics::WIREFRAME:
			graphicsDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			break;
    }
	// Integrate this.  What is the equivalent in OpenGL?
    //graphicsDevice->SetRenderState(D3DRS_AMBIENT,RGB(255,255,255));	
}

void D3DGraphics::swapBuffers()
{
	graphicsDevice->Present(NULL, NULL, NULL, NULL);
}

void D3DGraphics::pushMatrix()
{
    matrixStack.push(worldMatrix);
}

void D3DGraphics::popMatrix()
{
    worldMatrix = matrixStack.top();
    matrixStack.pop();
}

/**
 *   &x The distance from the right of the screen.
 *   &y The distance from the bottom of the screen.
 */
void D3DGraphics::setViewPort(int x, int y, int width, int height)
{
    D3DVIEWPORT9 viewPort;
    viewPort.X = x;
    viewPort.Y = y;
    viewPort.Width = width;
    viewPort.Height = height;
    graphicsDevice->SetViewport(&viewPort);
}

ScreenCoords D3DGraphics::getAbsoluteScreenCoords(int guiOrientation, int offsetX, 
                                                  int offsetY, int width, int height)
{
    ScreenCoords coords;   
	switch (guiOrientation)
	{
	case GUI_LOWER_LEFT :
		coords.left = offsetX;
		coords.right = coords.left + width - 1;
		coords.bottom = screenHeight - offsetY;
		coords.top = coords.bottom - height + 1;
		break;
	case GUI_UPPER_LEFT :
		coords.left = offsetX;
		coords.right = coords.left + width - 1;
        coords.top = offsetY;
		coords.bottom = coords.top + height - 1;
		break;
	case GUI_UPPER_RIGHT :
		coords.right = screenWidth - offsetX;
		coords.left = coords.right - width + 1;
        coords.top = offsetY;
		coords.bottom = coords.top + height - 1;
		break;
	case GUI_LOWER_RIGHT :
		coords.right = screenWidth - offsetX;
		coords.left = coords.right - width + 1;
		coords.bottom = screenHeight - offsetY;
		coords.top = coords.bottom - height + 1;
		break;
	case GUI_LOWER_CENTER :
		coords.left = screenWidth / 2 - width / 2 + offsetX;
		coords.right = coords.left + width - 1;
		coords.bottom = screenHeight - offsetY;
		coords.top = coords.bottom - height + 1;
		break;
	case GUI_UPPER_CENTER :
		coords.left = screenWidth / 2 - width / 2 + offsetX;
		coords.right = coords.left + width - 1;
		coords.top = offsetY;
		coords.bottom = coords.top + height - 1;
		break;
	case GUI_DEAD_CENTER :
		coords.left = screenWidth / 2 - width / 2 + offsetX;
		coords.right = coords.left + width - 1;
		coords.top = screenHeight / 2 - height / 2 + 1 + offsetY;
		coords.bottom = coords.top + height;
		break;
	}
    return coords;
}

ScreenPosition D3DGraphics::getAbsoluteScreenPosition(int x, int y)
{
    ScreenPosition pos;
    pos.x = x;
    pos.y = screenHeight - y;
    return pos;
}

bool D3DGraphics::ScreenCoordsContainsPosition(ScreenCoords coords, ScreenPosition pos)
{
    return pos.x >= coords.left && pos.x <= coords.right 
        && pos.y <= coords.bottom && pos.y >= coords.top;
}

void D3DGraphics::readFromBackBuffer(int x, int y, int width, int height, unsigned char *pixels)
{
    // End the scene to complete the rendering to the back buffer.
    graphicsDevice->EndScene();

    // This will be the back buffer
    IDirect3DSurface9 *renderTarget;

    // This is the surface we are copying the back buffer to.
    IDirect3DSurface9 *newSurface;


    HRESULT result;
    result = graphicsDevice->GetRenderTarget(0, &renderTarget); // 0 == BackBuffer
    if (FAILED(result))
    { 
        MessageBox(GetActiveWindow(),L"Failed to get the render target", DXGetErrorDescription(result), MB_OK); 
        return;
    }

    // The surface description of the back buffer.
    D3DSURFACE_DESC desc;
    renderTarget->GetDesc(&desc);
    result = graphicsDevice->CreateOffscreenPlainSurface(desc.Width, desc.Height, 
                                                    desc.Format, D3DPOOL_SYSTEMMEM, &newSurface, 0); 
    if (FAILED(result))
    { 
        MessageBox(GetActiveWindow(),L"Failed to create off-screen surface", DXGetErrorDescription(result), MB_OK);       
        renderTarget->Release();
        return;
    }
    result = graphicsDevice->GetRenderTargetData(renderTarget, newSurface); 
    if (FAILED(result))
    { 
        MessageBox(GetActiveWindow(),L"Failed to get render target data", DXGetErrorDescription(result), MB_OK);       
        renderTarget->Release();
        return;
    }
    
    D3DDISPLAYMODE displayMode; 
    graphicsObject->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);

    D3DLOCKED_RECT lockedRect;          
    RECT rect;             

    rect.left = 0;
    rect.right = displayMode.Width;   
    rect.top = 0; 
    rect.bottom = displayMode.Height;

    result = newSurface->LockRect( &lockedRect, &rect, D3DLOCK_READONLY );
    if (FAILED(result))
    { 
        MessageBox(GetActiveWindow(),L"Failed to lock new surface", DXGetErrorDescription(result), MB_OK);       
        renderTarget->Release();
        return;
    }
       
    unsigned char *buf = (unsigned char*)lockedRect.pBits;

    int pixelsIndex = 0;
    for (int row = y; row < height + y; row++)
    {
        for (int col = x; col < x + width; col++)
        {
            pixels[pixelsIndex++] = buf[row * lockedRect.Pitch + col * 4 + 0]; // blue
            pixels[pixelsIndex++] = buf[row * lockedRect.Pitch + col * 4 + 1]; // green
            pixels[pixelsIndex++] = buf[row * lockedRect.Pitch + col * 4 + 2]; // red
            //pixels[pixelsIndex++] = buf[row * lockedRect.Pitch + col * 4 + 3]; // alpha
        }
    }

    //Now the buffer has the pixels

    // Unlock the rect and release the surfaces.
    newSurface->UnlockRect();       
    renderTarget->Release(); 
    newSurface->Release();
}

void D3DGraphics::setPerspectiveProjection(float fovy, float aspect, float zNear, float zFar)
{
	// First change fovy to radians
    fovy = fovy * D3DX_PI / 180.0f;

	D3DXMatrixPerspectiveFovRH(&projectionMatrix, fovy, aspect, zNear, zFar);
	graphicsDevice->SetTransform(D3DTS_PROJECTION,&projectionMatrix);
}

void D3DGraphics::setGuiProjection()
{
    D3DXMatrixIdentity(&projectionMatrix);
}

void D3DGraphics::setMatrixMode(int mode)
{
	matrixMode = mode;
}

void D3DGraphics::setIdentityMatrix()
{
	switch (matrixMode)
	{
	case Graphics::MODELVIEW:
	    D3DXMatrixIdentity(&worldMatrix);
		break;
	case Graphics::PROJECTION:
	    D3DXMatrixIdentity(&projectionMatrix);
		break;
	}
}

/**
 *  Matrices in Direct3D are transposed with
 *  respect to OpenGL.  Therefore the operations
 *  must occur in reverse order.
 */
void D3DGraphics::rotateX(float angle)
{
	// Convert to radians
    angle = angle * D3DX_PI / 180.0f;
	
    D3DXMATRIX matrix;
    D3DXMatrixRotationX(&matrix, angle);
	switch (matrixMode)
	{
	case Graphics::MODELVIEW:
	    D3DXMatrixMultiply(&worldMatrix, &matrix, &worldMatrix);
		break;
	case Graphics::PROJECTION:
	    D3DXMatrixMultiply(&projectionMatrix, &matrix, &projectionMatrix);
		break;
	}
} 

/**
 *  Matrices in Direct3D are transposed with
 *  respect to OpenGL.  Therefore the operations
 *  must occur in reverse order.
 */
void D3DGraphics::rotateY(float angle)
{
	// Convert to radians
    angle = angle * D3DX_PI / 180.0f;
	
    D3DXMATRIX matrix;
    D3DXMatrixRotationY(&matrix, angle);
	switch (matrixMode)
	{
	case Graphics::MODELVIEW:
	    D3DXMatrixMultiply(&worldMatrix, &matrix, &worldMatrix);
		break;
	case Graphics::PROJECTION:
	    D3DXMatrixMultiply(&projectionMatrix, &matrix, &projectionMatrix);
		break;
	}  
}

/**
 *  Matrices in Direct3D are transposed with
 *  respect to OpenGL.  Therefore the operations
 *  must occur in reverse order.
 */
void D3DGraphics::rotateZ(float angle)
{
	// Convert to radians
    angle = angle * D3DX_PI / 180.0f;
	
    D3DXMATRIX matrix;
    D3DXMatrixRotationZ(&matrix, angle);
	switch (matrixMode)
	{
	case Graphics::MODELVIEW:
	    D3DXMatrixMultiply(&worldMatrix, &matrix, &worldMatrix);
		break;
	case Graphics::PROJECTION:
	    D3DXMatrixMultiply(&projectionMatrix, &matrix, &projectionMatrix);
		break;
	}
}

/**
 *  Matrices in Direct3D are transposed with
 *  respect to OpenGL.  Therefore the operations
 *  must occur in reverse order.
 */
void D3DGraphics::translate(float x, float y, float z)
{
    D3DXMATRIX matrix;
    D3DXMatrixTranslation(&matrix, x, y, z);
	switch (matrixMode)
	{
	case Graphics::MODELVIEW:
	    D3DXMatrixMultiply(&worldMatrix, &matrix, &worldMatrix);
		break;
	case Graphics::PROJECTION:
	    D3DXMatrixMultiply(&projectionMatrix, &matrix, &projectionMatrix);
		break;
	}
}

/**
 *  Matrices in Direct3D are transposed with
 *  respect to OpenGL.  Therefore the operations
 *  must occur in reverse order.
 */
void D3DGraphics::applyTransform(float matrix[16])
{
    D3DXMATRIX dxMatrix(matrix);

	switch (matrixMode)
	{
	case Graphics::MODELVIEW:
	    D3DXMatrixMultiply(&worldMatrix, &dxMatrix, &worldMatrix);
		break;
	case Graphics::PROJECTION:
	    D3DXMatrixMultiply(&projectionMatrix, &dxMatrix, &projectionMatrix);
		break;
	}
}

/**
 *  Don't forget to delete the array after finished with it.
 */
float *D3DGraphics::getModelViewProjectionMatrix()
{
    D3DXMATRIX worldViewProj;
    D3DXMatrixMultiply(&worldViewProj, &worldMatrix, &projectionMatrix);
    //D3DXMatrixTranspose(&worldViewProj, &worldViewProj);

    // TODO: Is there a better way to do this?
    float *matrix = new float[16];
    matrix[0] = worldViewProj._11;
    matrix[1] = worldViewProj._21;
    matrix[2] = worldViewProj._31;
    matrix[3] = worldViewProj._41;
    matrix[4] = worldViewProj._12;
    matrix[5] = worldViewProj._22;
    matrix[6] = worldViewProj._32;
    matrix[7] = worldViewProj._42;
    matrix[8] = worldViewProj._13;
    matrix[9] = worldViewProj._23;
    matrix[10] = worldViewProj._33;
    matrix[11] = worldViewProj._43;
    matrix[12] = worldViewProj._14;
    matrix[13] = worldViewProj._24;
    matrix[14] = worldViewProj._34;
    matrix[15] = worldViewProj._44;
    return matrix;
}

TextureReference *D3DGraphics::createTexture(TextureInfo *info)
{
	LPDIRECT3DTEXTURE9 texture;
	int pixelSize = 0;
	D3DFORMAT format;
	switch (info->getPixelFormat())
	{
	case TextureInfo::RGB16_FORMAT:
		format = D3DFMT_R5G6B5;
		pixelSize = 2;
		break;
	case TextureInfo::RGB24_FORMAT:
		format = D3DFMT_R8G8B8;
		pixelSize = 3;
		break;
	case TextureInfo::ARGB32_FORMAT:
		format = D3DFMT_A8R8G8B8;
		pixelSize = 4;
		break;
	}
	int dataSize = info->getHeight() * info->getWidth() * pixelSize;

	// If numberOfLevels == 0, then sublevels down to 1 by 1 will
	// be generated if hardware supports mipmapping.
    // TODO: Ultimately this will be set in the TextureInfo object.
	int numberOfLevels = 1;
	HRESULT hr = graphicsDevice->CreateTexture(info->getWidth(), 
		                          info->getHeight(),
								  numberOfLevels,
								  D3DUSAGE_DYNAMIC,
								  currentScreenFormat,
                                  //format,
								  D3DPOOL_DEFAULT,
								  &texture,
								  NULL);
	if (FAILED(hr))
	{
		
        MessageBox(GetActiveWindow(),L"Failed to create texture", DXGetErrorDescription(hr), MB_OK);
	}
    //D3DSURFACE_DESC pDesc;
    //texture->GetLevelDesc(1, &pDesc);
	D3DTextureReference *reference = new D3DTextureReference(graphicsDevice);
	reference->setTextureObject(texture);

	return reference;
}

/**
 *   Creates a vertex buffer and return a reference to it.
 */
VertexBufferReference *D3DGraphics::createVertexBuffer(VertexBufferInfo *info)
{
	// Note: LPDIRECT3DVERTEXBUFFER9 is defined by the following:
	// typedef struct IDirect3DVertexBuffer9 *LPDIRECT3DVERTEXBUFFER9
	LPDIRECT3DVERTEXBUFFER9 buffer;
	DWORD vertexFormat;
	void *vertexData = info->getVertexData();
	int numberOfVertices = info->getNumberOfVertices();
	int vertexSize = info->getSizeOfVertex();

	switch (info->getVertexType())
	{
	case VertexBufferInfo::VERTEX1:
		vertexFormat = D3DFVF_XYZ | D3DFVF_TEX1;
		break;
	case VertexBufferInfo::VERTEX2:
		// TODO: set format for this type
		break;
	case VertexBufferInfo::VERTEX3:
		vertexFormat = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	    break;
	case VertexBufferInfo::VERTEX4:
		vertexFormat = D3DFVF_XYZRHW | D3DFVF_TEX1;
		break;
    case VertexBufferInfo::VERTEX5:
        vertexFormat = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
        break;
	case VertexBufferInfo::VERTEX6:
		vertexFormat = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
        break;
    case VertexBufferInfo::VERTEX7:
        vertexFormat = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX2;
	    break;
    case VertexBufferInfo::VERTEX8:
        vertexFormat = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX3;
	    break;
	}

	int dataSize = numberOfVertices * vertexSize;
	graphicsDevice->CreateVertexBuffer(dataSize ,D3DUSAGE_WRITEONLY,
                                    vertexFormat,
									D3DPOOL_MANAGED, &buffer ,NULL);
    D3DVertexBufferReference *reference = 
		new D3DVertexBufferReference(this, vertexSize);
	reference->setBufferObject(buffer);
	reference->setVertexType(info->getVertexType());

	void *vertexBufferMemory;

	// Write the vertex data into memory referenced by the vertex buffer.
	buffer->Lock(0, 0,(void**)&vertexBufferMemory, 0);
	memcpy(vertexBufferMemory, vertexData, dataSize);
    buffer->Unlock();

	return reference;
}

/**
 *   Creates an index buffer and returns a reference to it.
 */
IndexBufferReference *D3DGraphics::createIndexBuffer(IndexBufferInfo *info)
{
	LPDIRECT3DINDEXBUFFER9 buffer;
    int indexSize = 0;
    D3DFORMAT indexFormat;
    switch (info->getIndexType())
    {
    case IndexBufferInfo::INT_16_BIT_INDICES:
        indexFormat = D3DFMT_INDEX16;
        indexSize = 2;
        break;
    case IndexBufferInfo::INT_32_BIT_INDICES:
        indexFormat = D3DFMT_INDEX32;
        indexSize = 4;
        break;
    }
    int indexCount = info->getNumberOfIndices();
	int dataSize = indexCount * indexSize;
	graphicsDevice->CreateIndexBuffer(dataSize, D3DUSAGE_WRITEONLY,
		                              indexFormat, D3DPOOL_MANAGED, 
									  &buffer, NULL);
	D3DIndexBufferReference *reference =
		new D3DIndexBufferReference(graphicsDevice, indexCount);
	reference->setBufferObject(buffer);
	void *indexData = info->getIndexData();

	void *indexBufferMemory;

	buffer->Lock(0, sizeof(indexBufferMemory), (void**) &indexBufferMemory, 0);
	memcpy(indexBufferMemory, indexData, dataSize);
    return reference;
}

ShaderReference *D3DGraphics::createShader(ShaderInfo *info)
{
    if (info->getFileName().find("glvert") != std::string::npos 
        ||
        info->getFileName().find("glfrag") != std::string::npos)
    {
        fatalError("GLSL not supported in Direct3D mode.");
    }
    D3DShaderReference *shader = new D3DShaderReference(graphicsDevice, 
        info->getShaderType(), info->getContextName());
    shader->load(info);
    return shader;
}

void D3DGraphics::setAttributesFor3DPicking()
{
}

void D3DGraphics::unsetAttributesFor3DPicking()
{
}

void D3DGraphics::setColor(float r, float g, float b, float a)
{
    // TODO: Implement this
}

void D3DGraphics::setColor(unsigned int color)
{
    currentColor = color;
}

/**
 *   Draw triangles from the active vertex buffer using the active index buffer.
 *   @startIndex is the start index of the index buffer
 *   @count is the number of indices.
 */
void D3DGraphics::drawIndexedTriangles(int count, int startIndex)
{ 
	graphicsDevice->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
	graphicsDevice->SetTransform(D3DTS_WORLD, &worldMatrix);
	graphicsDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, count, startIndex, count/3);
}

ModelCreator *D3DGraphics::getModelCreator()
{
    return modelCreator;
}

GuiImageCreator *D3DGraphics::getGuiImageCreator()
{
    return guiImageCreator;
}

FontGeometry *D3DGraphics::createFontGeometry()
{
    return new D3DFontGeometry();
}

