#include "Cube.h"
#include "util.h"
#include "Graphics.h"
#include "GraphicsSystem.h"

GLuint Cube::displayListName = 0;
int Cube::totalReferenceCount = 0;

Cube::Cube()
: referenceCount(0) 
{ 
}

Cube * Cube::create()
{
    if (totalReferenceCount == 0)
    {
        displayListName = glGenLists(1);
        glNewList(displayListName, GL_COMPILE);
        glBegin(GL_QUADS);
        // top
        glVertex3f(-1, 1, -1);
        glVertex3f(-1, 1, 1);
        glVertex3f(1, 1, 1);
        glVertex3f(1, 1, -1);
        // bottom
        glVertex3f(-1, -1, -1);
        glVertex3f(-1, -1, 1);
        glVertex3f(1, -1, 1);
        glVertex3f(1, -1, -1);
        // front
        glVertex3f(-1, 1, -1);
        glVertex3f(-1, -1, -1);
        glVertex3f(1, -1, -1);
        glVertex3f(1, 1, -1);
        // back
        glVertex3f(-1, 1, 1);
        glVertex3f(-1, -1, 1);
        glVertex3f(1, -1, 1);
        glVertex3f(1, 1, 1);
        // left
        glVertex3f(-1, 1, -1);
        glVertex3f(-1, -1, -1);
        glVertex3f(-1, -1, 1);
        glVertex3f(-1, 1, 1);
        // right
        glVertex3f(1, 1, -1);
        glVertex3f(1, -1, -1);
        glVertex3f(1, -1, 1);
        glVertex3f(1, 1, 1);
        glEnd();
        glEndList();
    }
    Cube * cube = new Cube();
    cube->acquireReference();
	return cube;
}

void Cube::acquireReference()
{
    ++referenceCount;
    ++totalReferenceCount;
}

void Cube::releaseReference()
{
	--totalReferenceCount;
	if (totalReferenceCount == 0)
	{
		glDeleteLists(displayListName, 1);
		displayListName = 0;
	}
	--referenceCount;
    if (referenceCount == 0)
    {
        delete this;
        return;
    }
	else if (referenceCount < 0)
	{
		fatalError("Cube released more times than acquired.");
	}
}

void Cube::update(float deltaSeconds)
{
}

void Cube::draw(Graphics *graphics, bool picking)
{
    if (!picking)
    {
        glDisable(GL_TEXTURE_2D);
        glColor4f(1, 0, 0, 1);
    }
    glCallList(displayListName);
}
