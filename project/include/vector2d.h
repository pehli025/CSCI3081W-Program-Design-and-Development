/**
 *@file vector2d.h
 */
#ifndef VECTOR2D_H
#define VECTOR2D_H
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "vector3d.h"
#include "vector"
namespace csci3081 {
/**
 * @brief A class to represent 2D vectors. Will be used to represent ground entities such as trucks.
 * 
 */
class Vector2D : public Vector3D{

    public:

        /*
        * Constructor: Constructor for a Vector 2D. Y is initialized to 0  no matter what value of y is passed in. All values default to 0.
        */
        Vector2D(float x_ = 0, float y_ = 0, float z_ = 0);

        /*
        * Static class method to convert any std::vector<float> to a Vector2D. Useful for doing math with vectors.
        */
        static Vector2D convertToVector2D(std::vector<float> v);
};

}  // namespace csci3081


#endif