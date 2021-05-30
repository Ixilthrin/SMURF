#include <string>
#include <iostream>
#include <sstream>

#include "opengl_includes.h"
#include "util.h"
#include "Gui.h"
#include "GLGraphics.h"
#include "Camera.h"
#include "GLTextureReference.h"
#include "TextureInfo.h"
#include "GLVertexBufferReference.h"
#include "VertexBufferInfo.h"
#include "GLIndexBufferReference.h"
#include "IndexBufferInfo.h"
#include "GLModelCreator.h"
#include "GLGuiImageCreator.h"
#include "GLFontGeometry.h"
#include "ShaderInfo.h"
#include "MatrixArray.h"

//#define USING_CG_SHADERS
#ifdef USING_CG_SHADERS
#include "GLShaderReference.h"
#endif
#include "GLSLShaderReference.h"

GLGraphics * GLGraphics::getInstance()
{
	static GLGraphics instance;
	return &instance;
}

bool GLGraphics::usingDirect3D()
{
	return false;
}

void GLGraphics::checkError(const std::string & message)
{
    
#ifndef USING_OPENGL_WITH_IPHONE  // Defined in opengl_includes.h
    GLenum errorCode = glGetError();
    if (errorCode == GL_NO_ERROR)
    {
        return;
    }
    const GLubyte * errorString = gluErrorString(errorCode);
    std::stringstream ss;
    ss << errorString << std::endl << message;
    fatalError(ss.str());
#endif
}

void GLGraphics::clearError(GLenum errorCodeToClear)
{
#ifndef USING_OPENGL_WITH_IPHONE // Defined in opengl_includes.h
    GLenum errorCode = glGetError();
    if (errorCode == GL_NO_ERROR || errorCode == errorCodeToClear)
    {
        return;
    }
    const GLubyte * errorString = gluErrorString(errorCode);
    std::stringstream ss;
    ss << errorString << std::endl;
    fatalError(ss.str());
#endif
}

GLGraphics::GLGraphics()
: indexBufferType(0)
{
    modelCreator = new GLModelCreator();
    guiImageCreator = new GLGuiImageCreator();
}

GLGraphics::~GLGraphics()
{
    delete modelCreator;
    delete guiImageCreator;
}

void GLGraphics::bindOpenGLExtensions()
{
#ifdef USING_SDL_WITH_OPENGL  // See opengl_includes.h
	loadGLBindings();
#endif
}

void GLGraphics::setBackgroundColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void GLGraphics::clearDepthAndScreenBuffers()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLGraphics::startRenderingPhase()
{
	clearDepthAndScreenBuffers();
}

void GLGraphics::stopRenderingPhase()
{
    glFlush();   // Is this necessary?
}

void GLGraphics::enableMode(int mode)
{
    switch (mode)
    {
        case Graphics::CULLING:
            //glEnable(GL_CULL_FACE);
            //glCullFace(GL_BACK);
            break;
        case Graphics::TEXTURE_2D:
            glEnable(GL_TEXTURE_2D);
            break;
        case Graphics::DEPTH_TEST:
            glEnable(GL_DEPTH_TEST);
            break;
        case Graphics::ALPHA_TEST:
            glEnable(GL_ALPHA_TEST);
            break;
        case Graphics::BLEND:
            glEnable(GL_BLEND);
            break;
        case Graphics::ALPHA_TRANSPARENCY:
            glEnable(GL_ALPHA_TEST);
            glEnable(GL_BLEND);
	        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	        glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
            break;
		case Graphics::WIREFRAME:
#ifndef USING_OPENGL_WITH_IPHONE  // Defined in opengl_includes.h
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
			break;
    }
}

void GLGraphics::disableMode(int mode)
{
    switch (mode)
    {
        case Graphics::CULLING:
            glDisable(GL_CULL_FACE);
            break;
        case Graphics::TEXTURE_2D:
            glDisable(GL_TEXTURE_2D);
            break;
        case Graphics::DEPTH_TEST:
            glDisable(GL_DEPTH_TEST);
            break;
        case Graphics::ALPHA_TEST:
            glDisable(GL_ALPHA_TEST);
            break;
        case Graphics::BLEND:
            glDisable(GL_BLEND);
            break;
        case Graphics::ALPHA_TRANSPARENCY:
            glDisable(GL_ALPHA_TEST);
            glDisable(GL_BLEND);
            break;
		case Graphics::WIREFRAME:
#ifndef USING_OPENGL_WITH_IPHONE  // Defined in opengl_includes.h
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
			break;
    }
}

