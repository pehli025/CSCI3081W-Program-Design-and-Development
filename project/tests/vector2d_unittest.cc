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
TEST_F(VectorTest, Magnitude2DWorking) {
    ASSERT_NEAR(3.5794,a.magnitude(),.001);
    ASSERT_NEAR(7.516648,b.magnitude(),.001);
}

TEST_F(VectorTest, Normalize2DWorking) {
    a.normalize();
    b.normalize();
    c.normalize();
    d.normalize();

    ASSERT_FLOAT_EQ(1,a.magnitude());
    ASSERT_FLOAT_EQ(1,b.magnitude());

    ASSERT_NEAR(.419058,a.getX(),.001);
    ASSERT_NEAR(.907959,a.getZ(),.001);
    ASSERT_NEAR(0,a.getY(),.001);

    Vector2D zero2D = Vector2D(0,0,0);

    ASSERT_FLOAT_EQ(0,zero2D.magnitude());
}

TEST_F(VectorTest, Constructor2DWorking) {
    // Y coordinate is always 0
    Vector2D nonZeroY = Vector2D(1.5,.2,3); 
    ASSERT_FLOAT_EQ(1.5,nonZeroY.getX());
    ASSERT_FLOAT_EQ(0,nonZeroY.getY());
    ASSERT_FLOAT_EQ(3,nonZeroY.getZ());

    ASSERT_FLOAT_EQ(1.5,a.getX());
    ASSERT_FLOAT_EQ(0,a.getY());
    ASSERT_FLOAT_EQ(3.25,a.getZ());

    ASSERT_FLOAT_EQ(4.1,b.getX());
    ASSERT_FLOAT_EQ(0,b.getY());
    ASSERT_FLOAT_EQ(6.3,b.getZ());
}
TEST_F(VectorTest, Setters2DWorking) {
    // Y coordinate is always 0
    Vector2D nonZeroY = Vector2D(); 
    nonZeroY.setX(1.5);
    nonZeroY.setY(0);
    nonZeroY.setZ(3);

    ASSERT_FLOAT_EQ(1.5,nonZeroY.getX());
    ASSERT_FLOAT_EQ(0,nonZeroY.getY());
    ASSERT_FLOAT_EQ(3,nonZeroY.getZ());

    ASSERT_FLOAT_EQ(1.5,a.getX());
    ASSERT_FLOAT_EQ(0,a.getY());
    ASSERT_FLOAT_EQ(3.25,a.getZ());

    ASSERT_FLOAT_EQ(4.1,b.getX());
    ASSERT_FLOAT_EQ(0,b.getY());
    ASSERT_FLOAT_EQ(6.3,b.getZ());
}
}  // namespace csci3081