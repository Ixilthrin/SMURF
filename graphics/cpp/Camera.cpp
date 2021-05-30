#include "Camera.h"
#include "Graphics.h"
#include "GraphicsSystem.h"

Camera Camera::instance; 

Camera::Camera()
: screenWidth(0), screenHeight(0), nearPlane(0.4f), 
  farPlane(2000), eyeX(0), eyeY(0), eyeZ(0), yaw(0), pitch(0) 
{
}

void Camera::setScreenSize(int screenWidth, int screenHeight) 
{ 
    this->screenWidth = screenWidth; 
    this->screenHeight = screenHeight; 
    GraphicsSystem::getInstance()->getGraphics()->setViewPort(0, 0, screenWidth, screenHeight);
}

void Camera::apply()
{
    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
	graphics->setMatrixMode(Graphics::PROJECTION);
	graphics->setIdentityMatrix();
	graphics->setPerspectiveProjection(60.0f, (float) screenWidth / screenHeight, nearPlane, farPlane);
	graphics->setMatrixMode(Graphics::MODELVIEW);
	graphics->setIdentityMatrix();
	graphics->rotateX(-pitch);
	graphics->rotateY(-yaw);
	graphics->translate(-eyeX, -eyeY, -eyeZ);

}
