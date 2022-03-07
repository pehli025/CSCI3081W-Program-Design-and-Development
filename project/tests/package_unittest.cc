#include "gtest/gtest.h"
#include "package.h"
#include "customer.h"
#include "json_helper.h"
#include "drone.h"
#include "entity_base.h"
#include <iostream>

namespace csci3081 {

class PackageTest : public ::testing::Test {
    protected:
        picojson::object p1 = JsonHelper::CreateJsonObject();
        picojson::object p2 = JsonHelper::CreateJsonObject();
        picojson::object p3 = JsonHelper::CreateJsonObject();
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(PackageTest, DefaultConsTest) {
    EntityBase::resetEntityCount();
    Drone d1 = Drone(p1);
    Drone d2 = Drone(p1);
    Drone d3 = Drone(p1);
    Package defPackage = Package(p1);
    ASSERT_EQ("Default name",defPackage.GetName());
    ASSERT_EQ(0,defPackage.GetPosition().at(0));
    ASSERT_EQ(0,defPackage.GetPosition().at(1));
    ASSERT_EQ(0,defPackage.GetPosition().at(2));
    EXPECT_EQ(3,defPackage.GetPosition().size());

    ASSERT_EQ(0,defPackage.GetDirection().at(0));
    ASSERT_EQ(0,defPackage.GetDirection().at(1));
    ASSERT_EQ(0,defPackage.GetDirection().at(2));
    ASSERT_EQ(3,defPackage.GetDirection().size());

    ASSERT_EQ(1,defPackage.GetRadius());
    ASSERT_EQ(3,defPackage.GetId());
    EXPECT_TRUE(defPackage.getCustomer() == nullptr);
    ASSERT_EQ(0,defPackage.GetVersion());
    ASSERT_EQ(picojson::value(p1).serialize(),picojson::value(defPackage.GetDetails()).serialize());
}

TEST_F(PackageTest, GeneralConsTest) {
    EntityBase::resetEntityCount();
    JsonHelper::AddStringToJsonObject(p2,"type","package");
    std::vector<float> p2Pos;
    p2Pos.push_back(10);
    p2Pos.push_back(15);
    p2Pos.push_back(20);
    JsonHelper::AddStdFloatVectorToJsonObject(p2,"position",p2Pos);

    std::vector<float> p2Dir;
    p2Dir.push_back(2);
    p2Dir.push_back(3);
    p2Dir.push_back(4);
    JsonHelper::AddStdFloatVectorToJsonObject(p2,"direction",p2Dir);
    JsonHelper::AddStringToJsonObject(p2,"name","charlie");
    JsonHelper::AddFloatToJsonObject(p2,"radius",4);

    Package p = Package(p2);
    ASSERT_EQ(10,p.GetPosition().at(0));
    ASSERT_EQ(15,p.GetPosition().at(1));
    ASSERT_EQ(20,p.GetPosition().at(2));

    ASSERT_NEAR(.371390676,p.GetDirection().at(0),.001);
    ASSERT_NEAR(.55706015,p.GetDirection().at(1),.001);
    ASSERT_NEAR(.742781353,p.GetDirection().at(2),.001);

    ASSERT_EQ("charlie",p.GetName());
    ASSERT_FLOAT_EQ(4,p.GetRadius());

    Customer* c = new Customer(p1);
    p.setCustomer(c);
    EXPECT_FALSE(p.getCustomer() == nullptr);
    ASSERT_EQ(c, p.getCustomer());
    ASSERT_FLOAT_EQ(0,p.getDestination().getX());
    ASSERT_FLOAT_EQ(0,p.getDestination().getY());
    ASSERT_FLOAT_EQ(0,p.getDestination().getZ());

    EXPECT_FALSE(p.IsDynamic());
    ASSERT_EQ(0,p.GetId());

    ASSERT_EQ(0,p.GetVersion());
    ASSERT_EQ(picojson::value(p2).serialize(),picojson::value(p.GetDetails()).serialize());
}

TEST_F(PackageTest, SettersTest) {
    Package pack_ = Package(p1);

    pack_.setDirection({1,2,3});
    pack_.setDynamic(true);
    pack_.setPosition({3,2,1});

    EXPECT_TRUE(pack_.IsDynamic());
    ASSERT_FLOAT_EQ(3, pack_.GetPosition().at(0));
    ASSERT_FLOAT_EQ(2, pack_.GetPosition().at(1));
    ASSERT_FLOAT_EQ(1, pack_.GetPosition().at(2));
    ASSERT_EQ(3,pack_.GetPosition().size());

    ASSERT_FLOAT_EQ(3, pack_.GetDirection().at(2));
    ASSERT_FLOAT_EQ(2, pack_.GetDirection().at(1));
    ASSERT_FLOAT_EQ(1, pack_.GetDirection().at(0));
    ASSERT_EQ(3,pack_.GetDirection().size());
}

}  // namespace csci3081
