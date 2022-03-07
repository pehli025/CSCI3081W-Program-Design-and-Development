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

class RobotTest : public ::testing::Test {
    protected:
        picojson::object robot1 = JsonHelper::CreateJsonObject();
        picojson::object robot2 = JsonHelper::CreateJsonObject();
        picojson::object robot3 = JsonHelper::CreateJsonObject();
        picojson::object robot4 = JsonHelper::CreateJsonObject();
        picojson::object robot5 = JsonHelper::CreateJsonObject();
        picojson::object robot6 = JsonHelper::CreateJsonObject();
        picojson::object robot7 = JsonHelper::CreateJsonObject();
        picojson::object robot8 = JsonHelper::CreateJsonObject();
        picojson::object robot9 = JsonHelper::CreateJsonObject();
        picojson::object robot10 = JsonHelper::CreateJsonObject();

        picojson::object package = JsonHelper::CreateJsonObject();
        picojson::object customer = JsonHelper::CreateJsonObject();
};  

/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(RobotTest, RobotConstructorDefaultPicoJson) {
    EntityBase::resetEntityCount();
    Robot r = Robot(robot1);
    EXPECT_EQ("Default name",r.GetName());
    ASSERT_FLOAT_EQ(0,r.GetPosition().at(0));
    ASSERT_FLOAT_EQ(0,r.GetPosition().at(1));
    ASSERT_FLOAT_EQ(0,r.GetPosition().at(2));
    EXPECT_EQ(3,r.GetPosition().size());

    ASSERT_FLOAT_EQ(0,r.GetDirection().at(0));
    ASSERT_FLOAT_EQ(0,r.GetDirection().at(1));
    ASSERT_FLOAT_EQ(0,r.GetDirection().at(2));
    EXPECT_EQ(3,r.GetDirection().size());

    ASSERT_FLOAT_EQ(1,r.GetRadius());
    ASSERT_FLOAT_EQ(0,r.GetId());

    ASSERT_EQ(false,r.holdingPackage());
    ASSERT_EQ(false,r.getMoving());
    ASSERT_EQ(false,r.IsDynamic());
    ASSERT_EQ(false,r.holdingPackage());
    ASSERT_EQ(false,r.getMoving());
    ASSERT_FLOAT_EQ(10,r.GetSpeed());

    ASSERT_EQ(0,r.GetVersion());
    // ASSERT_EQ(picojson::value(drone1).serialize(),picojson::value(d.GetDetails()).serialize());
}

TEST_F(RobotTest, RobotConstructorPicoJson) {
    EntityBase::resetEntityCount();
    Robot r1 = Robot(robot1);

    JsonHelper::AddStringToJsonObject(robot2,"type","robot");
    std::vector<float> robot2pos;
    robot2pos.push_back(10);
    robot2pos.push_back(15);
    robot2pos.push_back(20);
    JsonHelper::AddStdFloatVectorToJsonObject(robot2,"position",robot2pos);

    std::vector<float> robot2Dir;
    robot2Dir.push_back(2);
    robot2Dir.push_back(3);
    robot2Dir.push_back(4);
    JsonHelper::AddStdFloatVectorToJsonObject(robot2,"direction",robot2Dir);
    JsonHelper::AddStringToJsonObject(robot2,"name","charlie");
    JsonHelper::AddFloatToJsonObject(robot2,"radius",4);
    JsonHelper::AddFloatToJsonObject(robot2,"speed",100);

    Robot r2 = Robot(robot2);
    ASSERT_EQ("robot",r2.GetType());
    EXPECT_EQ("charlie",r2.GetName());
    ASSERT_FLOAT_EQ(4,r2.GetRadius());
    ASSERT_FLOAT_EQ(100,r2.GetSpeed());

    ASSERT_FLOAT_EQ(10,r2.GetPosition().at(0));
    ASSERT_FLOAT_EQ(15,r2.GetPosition().at(1));
    ASSERT_FLOAT_EQ(20,r2.GetPosition().at(2));
    EXPECT_EQ(3,r2.GetPosition().size());

    ASSERT_NEAR(.371390676,r2.GetDirection().at(0),.001);
    ASSERT_NEAR(.55706015,r2.GetDirection().at(1),.001);
    ASSERT_NEAR(.742781353,r2.GetDirection().at(2),.001);
    EXPECT_EQ(3,r2.GetDirection().size());
    EXPECT_EQ(0,r1.GetId());
    EXPECT_EQ(1,r2.GetId());
    EXPECT_EQ(0,r2.GetVersion());
    ASSERT_EQ(false,r2.holdingPackage());
    ASSERT_EQ(false,r2.getMoving());
    EXPECT_EQ(false,r2.IsDynamic());
    ASSERT_TRUE(nullptr == r2.getPackage());

    ASSERT_EQ(0,r1.GetVersion());
    ASSERT_EQ(0,r2.GetVersion());
    ASSERT_EQ(0,r2.getQueuedDeliveries().size());

    ASSERT_EQ(picojson::value(robot2).serialize(),picojson::value(r2.GetDetails()).serialize());
}