TextureReference *GLGraphics::createTexture(TextureInfo *info)
{
	bool isDynamic = info->getIsDynamic();  // TODO: Not really used currently.

	GLuint textureId;

    GLuint thePixelFormat = 0;	
    GLuint thePixelFormat2 = 0;		
	int pixelSize = 0;
	if (info->getPixelFormat() == TextureInfo::RGB24_FORMAT)
	{
		thePixelFormat = GL_RGB;
#ifdef USING_OPENGL_WITH_IPHONE  // Defined in opengl_includes.h
		thePixelFormat2 = GL_RGB;
#else
        thePixelFormat2 = GL_BGR;
#endif
		pixelSize = 24;
	}
	else if (info->getPixelFormat() == TextureInfo::ARGB32_FORMAT)
	{
		thePixelFormat = GL_RGBA;
		thePixelFormat2 = GL_RGBA;
		pixelSize = 32;
	}
	else
	{
		fatalError("Unsupported bytes per pixel in image.");
	}

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  // Not sure what this is for.
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    if (!info->getGenerateMipmap())
    {
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
    }
    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        thePixelFormat,
        info->getWidth(), 
        info->getHeight(),
        0, 
        thePixelFormat2,
        GL_UNSIGNED_BYTE, 
        info->getPixelData());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    // TODO: info->getPixelFormat() and texReference->setPixelFormat() represent
    // formats from different domains.  info's format is a pixel format defined
    // in TextureInfo, while ref's format is a GL defined format.

	GLTextureReference *texReference = 
		new GLTextureReference(info->getWidth(), info->getHeight(), info->getPixelFormat(), isDynamic);
	
    texReference->setPixelFormat(thePixelFormat);
	texReference->setTextureId(textureId);
	return texReference;
}

void GLGraphics::pushMatrix()
{
    glPushMatrix();
}

void GLGraphics::popMatrix()
{
    glPopMatrix();
}


void GLGraphics::setViewPort(int x, int y, int width, int height)
{
   	glViewport(0, 0, width, height);
}

ScreenCoords GLGraphics::getAbsoluteScreenCoords(int guiOrientation, int offsetX, 
                                                 int offsetY, int width, int height)
{
    ScreenCoords coords;
    
	switch (guiOrientation)
	{
	case GUI_LOWER_LEFT :
		coords.left = offsetX;
		coords.right = coords.left + width - 1;
		coords.bottom = offsetY;
		coords.top = coords.bottom + height - 1;
		break;
	case GUI_UPPER_LEFT :
		coords.left = offsetX;
		coords.right = coords.left + width - 1;
        coords.top = screenHeight - offsetY;
		coords.bottom = coords.top - height + 1;
		break;
	case GUI_UPPER_RIGHT :
		coords.right = screenWidth - offsetX;
		coords.left = coords.right - width + 1;
		coords.top = screenHeight - offsetY;
		coords.bottom = coords.top - height + 1;
		break;
	case GUI_LOWER_RIGHT :
		coords.right = screenWidth - offsetX;
		coords.left = coords.right - width + 1;
		coords.bottom = offsetY;
		coords.top = coords.bottom + height - 1;
		break;
	case GUI_LOWER_CENTER :
		coords.left = screenWidth / 2 - width / 2 + offsetX;
		coords.right = coords.left + width - 1;
		coords.bottom = offsetY;
		coords.top = coords.bottom + height - 1;
		break;
	case GUI_UPPER_CENTER :
		coords.left = screenWidth / 2 - width / 2 + offsetX;
		coords.right = coords.left + width - 1;
		coords.top = screenHeight - offsetY;
		coords.bottom = coords.top - height + 1;
		break;
	case GUI_DEAD_CENTER :
		coords.left = screenWidth / 2 - width / 2 + offsetX;
		coords.right = coords.left + width - 1;
		coords.top = screenHeight / 2 + height / 2 + offsetY;
		coords.bottom = coords.top - height + 1;
		break;
	}
    return coords;
}

ScreenPosition GLGraphics::getAbsoluteScreenPosition(int x, int y)
{
    ScreenPosition pos;
    pos.x = x;
    pos.y = y;
    return pos;
}

bool GLGraphics::ScreenCoordsContainsPosition(ScreenCoords coords, ScreenPosition pos)
{
    return pos.x >= coords.left && pos.x <= coords.right 
        && pos.y >= coords.bottom && pos.y <= coords.top;
}

void GLGraphics::readFromBackBuffer(int x, int y, int width, int height, unsigned char *pixels)
{
#ifndef USING_OPENGL_WITH_IPHONE  // Defined in opengl_includes.h
	glReadBuffer(GL_BACK);
	glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
#endif
}

