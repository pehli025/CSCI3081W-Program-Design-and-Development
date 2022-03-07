#include "vector2d.h"
#include "vector3d.h"
#include <vector>

namespace csci3081 {

    Vector2D::Vector2D(float x_, float y_, float z_) : Vector3D(x_, y_, z_) {
        setY(0);
    }

    Vector2D Vector2D::convertToVector2D(std::vector<float> v) {
        float x = v.at(0);
        float y = v.at(1);
        float z = v.at(2);
        return Vector2D(x,y,z);
    }

}
