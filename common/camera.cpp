#include "camera.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <utils.hpp>

Camera::Camera()
{
    _near = 0.1;
    _far = 100;
    _heading = 0;
    _pitch = 0;
    _roll = 0;
    _position = glm::vec3(0, 0, 0);
    _fovX = degToRad(100);
    _fovY = degToRad(75);
}

float Camera::getNear()const
{
    return _near;
}

float Camera::getFar()const
{
    return _far;
}

float Camera::getHeading()const
{
    return _heading;
}

float Camera::getPitch()const
{
    return _pitch;
}

float Camera::getRoll()const
{
    return _roll;
}

const glm::vec3& Camera::getPosition()const
{
    return _position;
}

float Camera::getFovX()const
{
    return _fovX;
}

float Camera::getFovY()const
{
    return _fovY;
}

void Camera::setNear(float nearDist)
{
    _near = nearDist;
}

void Camera::setFar(float farDist)
{
    _far = farDist;
}

void Camera::setHeading(float heading)
{
    _heading = heading;
}

void Camera::setPitch(float pitch)
{
    _pitch = pitch;
}

void Camera::setRoll(float roll)
{
    _roll = roll;
}

void Camera::setPosition(const glm::vec3& position)
{
    _position = position;
}

void Camera::setFovX(float fovX)
{
    _fovX = fovX;
}

void Camera::setFovY(float fovY)
{
    _fovY = fovY;
}

#include <iostream>
using namespace std;

void Camera::translateLocal(const glm::vec3& disp)
{
    glm::mat4 r =
            glm::rotate(_heading, glm::vec3(0, 1, 0)) *
            glm::rotate(_pitch, glm::vec3(1, 0, 0)) *
            glm::rotate(_roll, glm::vec3(0, 0, 1));
    glm::vec4 v = r * glm::vec4(disp, 1);
    _position += glm::vec3(v.x, v.y, v.z);
}

glm::mat4 Camera::getTransfMat()const
{
    glm::mat4 p = glm::perspective(_fovY, _fovX/_fovY, _near, _far);
    glm::mat4 v = getViewMat();
    glm::mat4 res = p * v;
    return res;
}

glm::mat4 Camera::getProjMat()const
{
    glm::mat4 p = glm::perspective(_fovY, _fovX/_fovY, _near, _far);
    return p;
}

glm::mat4 Camera::getViewMat()const
{
    glm::mat4 r =
            glm::rotate(_heading, glm::vec3(0, 1, 0)) *
            glm::rotate(_pitch, glm::vec3(1, 0, 0)) *
            glm::rotate(_roll, glm::vec3(0, 0, 1));
    glm::mat4 t = glm::translate(_position);
    glm::mat4 view = t * r;
    view = glm::inverse(view);
    return view;
}
