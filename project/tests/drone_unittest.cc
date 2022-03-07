#include "gtest/gtest.h"
#include "json_helper.h"
#include "customer.h"
#include "drone.h"
#include "robot.h"
#include "package.h"
#include "../include/entity_base.h"
#include <EntityProject/entity.h>
#include <EntityProject/project_settings.h>
#include <EntityProject/web_scene_viewer.h>
#include <EntityProject/osm_graph_parser.h>
#include <EntityProject/graph.h>
#include <EntityProject/facade/delivery_system.h>
#include <EntityProject/entity_console_logger.h>
#include <cmath>
#include <iostream>

namespace csci3081 {

using entity_project::IEntity;
using namespace entity_project;
// Implicitly tests the methods in the abstract class DeliveryVehicle
class DroneTest : public ::testing::Test {
    protected:
        picojson::object drone1 = JsonHelper::CreateJsonObject();
        picojson::object drone2 = JsonHelper::CreateJsonObject();
        picojson::object drone3 = JsonHelper::CreateJsonObject();
        picojson::object drone4 = JsonHelper::CreateJsonObject();
        picojson::object drone5 = JsonHelper::CreateJsonObject();
        picojson::object drone6 = JsonHelper::CreateJsonObject();
        picojson::object drone7 = JsonHelper::CreateJsonObject();
        picojson::object drone8 = JsonHelper::CreateJsonObject();
        picojson::object drone9 = JsonHelper::CreateJsonObject();
        picojson::object drone10 = JsonHelper::CreateJsonObject();
        picojson::object drone11 = JsonHelper::CreateJsonObject();

