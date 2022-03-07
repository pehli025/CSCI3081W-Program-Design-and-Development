#include "gtest/gtest.h"
#include "../include/customer.h"
#include "json_helper.h"
#include "../include/entity_base.h"

#include <iostream>

namespace csci3081 {

using entity_project::IEntity;

class CustomerTest : public ::testing::Test {
    protected:
        picojson::object json1 = JsonHelper::CreateJsonObject();
        picojson::object json2 = JsonHelper::CreateJsonObject();
    virtual void TearDown() {}
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/
TEST_F(CustomerTest, ConstructorDefault) {
    EntityBase::resetEntityCount();
    Customer defaultCust = Customer(json1);
    ASSERT_EQ(0,defaultCust.GetId());
    ASSERT_EQ("Default name",defaultCust.GetName());

    ASSERT_EQ(0,defaultCust.GetPosition().at(0));
    ASSERT_EQ(0,defaultCust.GetPosition().at(1));
    ASSERT_EQ(0,defaultCust.GetPosition().at(2));
    ASSERT_EQ(3,defaultCust.GetPosition().size());

    ASSERT_EQ(0,defaultCust.GetDirection().at(0));
    ASSERT_EQ(0,defaultCust.GetDirection().at(1));
    ASSERT_EQ(0,defaultCust.GetDirection().at(2));
    ASSERT_EQ(3,defaultCust.GetDirection().size());

    ASSERT_FLOAT_EQ(1,defaultCust.GetRadius());

    ASSERT_FLOAT_EQ(0,defaultCust.getDest().getX());
    ASSERT_FLOAT_EQ(0,defaultCust.getDest().getY());
    ASSERT_FLOAT_EQ(0,defaultCust.getDest().getZ());

    EXPECT_FALSE(defaultCust.IsDynamic());
    ASSERT_EQ(0,defaultCust.GetVersion());
    ASSERT_EQ(picojson::value(json1).serialize(),picojson::value(defaultCust.GetDetails()).serialize());

}

TEST_F(CustomerTest, ConstructorParams) {
    EntityBase::resetEntityCount();
    JsonHelper::AddStringToJsonObject(json2,"type","customer");
    std::vector<float> pos;
    pos.push_back(1000);
    pos.push_back(2000);
    pos.push_back(3000);
    JsonHelper::AddStdFloatVectorToJsonObject(json2,"position",pos);

    std::vector<float> dir;
    dir.push_back(-1000);
    dir.push_back(-2000);
    dir.push_back(-3000);
    JsonHelper::AddStdFloatVectorToJsonObject(json2,"direction",dir);
    JsonHelper::AddStringToJsonObject(json2,"name","beta");
    JsonHelper::AddFloatToJsonObject(json2,"radius",4);

    Customer c1 = Customer(json2);
    Customer c2 = Customer(json2);
    ASSERT_EQ(1,c2.GetId());
    ASSERT_FLOAT_EQ(1000,c2.GetPosition().at(0));
    ASSERT_FLOAT_EQ(2000,c2.GetPosition().at(1));
    ASSERT_FLOAT_EQ(3000,c2.GetPosition().at(2));

    ASSERT_NEAR(-.267261242,c2.GetDirection().at(0),.001);
    ASSERT_NEAR(-.534522484,c2.GetDirection().at(1),.001);
    ASSERT_NEAR(-.801783726,c2.GetDirection().at(2),.001);

    ASSERT_EQ(3,c2.GetPosition().size());
    ASSERT_EQ(3,c2.GetDirection().size());

    ASSERT_EQ("beta",c2.GetName());
    ASSERT_FLOAT_EQ(4,c2.GetRadius());
    ASSERT_EQ(0,c2.GetVersion());

    ASSERT_EQ(picojson::value(json2).serialize(),picojson::value(c2.GetDetails()).serialize());
    
}

TEST_F(CustomerTest, SettersCustomer) {
    Customer cust_ = Customer(json1);
    cust_.setDirection({1,2,3});
    cust_.setDynamic(true);
    cust_.setPosition({3,2,1});

    EXPECT_TRUE(cust_.IsDynamic());
    ASSERT_FLOAT_EQ(3, cust_.GetPosition().at(0));
    ASSERT_FLOAT_EQ(2, cust_.GetPosition().at(1));
    ASSERT_FLOAT_EQ(1, cust_.GetPosition().at(2));
    ASSERT_EQ(3,cust_.GetPosition().size());

    ASSERT_FLOAT_EQ(3, cust_.GetDirection().at(2));
    ASSERT_FLOAT_EQ(2, cust_.GetDirection().at(1));
    ASSERT_FLOAT_EQ(1, cust_.GetDirection().at(0));
    ASSERT_EQ(3,cust_.GetDirection().size());
}
}  // namespace csci3081