void GLGraphics::readFromDepthBuffer(int x, int y, int width, int height, void *pixels)
{
	// You need to know the size of the depth buffer
	// width * height * bytes_per_pixel
	// The depth buffer precision is a factor of the difference between near plane and far plane.

	// glGetIntegerv(GL_DEPTH_BITS, &bits);
	// glReadPixels(x, y, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, pixels);
	// Each component is then multiplied by GL_DEPTH_SCALE, added to GL_DEPTH_BIAS, and finally clamped to the range 01. 
}

static void printMatrix(int type)
{
    float m[16];
    glGetFloatv(GL_PROJECTION_MATRIX, m);
    for (int i = 0; i < 16; ++i)
    {
        if (i % 4 == 0)
            std::cout << std::endl;
        std::cout << m[i] << "   ";
    }
    std::cout << std::endl;
}

void GLGraphics::setPerspectiveProjection(float fovy, float aspect, float zNear, float zFar)
{
	glMatrixMode(GL_PROJECTION);

#if 1
    float f = 1.0f / tan(fovy * 3.14159f / 360.0f);
    float matrix[16] = { f / aspect, 0, 0, 0,
                         0, f, 0, 0,
                         0, 0, (zFar + zNear) / (zNear - zFar), -1,
                         0, 0, 2 * zFar * zNear / (zNear - zFar), 0};
	glLoadIdentity();
    glMultMatrixf(matrix);
#else
	glLoadIdentity();
    gluPerspective(fovy, aspect, zNear, zFar);
#endif
#if 0
    printMatrix(GL_PROJECTION_MATRIX);
#endif
    return;
}

void GLGraphics::setGuiProjection()
{
#ifdef USING_OPENGL_WITH_IPHONE  // Defined in opengl_includes.h
    fatalError("glOrtho not supported for iPhone");
#else
	glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);
#endif
}

void GLGraphics::setMatrixMode(int mode)
{
    switch (mode)
    {
        case PROJECTION:
            glMatrixMode(GL_PROJECTION);
            break;
        case MODELVIEW:
            glMatrixMode(GL_MODELVIEW);
            break;
    }
}

void GLGraphics::setIdentityMatrix()
{
    glLoadIdentity();
}

void GLGraphics::rotateX(float angle)
{
    glRotatef(angle, 1.0f, 0.0f, 0.0f);
}


void GLGraphics::rotateY(float angle)
{
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
}

void GLGraphics::rotateZ(float angle)
{
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
}

void GLGraphics::translate(float x, float y, float z)
{
    glTranslatef(x, y, z);
}

void GLGraphics::applyTransform(float matrix[16])
{
    glMultMatrixf(matrix);
}

VertexBufferReference *GLGraphics::createVertexBuffer(VertexBufferInfo *info)
{
    unsigned int id;
    glGenBuffers(1, &id);
    // GL_ARRAY_BUFFER is for vertices
    glBindBuffer(GL_ARRAY_BUFFER, id);
    int offsetInBytes = 0;
    int numberOfVertices = info->getNumberOfVertices();
    int vertexSize = info->getSizeOfVertex();
    int sizeInBytes = numberOfVertices * vertexSize;
    glBufferData(GL_ARRAY_BUFFER, sizeInBytes, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, offsetInBytes, sizeInBytes, info->getVertexData());
    GLVertexBufferReference *ref = new GLVertexBufferReference(info->getVertexType());
    ref->setId(id);
    ref->setSizeOfVertex(info->getSizeOfVertex());

    int vertexType = info->getVertexType();
    if (vertexType == VertexBufferInfo::VERTEX2)
    {
        glColorPointer(4, GL_FLOAT, vertexSize, BUFFER_OFFSET(12));
        glVertexPointer(3, GL_FLOAT, vertexSize, BUFFER_OFFSET(0));  // This gl..Pointer() should be last.
    } 
    else if (vertexType == VertexBufferInfo::VERTEX1)
    {
        //glClientActiveTexture(GL_TEXTURE0);
        glTexCoordPointer(2, GL_FLOAT, vertexSize, BUFFER_OFFSET(12));
        glVertexPointer(3, GL_FLOAT, vertexSize, BUFFER_OFFSET(0));  // This gl..Pointer() should be last.
    }
    else if (vertexType == VertexBufferInfo::VERTEX5)
    {
        glNormalPointer(GL_FLOAT, vertexSize, BUFFER_OFFSET(12));
        //glClientActiveTexture(GL_TEXTURE0);
        glTexCoordPointer(2, GL_FLOAT, vertexSize, BUFFER_OFFSET(24));
        glVertexPointer(3, GL_FLOAT, vertexSize, BUFFER_OFFSET(0));  // This gl..Pointer() should be last.
    }
    else if (vertexType == VertexBufferInfo::VERTEX6)
    {
        fatalError("VERTEX6 not implemented for OpenGL");
    }
    else if (vertexType == VertexBufferInfo::VERTEX7)
    {
        fatalError("VERTEX7 not implemented for OpenGL");
    }
    else if (vertexType == VertexBufferInfo::VERTEX8)
    {
        glNormalPointer(GL_FLOAT, vertexSize, BUFFER_OFFSET(12));
        glClientActiveTexture(GL_TEXTURE0);
        glTexCoordPointer(2, GL_FLOAT, vertexSize, BUFFER_OFFSET(24));
        glClientActiveTexture(GL_TEXTURE1);
        glTexCoordPointer(2, GL_FLOAT, vertexSize, BUFFER_OFFSET(32));
        glClientActiveTexture(GL_TEXTURE2);
        glTexCoordPointer(2, GL_FLOAT, vertexSize, BUFFER_OFFSET(40));
        glVertexPointer(3, GL_FLOAT, vertexSize, BUFFER_OFFSET(0));  // This gl..Pointer() should be last.
    }
    else
    {
        fatalError("Unknown vertex type in GLGraphics.");
    }

    return ref;
}