TEST_F(RobotTest, RobotMovement) {
    JsonHelper::AddStringToJsonObject(robot4,"type","robot");
    std::vector<float> robot2Dir;
    robot2Dir.push_back(1);
    robot2Dir.push_back(0);
    robot2Dir.push_back(0);
    JsonHelper::AddStdFloatVectorToJsonObject(robot4,"direction",robot2Dir);
    JsonHelper::AddStringToJsonObject(robot4,"name","Standard");
    JsonHelper::AddFloatToJsonObject(robot4,"radius",1);
    JsonHelper::AddFloatToJsonObject(robot4,"speed",1);
    Robot moving = Robot(robot4);

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

    // Testing movement in the y direction. Robot should not move.
    std::vector<float> y{0,1.5,0};
    moving.moveTo(y,2);
    ASSERT_NEAR(0,moving.GetPosition().at(0),.001);
    ASSERT_NEAR(0,moving.GetPosition().at(1),.001);
    ASSERT_NEAR(0,moving.GetPosition().at(2),.001);
    ASSERT_NEAR(0,moving.GetDirection().at(0),.001);
    ASSERT_NEAR(0,moving.GetDirection().at(1),.001);
    ASSERT_NEAR(0,moving.GetDirection().at(2),.001);
    
}
TEST_F(RobotTest, NoChangeInState) {
    EntityBase::resetEntityCount();
    JsonHelper::AddStringToJsonObject(robot7,"type","drone");
    JsonHelper::AddStringToJsonObject(robot7,"name","Standard");
    JsonHelper::AddFloatToJsonObject(robot7,"radius",1);
    JsonHelper::AddFloatToJsonObject(robot7,"speed",1);
    Robot rob = Robot(robot7);

    EXPECT_FALSE(rob.getMoving());
    EXPECT_FALSE(rob.holdingPackage());
    rob.setPackage(nullptr);
    EXPECT_FALSE(rob.getMoving());
    EXPECT_FALSE(rob.holdingPackage());
}
TEST_F(RobotTest, RobotRescheduleFlag) {
   EntityBase::resetEntityCount();

   JsonHelper::AddStringToJsonObject(robot5,"type","drone");
   std::vector<float> drone2Dir;
   drone2Dir.push_back(1);
   drone2Dir.push_back(0);
   drone2Dir.push_back(0);
   JsonHelper::AddStdFloatVectorToJsonObject(robot5,"direction",drone2Dir);
   JsonHelper::AddStringToJsonObject(robot5,"name","Standard");
   JsonHelper::AddFloatToJsonObject(robot5,"radius",1);
   JsonHelper::AddFloatToJsonObject(robot5,"speed",1);
   JsonHelper::AddFloatToJsonObject(robot5, "battery_capacity", 1);
   Robot r = Robot(robot5);

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
   r.setToPackage(toPackage);
   r.setPackageToCust(packageToCust);
   toPackage.insert(toPackage.end(),packageToCust.begin(),packageToCust.end());
   p.setCustomer(&cust);
   r.setPackage(&p);
   r.setPath(toPackage);

   ASSERT_FLOAT_EQ(1, r.getBattery().getRemLife()); 
   EXPECT_FALSE(r.getRescheduleFlag());
   r.update(1);
   r.update(1);
   EXPECT_TRUE(r.getRescheduleFlag());

   r.update(1); // Update to check if battery drains at idle
   ASSERT_FLOAT_EQ(0, r.getBattery().getRemLife()); 
}

