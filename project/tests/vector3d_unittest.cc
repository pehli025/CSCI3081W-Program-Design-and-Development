#include "gtest/gtest.h"
#include <EntityProject/project_settings.h>
#include "../include/delivery_simulation.h"
#include <EntityProject/entity.h>
#include "json_helper.h"
#include "../include/vector2d.h"
#include "../include/vector3d.h"
#include "../include/vector_operators.h"

#include <iostream>

namespace csci3081 {

class VectorTest : public ::testing::Test {
    protected:

        // Keep it simple and just initialize member variables
        // void SetUp() override {}
        // Tear down not needed
        // void TearDown override {}
        Vector2D a = Vector2D(1.5,0,3.25);
        Vector2D b = Vector2D(4.1,0,6.3);
        Vector3D c = Vector3D(7.25,8.3,9.7);
        Vector3D d = Vector3D(10.1,11.5,12.1);
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(VectorTest, Magnitude3DWorking) {
    ASSERT_NEAR(14.68136,c.magnitude(),.001);
    ASSERT_NEAR(19.510766,d.magnitude(),.001);
}

TEST_F(VectorTest, Normalize3DWorking) {
    c.normalize();
    d.normalize();

    ASSERT_FLOAT_EQ(1,c.magnitude());
    ASSERT_FLOAT_EQ(1,d.magnitude());

    ASSERT_NEAR(.5176629,d.getX(),.001);
    ASSERT_NEAR(.589418,d.getY(),.001);
    ASSERT_NEAR(.6201704,d.getZ(),.001);

    Vector3D zero3D = Vector3D(0,0,0);

    ASSERT_FLOAT_EQ(0,zero3D.magnitude());
}

TEST_F(VectorTest, Constructor3DWorking) {

    ASSERT_FLOAT_EQ(10.1,d.getX());
    ASSERT_FLOAT_EQ(11.5,d.getY());
    ASSERT_FLOAT_EQ(12.1,d.getZ());

    ASSERT_FLOAT_EQ(7.25,c.getX());
    ASSERT_FLOAT_EQ(8.3,c.getY());
    ASSERT_FLOAT_EQ(9.7,c.getZ());
}

TEST_F(VectorTest, Setters3DWorkings) {
    Vector3D vector_ = Vector3D(); 
    vector_.setX(1.2);
    vector_.setY(2.6);
    vector_.setZ(3.1);

    ASSERT_FLOAT_EQ(1.2,vector_.getX());
    ASSERT_FLOAT_EQ(2.6,vector_.getY());
    ASSERT_FLOAT_EQ(3.1,vector_.getZ());
}
}  // namespace csci3081