IndexBufferReference *GLGraphics::createIndexBuffer(IndexBufferInfo *info)
{
    int indexSize = 0;
    switch (info->getIndexType())
    {
    case IndexBufferInfo::INT_16_BIT_INDICES:
        indexSize = 2;
        break;
    case IndexBufferInfo::INT_32_BIT_INDICES:
        indexSize = 4;
        break;
    }
    int numberOfIndices = info->getNumberOfIndices();

    int indexDataSize = numberOfIndices * indexSize;
    unsigned int id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize, info->getIndexData(), GL_STATIC_DRAW);

    GLIndexBufferReference *ref = new GLIndexBufferReference(this, numberOfIndices);
    ref->setId(id);
    ref->setIndexType(info->getIndexType());
    return ref;
}

ShaderReference *GLGraphics::createShader(ShaderInfo *info)
{
    if (info->getFileName().find("glvert") != std::string::npos 
        ||
        info->getFileName().find("glfrag") != std::string::npos)
    {
        GLSLShaderReference *shader = 
            new GLSLShaderReference(info->getShaderType(), info->getContextName());
        shader->load(info);
        return shader;
    }
    else
    {
        // TODO: GLShaderReference should be supported on all platforms.
#ifdef USING_CG_SHADERS
        GLShaderReference *shader = 
            new GLShaderReference(info->getShaderType(), info->getContextName());
        shader->load(info);
        return shader;
#endif
    }
    return 0;
}

/**
 *  The purpose of this method is to be able
 *  to combine many functions calls for the
 *  special feature of picking.
 */
void GLGraphics::setAttributesFor3DPicking()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_STENCIL_TEST); 
	glDisable(GL_DITHER);
	glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
}

void GLGraphics::unsetAttributesFor3DPicking()
{
    // We want to set the state to what it was before picking.
}

void GLGraphics::setColor(float r, float g, float b, float a)
{
    glColor4f(r, g, b, a);
}

void GLGraphics::setColor(unsigned int color)
{
#ifdef USING_OPENGL_WITH_IPHONE  // Defined in opengl_includes.h
    fatalError("glColor3ubv not supported for iPhone");
#else
    glColor3ubv(reinterpret_cast<GLubyte *>(&color));
#endif
}

//  @count is the number of indices.
void GLGraphics::drawIndexedTriangles(int count, int startIndex)
{
    if (indexBufferType == 0)
    {
        // Fatal error
		std::cerr << "Index buffer type not set at drawIndexedTriangles." << std::endl;
        std::cerr << "Press any key to terminate..." << std::endl;
        std::cin.get();
    }
    //glDrawElements(GL_TRIANGLES, count, indexBufferType, BUFFER_OFFSET(0));
    glDrawElements(GL_TRIANGLES, 1, indexBufferType, BUFFER_OFFSET(0));
}

ModelCreator *GLGraphics::getModelCreator()
{
    return modelCreator;
}

GuiImageCreator *GLGraphics::getGuiImageCreator()
{
    return guiImageCreator;
}

FontGeometry *GLGraphics::createFontGeometry()
{
    return new GLFontGeometry();
}

/**
 *  Don't forget to delete the array after finished with it.
 */
float *GLGraphics::getModelViewProjectionMatrix()
{
    float modelview[16];
    float projection[16];

    // get the current modelview matrix
    glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
    glGetFloatv(GL_PROJECTION_MATRIX, projection);

    MatrixArray modView;
    modView.setTransformColumnMajor(modelview);
    MatrixArray proj;
    proj.setTransformColumnMajor(projection);

    MatrixArray result = MatrixArray::product(proj, modView);
    float *values = new float[16];
    for (int i = 0; i < 16; ++i)
    {
        values[i] = result.transform[i];
    }
    return values;
}