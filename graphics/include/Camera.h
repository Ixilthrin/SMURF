#ifndef GENG_CAMERA_H
#define GENG_CAMERA_H

class Graphics;

class Camera
{
public:
    static Camera * getInstance() { return &instance; }

    int getScreenWidth() { return screenWidth; }
    int getScreenHeight() { return screenHeight; }
    float getNearPlane() { return nearPlane; }
    float getFarPlane() { return farPlane; }
    float getEyeX() { return eyeX; }
    float getEyeY() { return eyeY; }
    float getEyeZ() { return eyeZ; }
    float getYaw() { return yaw; }
    float getPitch() { return pitch; }

    void setScreenSize(int screenWidth, int screenHeight);
    void setNearPlane() { this->nearPlane = nearPlane; }
    void setFarPlane() { this->farPlane = farPlane; }
    void setPosition(float eyeX, float eyeY, float eyeZ) { this->eyeX = eyeX; this->eyeY = eyeY; this->eyeZ = eyeZ; }
	void setOrientation(float yaw, float pitch) { this->yaw = yaw; this->pitch = pitch; }
    void apply();

private:
	static Camera instance; 
    Camera();
    int screenWidth;
    int screenHeight;
    float nearPlane;
    float farPlane;
    float eyeX;
    float eyeY;
    float eyeZ;
    float yaw;
    float pitch;
};

#endif
