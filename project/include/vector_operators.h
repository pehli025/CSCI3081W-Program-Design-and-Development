/**
 *@file vector_operators.h
 */
#ifndef VECTOR_OPERATORS_H
#define VECTOR_OPERATORS_H

#include "vector2d.h"
#include "vector3d.h"
namespace csci3081 {

// All vector operator overloads do not take in references like standard operators do because references do not
// Allow us to chain together operators because the return value of these operators is unnamed.

/*
* Adds two Vector2Ds together componentwise. 
* @param[in] lhs The vector on the left hand side of the +
* @param[in] rhs The vector on the right hand side of the +
* @return A Vector2D representing the componentwise sum of lhs and rhs.
*/
inline Vector2D operator+(Vector2D lhs, Vector2D rhs) {
    return Vector2D(lhs.getX() + rhs.getX(), lhs.getY() + rhs.getY(), lhs.getZ() + rhs.getZ());
}
/*
* Subtracts two Vector2Ds together componentwise. 
* @param[in] lhs The vector on the left hand side of the -
* @param[in] rhs The vector on the right hand side of the -
* @return A Vector2D representing the componentwise difference of lhs and rhs.
*/
inline Vector2D operator-(Vector2D lhs, Vector2D rhs) {
    return Vector2D(lhs.getX() - rhs.getX(), lhs.getY() - rhs.getY(), lhs.getZ() - rhs.getZ());
}
/*
* Adds a Vector2D and a Vector3D together componentwise. 
* @param[in] lhs The vector2D on the left hand side of the +
* @param[in] rhs The vector3D on the right hand side of the +
* @return A Vector3D representing the componentwise sum of lhs and rhs.
*/
inline Vector3D operator+(Vector2D lhs, Vector3D rhs) {
    return Vector3D(lhs.getX() + rhs.getX(), lhs.getY() + rhs.getY(), lhs.getZ() + rhs.getZ());
}
/*
* Subtracts a Vector2D and a Vector3D together componentwise. 
* @param[in] lhs The vector2D on the left hand side of the -
* @param[in] rhs The vector3D on the right hand side of the -
* @return A Vector3D representing the componentwise difference of lhs and rhs.
*/
inline Vector3D operator-(Vector2D lhs, Vector3D rhs) {
    return Vector3D(lhs.getX() - rhs.getX(), lhs.getY() - rhs.getY(), lhs.getZ() - rhs.getZ());
}
/*
* Adds a Vector2D and a Vector3D together componentwise. 
* @param[in] lhs The vector3D on the left hand side of the -
* @param[in] rhs The vector2D on the right hand side of the -
* @return A Vector3D representing the componentwise sum of lhs and rhs.
*/
inline Vector3D operator+(Vector3D lhs, Vector2D rhs) {
    return Vector3D(lhs.getX() + rhs.getX(), lhs.getY() + rhs.getY(), lhs.getZ() + rhs.getZ());
}
/*
* Subtracts a Vector2D and a Vector3D together componentwise. 
* @param[in] lhs The vector3D on the left hand side of the -
* @param[in] rhs The vector2D on the right hand side of the -
* @return A Vector3D representing the componentwise difference of lhs and rhs.
*/
inline Vector3D operator-(Vector3D lhs, Vector2D rhs) {
    return Vector3D(lhs.getX() - rhs.getX(), lhs.getY() - rhs.getY(), lhs.getZ() - rhs.getZ());
}
/*
* Adds a Vector3D and a Vector3D together componentwise. 
* @param[in] lhs The vector3D on the left hand side of the +
* @param[in] rhs The vector3D on the right hand side of the +
* @return A Vector3D representing the componentwise sum of lhs and rhs.
*/
inline Vector3D operator+(Vector3D lhs, Vector3D rhs) {
    return Vector3D(lhs.getX() + rhs.getX(), lhs.getY() + rhs.getY(), lhs.getZ() + rhs.getZ());
}
/*
* Subtracts a Vector3D and a Vector3D together componentwise. 
* @param[in] lhs The vector3D on the left hand side of the -
* @param[in] rhs The vector3D on the right hand side of the -
* @return A Vector3D representing the componentwise difference of lhs and rhs.
*/
inline Vector3D operator-(Vector3D lhs, Vector3D rhs) {
    return Vector3D(lhs.getX() - rhs.getX(), lhs.getY() - rhs.getY(), lhs.getZ() - rhs.getZ());
}
/*
* Multiplies a Vector3D by a floating scalar componentwise.
* @param[in] v The vector3D on the left hand side of the *
* @param[in] s The floating scalar on the right hand side of the *
* @return A Vector3D representing the product of the vector and s.
*/
inline Vector3D operator*(Vector3D v, float s) {
    return Vector3D(v.getX()*s, v.getY()*s, v.getZ()*s);
}
/*
* Multiplies a Vector3D by a floating scalar componentwise.
* @param[in] s The floating scalar on the left hand side of the *
* @param[in] v The vector3D on the right hand side of the *
* @return A Vector3D representing the product of the vector and s.
*/
inline Vector3D operator*(float s, Vector3D v) {
    return Vector3D(v.getX()*s, v.getY()*s, v.getZ()*s);
}
/*
* Multiplies a floating scalar by a Vector2D componentwise
* @param[in] s The floating scalar on the lhs of the *
* @param[in] v The vecttor2D on the rhs of the *
* @return A Vector2D representing the product of the vector and s.
*/
inline Vector2D operator*(float s, Vector2D v) {
    return Vector2D(v.getX()*s, v.getY()*s, v.getZ()*s);
}
/*
* Multiplies a Vector2D by a floating scalar componentwise.
* @param[in] v The vector2D on the lhs of the *
* @param[in] s The floating scalar on the right hand side of the *
* @return A Vector3D representing the product of the vector and s.
*/
inline Vector2D operator*(Vector2D v, float s) {
    return Vector2D(v.getX()*s, v.getY()*s, v.getZ()*s);
}

}

#endif