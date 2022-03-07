/**
 *@file vector3d.h
 */
#ifndef VECTOR3D_H
#define VECTOR3D_H
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <vector>
namespace csci3081 {
/**
 * @brief A class representing a 3D vector. Helps with doing vector math for calculation in the simulation
 * 
 */
class Vector3D {
    public:
        /*
        * Constructor: Initializes the vector's coordinates to the values passed in. Defaults to 0.
        * @param[in] x_ The x coordinate of the vector.
        * @param[in] y_ The y coordinate of the vector.
        * @param[in] z_ The z coordinate of the vector.
        */
        Vector3D(float x_ = 0, float y_ = 0, float z_ = 0);

        /*
        * Converts the vector object to a std::vector<float> 
        * @return A std::vector<float> of size 3 representing the coordinates of the vector.
        */
        std::vector<float> toStdVector();

        /*
        * Finds the length of the vector object.
        * @return A floating point number representing the length of the vector.
        */
        float magnitude();

        /*
        * Takes the vector object and scales all its components so that the length of the vector is 1 while maintaining the same direction.
        */
        void normalize();

        /*
        * Returns the vector's x coordinate.
        */
        float getX() const;
        /*
        * Returns the vector's y coordinate.
        */
        float getY() const;
        /*
        * Returns the vector's z coordinate.
        */
        float getZ() const;
        /*
        * Sets the vector's x coordinate.
        * @param[in] x_ The new x coordinate.
        */
        void setX(float x_);
        /*
        * Sets the vector's y coordinate.
        * @param[in] y_ The new y coordinate.
        */
        void setY(float y_);
        /*
        * Sets the vector's x coordinate.
        * @param[in] z_ The new y coordinate.
        */
        void setZ(float z_);

        /*
        * A static class method to convert a std::vector<float> to a Vector3D.
        * @param[in] v A std::vector<float> of size 3 to convert to a Vector3D.
        * @return A Vector3D that has the same x, y, and z coordinates as the std::vector<float> passed in.
        */
        static Vector3D convertToVector3D(std::vector<float> v);

    protected:
        float x;
        float y;
        float z;
};

}  // namespace csci3081


#endif