TEST_F(RobotTest, RobotRadius) {
    EntityBase::resetEntityCount();
    JsonHelper::AddStringToJsonObject(robot6,"type","drone");
    std::vector<float> robot2Dir = {1,0,0};
    std::vector<float> pos = {0,0,0};
    JsonHelper::AddStdFloatVectorToJsonObject(robot6,"direction",robot2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(robot6,"position",pos);
    JsonHelper::AddStringToJsonObject(robot6,"name","Standard");
    JsonHelper::AddFloatToJsonObject(robot6,"radius",.5);
    JsonHelper::AddFloatToJsonObject(robot6,"speed",1);
    JsonHelper::AddFloatToJsonObject(robot6, "battery_capacity", 100);
    Robot r1 = Robot(robot6);

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
    JsonHelper::AddStdFloatVectorToJsonObject(package,"position",robot2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(package,"direction",pos2);
    JsonHelper::AddStringToJsonObject(package,"name","package");
    JsonHelper::AddFloatToJsonObject(package,"radius",4);
    Package p = Package(package);
    EXPECT_FALSE(r1.holdingPackage()); // Drone is not holding package before the update
    std::vector<std::vector<float>> toPackage;
    std::vector<std::vector<float>> packageToCust;
    toPackage.push_back(robot2Dir);
    packageToCust.push_back(robot2Dir);
    packageToCust.push_back(custpos);
    r1.setToPackage(toPackage);
    ASSERT_EQ(toPackage,r1.getToPackage());
    r1.setPackageToCust(packageToCust);
    ASSERT_EQ(packageToCust,r1.getPackageToCust());
    toPackage.insert(toPackage.end(),packageToCust.begin(),packageToCust.end());
    p.setCustomer(&cust);
    r1.setPackage(&p);
    r1.setPath(toPackage);
    ASSERT_EQ(toPackage,r1.getPath());
    r1.update(1);

    EXPECT_TRUE(r1.holdingPackage());
    r1.update(.01); // Pop off duplicated node at package
    r1.update(16.764); // Enough time to move directly to the package.

    // ASSERT FLAGS ARE BEING SET
    ASSERT_EQ(nullptr, r1.getPackage());
    ASSERT_EQ(false, r1.getMoving());
    ASSERT_EQ(0,p.GetPosition().at(0));
    ASSERT_EQ(-10000,p.GetPosition().at(1));
    ASSERT_EQ(0,p.GetPosition().at(2));
    EXPECT_FALSE(r1.IsDynamic());
    ASSERT_EQ(0,r1.getQueuedDeliveries().size());
    EXPECT_FALSE(r1.getRescheduleFlag());
    EXPECT_FALSE(r1.holdingPackage());
}

TEST_F(RobotTest, EnqueueDelivery) {
    EntityBase::resetEntityCount();
    JsonHelper::AddStringToJsonObject(robot8,"type","robot");
    std::vector<float> robot2Dir = {1,0,0};
    std::vector<float> pos = {0,0,0};
    JsonHelper::AddStdFloatVectorToJsonObject(robot8,"direction",robot2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(robot8,"position",pos);
    JsonHelper::AddStringToJsonObject(robot8,"name","Standard");
    JsonHelper::AddFloatToJsonObject(robot8,"radius",.5);
    JsonHelper::AddFloatToJsonObject(robot8,"speed",1);
    JsonHelper::AddFloatToJsonObject(robot8, "battery_capacity", 100);
    Robot r1(robot8); // Default picojson drone

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
    JsonHelper::AddStdFloatVectorToJsonObject(package,"position",robot2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(package,"direction",pos2);
    JsonHelper::AddStringToJsonObject(package,"name","package");
    JsonHelper::AddFloatToJsonObject(package,"radius",4);
    Package p = Package(package);

    r1.enqueueDelivery(&p,&cust);
    ASSERT_EQ(1,r1.getQueuedDeliveries().size());
    ASSERT_EQ(&p, r1.getQueuedDeliveries().front().at(0));
    ASSERT_EQ(&cust, r1.getQueuedDeliveries().front().at(1));
}

TEST_F(RobotTest, updateReschedule) {
    EntityBase::resetEntityCount();
    JsonHelper::AddStringToJsonObject(robot7,"type","drone");
    std::vector<float> robot2Dir = {1,0,0};
    std::vector<float> pos = {0,0,0};
    JsonHelper::AddStdFloatVectorToJsonObject(robot7,"direction",robot2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(robot7,"position",pos);
    JsonHelper::AddStringToJsonObject(robot7,"name","Standard");
    JsonHelper::AddFloatToJsonObject(robot7,"radius",.5);
    JsonHelper::AddFloatToJsonObject(robot7,"speed",1);
    JsonHelper::AddFloatToJsonObject(robot7, "battery_capacity", 100);
    Drone r1 = Drone(robot7);

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
    JsonHelper::AddStdFloatVectorToJsonObject(package,"position",robot2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(package,"direction",pos2);
    JsonHelper::AddStringToJsonObject(package,"name","package");
    JsonHelper::AddFloatToJsonObject(package,"radius",4);
    Package p = Package(package);
    EXPECT_FALSE(r1.holdingPackage()); // Drone is not holding package before the update
    std::vector<std::vector<float>> toPackage;
    std::vector<std::vector<float>> packageToCust;
    toPackage.push_back(robot2Dir);
    packageToCust.push_back(robot2Dir);
    packageToCust.push_back(custpos);
    r1.setToPackage(toPackage);
    ASSERT_EQ(toPackage,r1.getToPackage());
    r1.setPackageToCust(packageToCust);
    ASSERT_EQ(packageToCust,r1.getPackageToCust());
    toPackage.insert(toPackage.end(),packageToCust.begin(),packageToCust.end());
    p.setCustomer(&cust);
    r1.setPackage(&p);
    r1.setPath(toPackage);
    ASSERT_EQ(toPackage,r1.getPath());
    r1.update(1);

    EXPECT_TRUE(r1.holdingPackage());
    r1.updateReschedule(); // update reschedule while holding package
    ASSERT_EQ(nullptr,r1.getPackage());
    ASSERT_EQ(0,r1.getPackageToCust().size());
    ASSERT_EQ(0,r1.getToPackage().size());
    ASSERT_EQ(0,r1.getPath().size());
    ASSERT_FALSE(r1.holdingPackage());
    ASSERT_FALSE(r1.getMoving());
    ASSERT_FALSE(r1.getRescheduleFlag()); // Sets to false once method completes.
}

TEST_F(RobotTest, StartNewDelivery) {
    EntityBase::resetEntityCount();
    JsonHelper::AddStringToJsonObject(robot7,"type","robot");
    std::vector<float> robot2Dir = {1,0,0};
    std::vector<float> pos = {0,0,0};
    JsonHelper::AddStdFloatVectorToJsonObject(robot7,"direction",robot2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(robot7,"position",pos);
    JsonHelper::AddStringToJsonObject(robot7,"name","Standard");
    JsonHelper::AddFloatToJsonObject(robot7,"radius",.5);
    JsonHelper::AddFloatToJsonObject(robot7,"speed",1);
    JsonHelper::AddFloatToJsonObject(robot7, "battery_capacity", 100);
    JsonHelper::AddStringToJsonObject(robot7, "path", "beeline");
    Robot r1 = Robot(robot7);

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
    JsonHelper::AddStdFloatVectorToJsonObject(package,"position",robot2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(package,"direction",pos2);
    JsonHelper::AddStringToJsonObject(package,"name","package");
    JsonHelper::AddFloatToJsonObject(package,"radius",4);
    Package p = Package(package);
    r1.enqueueDelivery(&p, &cust);
    r1.startNewDelivery(nullptr);
    ASSERT_EQ(4,r1.getToPackage().size());
    ASSERT_EQ(4,r1.getPackageToCust().size());
    ASSERT_EQ(8,r1.getPath().size());
    ASSERT_EQ(0,r1.getQueuedDeliveries().size());
    EXPECT_TRUE(r1.getMoving());
    EXPECT_TRUE(r1.IsDynamic());
    ASSERT_EQ(&p,r1.getPackage());
}

TEST_F(RobotTest, SetColor) {
    EntityBase::resetEntityCount();
    JsonHelper::AddStringToJsonObject(robot7,"type","robot");
    std::vector<float> robot2Dir = {1,0,0};
    std::vector<float> pos = {0,0,0};
    JsonHelper::AddStdFloatVectorToJsonObject(robot7,"direction",robot2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(robot7,"position",pos);
    JsonHelper::AddStringToJsonObject(robot7,"name","Standard");
    JsonHelper::AddFloatToJsonObject(robot7,"radius",.5);
    JsonHelper::AddFloatToJsonObject(robot7,"speed",1);
    JsonHelper::AddFloatToJsonObject(robot7, "battery_capacity", 100);
    JsonHelper::AddStringToJsonObject(robot7, "path", "beeline");
    Robot r1 = Robot(robot7);

    ASSERT_TRUE(picojson::value(r1.GetDetails()).serialize().find("\"color\":\"0xFF00FF\"") == std::string::npos); // Check key is not contained in the serialization
    r1.setColor("0xFF00FF");
    ASSERT_FALSE(picojson::value(r1.GetDetails()).serialize().find("\"color\":\"0xFF00FF\"") == std::string::npos); // Check if key is contained in serialization
}

TEST_F(RobotTest, EntityBaseSetters) {
    EntityBase::resetEntityCount();
    JsonHelper::AddStringToJsonObject(robot7,"type","robot");
    std::vector<float> robot2Dir = {1,0,0};
    std::vector<float> pos = {0,0,0};
    JsonHelper::AddStdFloatVectorToJsonObject(robot7,"direction",robot2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(robot7,"position",pos);
    JsonHelper::AddStringToJsonObject(robot7,"name","Standard");
    JsonHelper::AddFloatToJsonObject(robot7,"radius",.5);
    JsonHelper::AddFloatToJsonObject(robot7,"speed",1);
    JsonHelper::AddFloatToJsonObject(robot7, "battery_capacity", 100);
    JsonHelper::AddStringToJsonObject(robot7, "path", "beeline");
    Robot r1 = Robot(robot7);
    std::vector<float> newPos{-1,-2,-3};
    std::vector<float> newDir{-1.5,-2.5,-3.5};
    r1.setPosition(newPos);
    r1.setDynamic(true);
    r1.setDirection(newDir);
    ASSERT_FLOAT_EQ(-1,r1.GetPosition().at(0));
    ASSERT_FLOAT_EQ(-2,r1.GetPosition().at(1));
    ASSERT_FLOAT_EQ(-3,r1.GetPosition().at(2));
    ASSERT_FLOAT_EQ(-1.5,r1.GetDirection().at(0));
    ASSERT_FLOAT_EQ(-2.5,r1.GetDirection().at(1));
    ASSERT_FLOAT_EQ(-3.5,r1.GetDirection().at(2));
    EXPECT_TRUE(r1.IsDynamic());
}

TEST_F(RobotTest, SettersRobot) {
    Robot robot_ = Robot(robot1);
    robot_.setDirection({1,2,3});
    robot_.setDynamic(true);
    robot_.setPosition({3,2,1});

    EXPECT_TRUE(robot_.IsDynamic());
    ASSERT_FLOAT_EQ(3, robot_.GetPosition().at(0));
    ASSERT_FLOAT_EQ(2, robot_.GetPosition().at(1));
    ASSERT_FLOAT_EQ(1, robot_.GetPosition().at(2));
    ASSERT_EQ(3,robot_.GetPosition().size());

    ASSERT_FLOAT_EQ(3, robot_.GetDirection().at(2));
    ASSERT_FLOAT_EQ(2, robot_.GetDirection().at(1));
    ASSERT_FLOAT_EQ(1, robot_.GetDirection().at(0));
    ASSERT_EQ(3,robot_.GetDirection().size());
}
}  // namespace csci3081
