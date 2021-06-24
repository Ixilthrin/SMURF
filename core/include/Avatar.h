#ifndef GENG_AVATAR_H
#define GENG_AVATAR_H

class Camera;

class Avatar
{
public:
    Avatar();
    virtual ~Avatar();
    void moveForward();
    void moveBackward();
    void strafeLeft();
    void strafeRight();
    virtual void updatePreSimulation(float deltaSeconds);
    virtual void updatePostSimulation();

protected:
    Camera * camera;
    bool moveLeftOnUpdate;
    bool moveRightOnUpdate;
    bool moveForwardOnUpdate;
    bool moveBackwardOnUpdate;
};

#endif
