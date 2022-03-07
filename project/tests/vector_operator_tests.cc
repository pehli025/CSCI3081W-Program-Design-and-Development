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
TEST_F(VectorTest, OperatorsUnitWorking) {
    // 2D + 2D
    Vector2D res1 = a + b;

    // 2D - 2D
    Vector2D res2 = b - a;

    // 2D + 3D
    Vector3D res3 = b + c;

    // 2D - 3D
    Vector3D res4 = a - d;

    // 3D + 2D
    Vector3D res5 = c + a;

    // 3D - 2D
    Vector3D res6 = d - b;

    // 3D + 3D
    Vector3D res7 = c + d;

    // 3D - 3D
    Vector3D res8 = d - c;

    Vector2D res9 = 2 * a;

    Vector2D res10 = b * 2;

    Vector3D res11 = 3 * c;

    Vector3D res12 = d * 4;

    ASSERT_FLOAT_EQ(5.6,res1.getX());
    ASSERT_FLOAT_EQ(0,res1.getY());
    ASSERT_FLOAT_EQ(9.55,res1.getZ());

    ASSERT_FLOAT_EQ(2.6,res2.getX());
    ASSERT_FLOAT_EQ(0,res2.getY());
    ASSERT_FLOAT_EQ(3.05,res2.getZ());

    ASSERT_FLOAT_EQ(11.35,res3.getX());
    ASSERT_FLOAT_EQ(8.3,res3.getY());
    ASSERT_FLOAT_EQ(16.0,res3.getZ());

    ASSERT_FLOAT_EQ(-8.6,res4.getX());
    ASSERT_FLOAT_EQ(-11.5,res4.getY());
    ASSERT_FLOAT_EQ(-8.85,res4.getZ());

    ASSERT_FLOAT_EQ(8.75,res5.getX());
    ASSERT_FLOAT_EQ(8.3,res5.getY());
    ASSERT_FLOAT_EQ(12.95,res5.getZ());

    ASSERT_FLOAT_EQ(6.0,res6.getX());
    ASSERT_FLOAT_EQ(11.5,res6.getY());
    ASSERT_FLOAT_EQ(5.8,res6.getZ());

    ASSERT_FLOAT_EQ(17.35,res7.getX());
    ASSERT_FLOAT_EQ(19.8,res7.getY());
    ASSERT_FLOAT_EQ(21.8,res7.getZ());

    ASSERT_FLOAT_EQ(2.85,res8.getX());
    ASSERT_FLOAT_EQ(3.2,res8.getY());
    ASSERT_FLOAT_EQ(2.4,res8.getZ());

    ASSERT_FLOAT_EQ(3.0,res9.getX());
    ASSERT_FLOAT_EQ(6.5,res9.getZ());
    ASSERT_FLOAT_EQ(0,res9.getY());

    ASSERT_FLOAT_EQ(8.2,res10.getX());
    ASSERT_FLOAT_EQ(0,res10.getY());
    ASSERT_FLOAT_EQ(12.6,res10.getZ());

    ASSERT_FLOAT_EQ(21.75,res11.getX());
    ASSERT_FLOAT_EQ(24.9,res11.getY());
    ASSERT_FLOAT_EQ(29.1,res11.getZ());

    ASSERT_FLOAT_EQ(40.4,res12.getX());
    ASSERT_FLOAT_EQ(46.0,res12.getY());
    ASSERT_FLOAT_EQ(48.4,res12.getZ());
}
}  // namespace csci3081