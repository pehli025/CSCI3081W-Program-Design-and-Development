#include "gtest/gtest.h"
#include "../include/battery.h"

#include <iostream>

namespace csci3081 {

class BatteryTest : public ::testing::Test {
    protected:

        // Keep it simple and just initialize member variables
        // void SetUp() override {}
        // Tear down not needed
        // void TearDown override {}
        Battery defaultBattery = Battery();
        Battery weakBattery = Battery(2);
        Battery negativeBattery = Battery(-1);
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(BatteryTest, ConstructorWorking) {
    ASSERT_FLOAT_EQ(10000,defaultBattery.getMaxLife());
    ASSERT_FLOAT_EQ(2,weakBattery.getMaxLife());
    ASSERT_FLOAT_EQ(-1,negativeBattery.getMaxLife());

    ASSERT_FLOAT_EQ(10000,defaultBattery.getRemLife());
    ASSERT_FLOAT_EQ(2,weakBattery.getRemLife());
    ASSERT_FLOAT_EQ(-1,negativeBattery.getRemLife());
}

TEST_F(BatteryTest, UseBattery) {
    defaultBattery.useBattery(5001);
    defaultBattery.useBattery(5001);
    ASSERT_FLOAT_EQ(-2,defaultBattery.getRemLife());
    ASSERT_FLOAT_EQ(10000,defaultBattery.getMaxLife());
    weakBattery.useBattery(.5);
    weakBattery.useBattery(.5);
    weakBattery.useBattery(.5);
    ASSERT_FLOAT_EQ(.5,weakBattery.getRemLife());
    ASSERT_FLOAT_EQ(2,weakBattery.getMaxLife());
    weakBattery.useBattery(.5);
    ASSERT_FLOAT_EQ(0,weakBattery.getRemLife());
    ASSERT_FLOAT_EQ(2,weakBattery.getMaxLife());
    weakBattery.useBattery(.5);
    ASSERT_FLOAT_EQ(0,weakBattery.getRemLife());
    ASSERT_FLOAT_EQ(2,weakBattery.getMaxLife());
    negativeBattery.useBattery(.5);
    ASSERT_FLOAT_EQ(-1,negativeBattery.getRemLife());
    ASSERT_FLOAT_EQ(-1,negativeBattery.getMaxLife());
}

}  // namespace csci3081