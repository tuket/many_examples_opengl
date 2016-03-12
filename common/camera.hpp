#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/common.hpp>

class Camera
{

public:

    Camera();

    float getNear()const;
    float getFar()const;
    float getHeading()const;
    float getPitch()const;
    float getRoll()const;
    const glm::vec3& getPosition()const;
    float getFovX()const;
    float getFovY()const;

    void setNear(float nearDist);
    void setFar(float farDist);
    void setHeading(float heading);
    void setPitch(float pitch);
    void setRoll(float roll);
    void setPosition(const glm::vec3& position);
    void setFovX(float fovX);
    void setFovY(float fovY);

    void translateLocal(const glm::vec3& disp);

    glm::mat4 getTransfMat()const;

protected:

    float _near, _far;
    float _heading, _pitch, _roll;
    glm::vec3 _position;
    float _fovX, _fovY;

};

#endif
