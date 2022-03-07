#include "gtest/gtest.h"
#include "../include/vector2d.h"
#include "../include/vector3d.h"
#include "../include/vector_operators.h"

#include <iostream>

namespace csci3081 {

class ConversionTest : public ::testing::Test {
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

TEST_F(ConversionTest, ConvertTo3D) {
    std::vector<float> t{1.2,4.4,7.6};
    Vector3D converted = Vector3D::convertToVector3D(t);
    ASSERT_FLOAT_EQ(1.2,converted.getX());
    ASSERT_FLOAT_EQ(4.4,converted.getY());
    ASSERT_FLOAT_EQ(7.6,converted.getZ());
}

TEST_F(ConversionTest, ConvertTo2D) {
    std::vector<float> t{1.2,4.4,7.6}; // Nonzero y component gets murdered
    Vector2D converted = Vector2D::convertToVector2D(t);
    ASSERT_FLOAT_EQ(1.2,converted.getX());
    ASSERT_FLOAT_EQ(0,converted.getY());
    ASSERT_FLOAT_EQ(7.6,converted.getZ());
}

TEST_F(ConversionTest, ConvertFrom2D) {
    std::vector<float> t = a.toStdVector();
    ASSERT_FLOAT_EQ(1.5,t.at(0));
    ASSERT_FLOAT_EQ(0,t.at(1));
    ASSERT_FLOAT_EQ(3.25,t.at(2));
}

TEST_F(ConversionTest, ConvertFrom3D) {
    std::vector<float> t = c.toStdVector();
    ASSERT_FLOAT_EQ(7.25,t.at(0));
    ASSERT_FLOAT_EQ(8.3,t.at(1));
    ASSERT_FLOAT_EQ(9.7,t.at(2));
}
}  // namespace csci3081