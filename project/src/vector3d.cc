#include "vector3d.h"
#include "vector2d.h"
#include <vector>
#include <math.h>
#include <iostream>

namespace csci3081 {

    Vector3D::Vector3D(float x_, float y_, float z_) : x{x_}, y{y_}, z{z_} {}
    float Vector3D::magnitude() {
        float a = sqrt(getX()*getX() + getY()*getY() + getZ()*getZ());
        return a;
     }

    void Vector3D::normalize() {
        float mag = this->magnitude();
        if (fabs(mag) < .0001) { x = 0; y = 0; z = 0; return; } // Magnitude is 0. Can't normalize
        x = x / mag;
        y = y / mag;
        z = z / mag;
    }

    float Vector3D::getX() const { return x; }

    float Vector3D::getY() const { return y; }

    float Vector3D::getZ() const { return z; }

    void Vector3D::setX(float x_) { x = x_; }

    void Vector3D::setY(float y_) { y = y_; }

    void Vector3D::setZ(float z_) { z = z_; }

    Vector3D Vector3D::convertToVector3D(std::vector<float> v) {
        float x = v.at(0);
        float y = v.at(1);
        float z = v.at(2);
        return Vector3D(x,y,z);
    }

    std::vector<float> Vector3D::toStdVector() {
        std::vector<float> v;
        v.push_back(getX());
        v.push_back(getY());
        v.push_back(getZ());
        return v;
    }
    
}