        picojson::object package = JsonHelper::CreateJsonObject();
        picojson::object customer = JsonHelper::CreateJsonObject();
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(DroneTest, DroneConstructorDefaultPicoJson) {
    EntityBase::resetEntityCount();
    Drone d = Drone(drone1);
    EXPECT_EQ("Default name",d.GetName());

    ASSERT_FLOAT_EQ(0,d.GetPosition().at(0));
    ASSERT_FLOAT_EQ(0,d.GetPosition().at(1));
    ASSERT_FLOAT_EQ(0,d.GetPosition().at(2));
    EXPECT_EQ(3,d.GetPosition().size());

    ASSERT_FLOAT_EQ(0,d.GetDirection().at(0));
    ASSERT_FLOAT_EQ(0,d.GetDirection().at(1));
    ASSERT_FLOAT_EQ(0,d.GetDirection().at(2));
    EXPECT_EQ(3,d.GetDirection().size());

    ASSERT_FLOAT_EQ(1,d.GetRadius());
    ASSERT_FLOAT_EQ(0,d.GetId());

    ASSERT_EQ(false,d.holdingPackage());
    ASSERT_EQ(false,d.getMoving());
    ASSERT_EQ(false,d.IsDynamic());
    ASSERT_EQ(false,d.holdingPackage());
    ASSERT_EQ(false,d.getMoving());
    ASSERT_FLOAT_EQ(10,d.GetSpeed());

    ASSERT_EQ(0,d.GetVersion());
    ASSERT_EQ(picojson::value(drone1).serialize(),picojson::value(d.GetDetails()).serialize());
}

TEST_F(DroneTest, DroneConstructorPicoJson) {
    EntityBase::resetEntityCount();
    Drone d1 = Drone(drone1);

    JsonHelper::AddStringToJsonObject(drone2,"type","drone");
    std::vector<float> drone2Pos;
    drone2Pos.push_back(10);
    drone2Pos.push_back(15);
    drone2Pos.push_back(20);
    JsonHelper::AddStdFloatVectorToJsonObject(drone2,"position",drone2Pos);

    std::vector<float> drone2Dir;
    drone2Dir.push_back(2);
    drone2Dir.push_back(3);
    drone2Dir.push_back(4);
    JsonHelper::AddStdFloatVectorToJsonObject(drone2,"direction",drone2Dir);
    JsonHelper::AddStringToJsonObject(drone2,"name","charlie");
    JsonHelper::AddFloatToJsonObject(drone2,"radius",4);
    JsonHelper::AddFloatToJsonObject(drone2,"speed",100);

    Drone d2 = Drone(drone2);
    ASSERT_EQ("drone",d2.GetType());
    EXPECT_EQ("charlie",d2.GetName());
    ASSERT_FLOAT_EQ(4,d2.GetRadius());
    ASSERT_FLOAT_EQ(100,d2.GetSpeed());

    ASSERT_FLOAT_EQ(10,d2.GetPosition().at(0));
    ASSERT_FLOAT_EQ(15,d2.GetPosition().at(1));
    ASSERT_FLOAT_EQ(20,d2.GetPosition().at(2));
    EXPECT_EQ(3,d2.GetPosition().size());

    ASSERT_NEAR(.371390676,d2.GetDirection().at(0),.001);
    ASSERT_NEAR(.55706015,d2.GetDirection().at(1),.001);
    ASSERT_NEAR(.742781353,d2.GetDirection().at(2),.001);
    EXPECT_EQ(3,d2.GetDirection().size());
    EXPECT_EQ(0,d1.GetId());
    EXPECT_EQ(1,d2.GetId());
    EXPECT_EQ(0,d2.GetVersion());
    ASSERT_EQ(false,d2.holdingPackage());
    ASSERT_EQ(false,d2.getMoving());
    EXPECT_EQ(false,d2.IsDynamic());
    ASSERT_TRUE(nullptr == d2.getPackage());

    ASSERT_EQ(0,d1.GetVersion());
    ASSERT_EQ(0,d2.GetVersion());
    ASSERT_EQ(0,d2.getQueuedDeliveries().size());

    ASSERT_EQ(picojson::value(drone2).serialize(),picojson::value(d2.GetDetails()).serialize());
}

TEST_F(DroneTest, DroneMovement) {
    JsonHelper::AddStringToJsonObject(drone4,"type","drone");
    std::vector<float> drone2Dir;
    drone2Dir.push_back(1);
    drone2Dir.push_back(0);
    drone2Dir.push_back(0);
    JsonHelper::AddStdFloatVectorToJsonObject(drone4,"direction",drone2Dir);
    JsonHelper::AddStringToJsonObject(drone4,"name","Standard");
    JsonHelper::AddFloatToJsonObject(drone4,"radius",1);
    JsonHelper::AddFloatToJsonObject(drone4,"speed",1);
    Drone moving = Drone(drone4);

    // Overshoot our point, clamp to dest
    std::vector<float> dest{1.5,0,0};
    moving.moveTo(dest,2);
    ASSERT_NEAR(1.5,moving.GetPosition().at(0),.001);
    ASSERT_NEAR(0,moving.GetPosition().at(1),.001);
    ASSERT_NEAR(0,moving.GetPosition().at(2),.001);
    ASSERT_NEAR(1.0,moving.GetDirection().at(0),.001);
    ASSERT_NEAR(0,moving.GetDirection().at(1),.001);
    ASSERT_NEAR(0,moving.GetDirection().at(2),.001);

    // Undershoot
    std::vector<float> origin{0,0,0};
    moving.moveTo(origin,.5);
    ASSERT_NEAR(1.0,moving.GetPosition().at(0),.001);
    ASSERT_NEAR(0,moving.GetPosition().at(1),.001);
    ASSERT_NEAR(0,moving.GetPosition().at(2),.001);

    // Test direction switch
    ASSERT_NEAR(-1.0,moving.GetDirection().at(0),.001);
    ASSERT_NEAR(0,moving.GetDirection().at(1),.001);
    ASSERT_NEAR(0,moving.GetDirection().at(2),.001);

    // Exact
    moving.moveTo(origin,1);
    ASSERT_NEAR(0,moving.GetPosition().at(0),.001);
    ASSERT_NEAR(0,moving.GetPosition().at(1),.001);
    ASSERT_NEAR(0,moving.GetPosition().at(2),.001);
    
}
TEST_F(DroneTest, NoChangeInState) {
    EntityBase::resetEntityCount();
    JsonHelper::AddStringToJsonObject(drone5,"type","drone");
    std::vector<float> drone2Dir;
    drone2Dir.push_back(1);
    drone2Dir.push_back(0);
    drone2Dir.push_back(0);
    JsonHelper::AddStdFloatVectorToJsonObject(drone5,"direction",drone2Dir);
    JsonHelper::AddStringToJsonObject(drone5,"name","Standard");
    JsonHelper::AddFloatToJsonObject(drone5,"radius",1);
    JsonHelper::AddFloatToJsonObject(drone5,"speed",1);
    Drone d = Drone(drone5);

    EXPECT_FALSE(d.getMoving());
    EXPECT_FALSE(d.holdingPackage());
    d.setPackage(nullptr);
    EXPECT_FALSE(d.getMoving());
    EXPECT_FALSE(d.holdingPackage());
}
TEST_F(DroneTest, DroneRescheduleFlag) {
   EntityBase::resetEntityCount();
   JsonHelper::AddStringToJsonObject(drone6,"type","drone");
   std::vector<float> drone2Dir;
   drone2Dir.push_back(1);
   drone2Dir.push_back(0);
   drone2Dir.push_back(0);
   JsonHelper::AddStdFloatVectorToJsonObject(drone6,"direction",drone2Dir);
   JsonHelper::AddStringToJsonObject(drone6,"name","Standard");
   JsonHelper::AddFloatToJsonObject(drone6,"radius",1);
   JsonHelper::AddFloatToJsonObject(drone6,"speed",1);
   JsonHelper::AddFloatToJsonObject(drone6, "battery_capacity", 1);
   Drone d = Drone(drone6);

   std::vector<float> custpos = {10, 10, 10};
   std::vector<float> custdir = {0,1,0};
   JsonHelper::AddStringToJsonObject(customer,"type","customer");
   JsonHelper::AddStdFloatVectorToJsonObject(customer,"position", custpos);
   JsonHelper::AddStdFloatVectorToJsonObject(customer,"direction", custdir);
   JsonHelper::AddStringToJsonObject(customer,"name","joe");
   JsonHelper::AddFloatToJsonObject(customer,"radius",4);
   Customer cust = Customer(customer);

   std::vector<float> packPos{1,0,0};
   JsonHelper::AddStringToJsonObject(package,"type","package");
   JsonHelper::AddStdFloatVectorToJsonObject(package,"position",packPos);
   JsonHelper::AddStdFloatVectorToJsonObject(package,"direction",packPos);
   JsonHelper::AddStringToJsonObject(package,"name","package");
   JsonHelper::AddFloatToJsonObject(package,"radius",1);
   Package p = Package(package);

   std::vector<std::vector<float>> toPackage;
   std::vector<std::vector<float>> packageToCust;
   
   toPackage.push_back(drone2Dir);
   packageToCust.push_back(drone2Dir);
   packageToCust.push_back(custpos);
   d.setToPackage(toPackage);
   d.setPackageToCust(packageToCust);
   toPackage.insert(toPackage.end(),packageToCust.begin(),packageToCust.end());
   p.setCustomer(&cust);
   d.setPackage(&p);
   d.setPath(toPackage);

   ASSERT_FLOAT_EQ(1, d.getBattery().getRemLife()); 
   EXPECT_FALSE(d.getRescheduleFlag());
   d.update(1);
   d.update(1);
   EXPECT_TRUE(d.getRescheduleFlag());

   d.update(1); // Update to check if battery drains at idle
   ASSERT_FLOAT_EQ(0, d.getBattery().getRemLife());
}

TEST_F(DroneTest, DroneRadius) {
    EntityBase::resetEntityCount();
    JsonHelper::AddStringToJsonObject(drone7,"type","drone");
    std::vector<float> drone2Dir = {1,0,0};
    std::vector<float> pos = {0,0,0};
    JsonHelper::AddStdFloatVectorToJsonObject(drone7,"direction",drone2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(drone7,"position",pos);
    JsonHelper::AddStringToJsonObject(drone7,"name","Standard");
    JsonHelper::AddFloatToJsonObject(drone7,"radius",.5);
    JsonHelper::AddFloatToJsonObject(drone7,"speed",1);
    JsonHelper::AddFloatToJsonObject(drone7, "battery_capacity", 100);
    Drone d1 = Drone(drone7);

    std::vector<float> custpos = {10, 10, 10};
    std::vector<float> custdir = {0,1,0};
    JsonHelper::AddStringToJsonObject(customer,"type","customer");
    JsonHelper::AddStdFloatVectorToJsonObject(customer,"position", custpos);
    JsonHelper::AddStdFloatVectorToJsonObject(customer,"direction", custdir);
    JsonHelper::AddStringToJsonObject(customer,"name","joe");
    JsonHelper::AddFloatToJsonObject(customer,"radius",4);
    Customer cust = Customer(customer);

    std::vector<float> pos2;
    pos2.push_back(5);
    pos2.push_back(5);
    pos2.push_back(5);
    JsonHelper::AddStringToJsonObject(package,"type","package");
    JsonHelper::AddStdFloatVectorToJsonObject(package,"position",drone2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(package,"direction",pos2);
    JsonHelper::AddStringToJsonObject(package,"name","package");
    JsonHelper::AddFloatToJsonObject(package,"radius",4);
    Package p = Package(package);
    EXPECT_FALSE(d1.holdingPackage()); // Drone is not holding package before the update
    std::vector<std::vector<float>> toPackage;
    std::vector<std::vector<float>> packageToCust;
    toPackage.push_back(drone2Dir);
    packageToCust.push_back(drone2Dir);
    packageToCust.push_back(custpos);
    d1.setToPackage(toPackage);
    ASSERT_EQ(toPackage,d1.getToPackage());
    d1.setPackageToCust(packageToCust);
    ASSERT_EQ(packageToCust,d1.getPackageToCust());
    toPackage.insert(toPackage.end(),packageToCust.begin(),packageToCust.end());
    p.setCustomer(&cust);
    d1.setPackage(&p);
    d1.setPath(toPackage);
    ASSERT_EQ(toPackage,d1.getPath());
    d1.update(1);

    EXPECT_TRUE(d1.holdingPackage());
    d1.update(.01); // Pop off duplicated node at package
    d1.update(16.764); // Enough time to move directly to the customer.

    // ASSERT FLAGS ARE BEING SET
    ASSERT_EQ(nullptr, d1.getPackage());
    ASSERT_EQ(false, d1.getMoving());
    ASSERT_EQ(0,p.GetPosition().at(0));
    ASSERT_EQ(-10000,p.GetPosition().at(1));
    ASSERT_EQ(0,p.GetPosition().at(2));
    EXPECT_FALSE(d1.IsDynamic()); // After deliver it isn't dynamic
    ASSERT_EQ(0,d1.getQueuedDeliveries().size());
    EXPECT_FALSE(d1.getRescheduleFlag());
    EXPECT_FALSE(d1.holdingPackage());
}

TEST_F(DroneTest, EnqueueDelivery) {
    EntityBase::resetEntityCount();
    JsonHelper::AddStringToJsonObject(drone8,"type","drone");
    std::vector<float> drone2Dir = {1,0,0};
    std::vector<float> pos = {0,0,0};
    JsonHelper::AddStdFloatVectorToJsonObject(drone8,"direction",drone2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(drone8,"position",pos);
    JsonHelper::AddStringToJsonObject(drone8,"name","Standard");
    JsonHelper::AddFloatToJsonObject(drone8,"radius",.5);
    JsonHelper::AddFloatToJsonObject(drone8,"speed",1);
    JsonHelper::AddFloatToJsonObject(drone8, "battery_capacity", 100);
    Drone d1(drone8); // Default picojson drone

    std::vector<float> custpos = {10, 10, 10};
    std::vector<float> custdir = {0,1,0};
    JsonHelper::AddStringToJsonObject(customer,"type","customer");
    JsonHelper::AddStdFloatVectorToJsonObject(customer,"position", custpos);
    JsonHelper::AddStdFloatVectorToJsonObject(customer,"direction", custdir);
    JsonHelper::AddStringToJsonObject(customer,"name","joe");
    JsonHelper::AddFloatToJsonObject(customer,"radius",4);
    Customer cust = Customer(customer);
    
    std::vector<float> pos2;
    pos2.push_back(5);
    pos2.push_back(5);
    pos2.push_back(5);
    JsonHelper::AddStringToJsonObject(package,"type","package");
    JsonHelper::AddStdFloatVectorToJsonObject(package,"position",drone2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(package,"direction",pos2);
    JsonHelper::AddStringToJsonObject(package,"name","package");
    JsonHelper::AddFloatToJsonObject(package,"radius",4);
    Package p = Package(package);

    d1.enqueueDelivery(&p,&cust);
    ASSERT_EQ(1,d1.getQueuedDeliveries().size());
    ASSERT_EQ(&p, d1.getQueuedDeliveries().front().at(0));
    ASSERT_EQ(&cust, d1.getQueuedDeliveries().front().at(1));
}

TEST_F(DroneTest, updateReschedule) {
    EntityBase::resetEntityCount();
    JsonHelper::AddStringToJsonObject(drone7,"type","drone");
    std::vector<float> drone2Dir = {1,0,0};
    std::vector<float> pos = {0,0,0};
    JsonHelper::AddStdFloatVectorToJsonObject(drone7,"direction",drone2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(drone7,"position",pos);
    JsonHelper::AddStringToJsonObject(drone7,"name","Standard");
    JsonHelper::AddFloatToJsonObject(drone7,"radius",.5);
    JsonHelper::AddFloatToJsonObject(drone7,"speed",1);
    JsonHelper::AddFloatToJsonObject(drone7, "battery_capacity", 100);
    Drone d1 = Drone(drone7);

    std::vector<float> custpos = {10, 10, 10};
    std::vector<float> custdir = {0,1,0};
    JsonHelper::AddStringToJsonObject(customer,"type","customer");
    JsonHelper::AddStdFloatVectorToJsonObject(customer,"position", custpos);
    JsonHelper::AddStdFloatVectorToJsonObject(customer,"direction", custdir);
    JsonHelper::AddStringToJsonObject(customer,"name","joe");
    JsonHelper::AddFloatToJsonObject(customer,"radius",4);
    Customer cust = Customer(customer);

    std::vector<float> pos2;
    pos2.push_back(5);
    pos2.push_back(5);
    pos2.push_back(5);
    JsonHelper::AddStringToJsonObject(package,"type","package");
    JsonHelper::AddStdFloatVectorToJsonObject(package,"position",drone2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(package,"direction",pos2);
    JsonHelper::AddStringToJsonObject(package,"name","package");
    JsonHelper::AddFloatToJsonObject(package,"radius",4);
    Package p = Package(package);
    EXPECT_FALSE(d1.holdingPackage()); // Drone is not holding package before the update
    std::vector<std::vector<float>> toPackage;
    std::vector<std::vector<float>> packageToCust;
    toPackage.push_back(drone2Dir);
    packageToCust.push_back(drone2Dir);
    packageToCust.push_back(custpos);
    d1.setToPackage(toPackage);
    ASSERT_EQ(toPackage,d1.getToPackage());
    d1.setPackageToCust(packageToCust);
    ASSERT_EQ(packageToCust,d1.getPackageToCust());
    toPackage.insert(toPackage.end(),packageToCust.begin(),packageToCust.end());
    p.setCustomer(&cust);
    d1.setPackage(&p);
    d1.setPath(toPackage);
    ASSERT_EQ(toPackage,d1.getPath());
    d1.update(1);

    EXPECT_TRUE(d1.holdingPackage());
    d1.updateReschedule(); // update reschedule while holding package
    ASSERT_EQ(nullptr,d1.getPackage());
    ASSERT_EQ(0,d1.getPackageToCust().size());
    ASSERT_EQ(0,d1.getToPackage().size());
    ASSERT_EQ(0,d1.getPath().size());
    ASSERT_FALSE(d1.holdingPackage());
    ASSERT_FALSE(d1.getMoving());
    ASSERT_FALSE(d1.getRescheduleFlag()); // Sets to false once method completes.
}

TEST_F(DroneTest, StartNewDelivery) {
    EntityBase::resetEntityCount();
    JsonHelper::AddStringToJsonObject(drone7,"type","drone");
    std::vector<float> drone2Dir = {1,0,0};
    std::vector<float> pos = {0,0,0};
    JsonHelper::AddStdFloatVectorToJsonObject(drone7,"direction",drone2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(drone7,"position",pos);
    JsonHelper::AddStringToJsonObject(drone7,"name","Standard");
    JsonHelper::AddFloatToJsonObject(drone7,"radius",.5);
    JsonHelper::AddFloatToJsonObject(drone7,"speed",1);
    JsonHelper::AddFloatToJsonObject(drone7, "battery_capacity", 100);
    JsonHelper::AddStringToJsonObject(drone7, "path", "beeline");
    Drone d1 = Drone(drone7);

    std::vector<float> custpos = {10, 10, 10};
    std::vector<float> custdir = {0,1,0};
    JsonHelper::AddStringToJsonObject(customer,"type","customer");
    JsonHelper::AddStdFloatVectorToJsonObject(customer,"position", custpos);
    JsonHelper::AddStdFloatVectorToJsonObject(customer,"direction", custdir);
    JsonHelper::AddStringToJsonObject(customer,"name","joe");
    JsonHelper::AddFloatToJsonObject(customer,"radius",4);
    Customer cust = Customer(customer);

    std::vector<float> pos2;
    pos2.push_back(5);
    pos2.push_back(5);
    pos2.push_back(5);
    JsonHelper::AddStringToJsonObject(package,"type","package");
    JsonHelper::AddStdFloatVectorToJsonObject(package,"position",drone2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(package,"direction",pos2);
    JsonHelper::AddStringToJsonObject(package,"name","package");
    JsonHelper::AddFloatToJsonObject(package,"radius",4);
    Package p = Package(package);
    d1.enqueueDelivery(&p, &cust);
    d1.startNewDelivery(nullptr);
    ASSERT_EQ(4,d1.getToPackage().size());
    ASSERT_EQ(4,d1.getPackageToCust().size());
    ASSERT_EQ(8,d1.getPath().size());
    ASSERT_EQ(0,d1.getQueuedDeliveries().size());
    EXPECT_TRUE(d1.getMoving());
    EXPECT_TRUE(d1.IsDynamic());
    ASSERT_EQ(&p,d1.getPackage());
}

TEST_F(DroneTest, SetColor) {
    EntityBase::resetEntityCount();
    JsonHelper::AddStringToJsonObject(drone7,"type","drone");
    std::vector<float> drone2Dir = {1,0,0};
    std::vector<float> pos = {0,0,0};
    JsonHelper::AddStdFloatVectorToJsonObject(drone7,"direction",drone2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(drone7,"position",pos);
    JsonHelper::AddStringToJsonObject(drone7,"name","Standard");
    JsonHelper::AddFloatToJsonObject(drone7,"radius",.5);
    JsonHelper::AddFloatToJsonObject(drone7,"speed",1);
    JsonHelper::AddFloatToJsonObject(drone7, "battery_capacity", 100);
    JsonHelper::AddStringToJsonObject(drone7, "path", "beeline");
    Drone d1 = Drone(drone7);

    ASSERT_TRUE(picojson::value(d1.GetDetails()).serialize().find("\"color\":\"0xFF00FF\"") == std::string::npos); // Check key is not contained in the serialization
    d1.setColor("0xFF00FF");
    ASSERT_FALSE(picojson::value(d1.GetDetails()).serialize().find("\"color\":\"0xFF00FF\"") == std::string::npos); // Check if key is contained in serialization

}

TEST_F(DroneTest, EntityBaseSetters) {
    EntityBase::resetEntityCount();
    JsonHelper::AddStringToJsonObject(drone7,"type","drone");
    std::vector<float> drone2Dir = {1,0,0};
    std::vector<float> pos = {0,0,0};
    JsonHelper::AddStdFloatVectorToJsonObject(drone7,"direction",drone2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(drone7,"position",pos);
    JsonHelper::AddStringToJsonObject(drone7,"name","Standard");
    JsonHelper::AddFloatToJsonObject(drone7,"radius",.5);
    JsonHelper::AddFloatToJsonObject(drone7,"speed",1);
    JsonHelper::AddFloatToJsonObject(drone7, "battery_capacity", 100);
    JsonHelper::AddStringToJsonObject(drone7, "path", "beeline");
    Drone d1 = Drone(drone7);
    std::vector<float> newPos{-1,-2,-3};
    std::vector<float> newDir{-1.5,-2.5,-3.5};
    d1.setPosition(newPos);
    d1.setDynamic(true);
    d1.setDirection(newDir);
    ASSERT_FLOAT_EQ(-1,d1.GetPosition().at(0));
    ASSERT_FLOAT_EQ(-2,d1.GetPosition().at(1));
    ASSERT_FLOAT_EQ(-3,d1.GetPosition().at(2));
    ASSERT_FLOAT_EQ(-1.5,d1.GetDirection().at(0));
    ASSERT_FLOAT_EQ(-2.5,d1.GetDirection().at(1));
    ASSERT_FLOAT_EQ(-3.5,d1.GetDirection().at(2));
    EXPECT_TRUE(d1.IsDynamic());
}

TEST_F(DroneTest, SettersDrone) {
    Drone drone_ = Drone(drone1);
    drone_.setDirection({1,2,3});
    drone_.setDynamic(true);
    drone_.setPosition({3,2,1});

    EXPECT_TRUE(drone_.IsDynamic());
    ASSERT_FLOAT_EQ(3, drone_.GetPosition().at(0));
    ASSERT_FLOAT_EQ(2, drone_.GetPosition().at(1));
    ASSERT_FLOAT_EQ(1, drone_.GetPosition().at(2));
    ASSERT_EQ(3,drone_.GetPosition().size());

    ASSERT_FLOAT_EQ(3, drone_.GetDirection().at(2));
    ASSERT_FLOAT_EQ(2, drone_.GetDirection().at(1));
    ASSERT_FLOAT_EQ(1, drone_.GetDirection().at(0));
    ASSERT_EQ(3,drone_.GetDirection().size());
}
}  // namespace csci3081
