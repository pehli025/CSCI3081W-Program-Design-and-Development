#include "gtest/gtest.h"
#include "json_helper.h"
#include "customer.h"
#include "drone.h"
#include "robot.h"
#include "dvec_color_dec.h"
#include "dvec_decorator.h"
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

class DecoratorTest : public ::testing::Test{
    protected:
        picojson::object drone = JsonHelper::CreateJsonObject();
        picojson::object drone2 = JsonHelper::CreateJsonObject();
        picojson::object drone3 = JsonHelper::CreateJsonObject();
        picojson::object drone4 = JsonHelper::CreateJsonObject();
        picojson::object drone5 = JsonHelper::CreateJsonObject();
        picojson::object drone6 = JsonHelper::CreateJsonObject();

        picojson::object robot = JsonHelper::CreateJsonObject();
        picojson::object robot1 = JsonHelper::CreateJsonObject();
        picojson::object robot2 = JsonHelper::CreateJsonObject();
        picojson::object robot3 = JsonHelper::CreateJsonObject();
        picojson::object robot4 = JsonHelper::CreateJsonObject();
        picojson::object robot5 = JsonHelper::CreateJsonObject();

        picojson::object package = JsonHelper::CreateJsonObject();
        picojson::object customer = JsonHelper::CreateJsonObject();
        picojson::object package2 = JsonHelper::CreateJsonObject();
        picojson::object customer2 = JsonHelper::CreateJsonObject();
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(DecoratorTest, DroneTestColorUpdate) {
    EntityBase::resetEntityCount();
    
    //  Making new drone
    JsonHelper::AddStringToJsonObject(drone,"type","drone");
    std::vector<float> drone2Dir = {1,0,0};
    std::vector<float> pos = {0,0,0};
    JsonHelper::AddStdFloatVectorToJsonObject(drone,"direction",drone2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(drone,"position",pos);
    JsonHelper::AddStringToJsonObject(drone,"name","Standard");
    JsonHelper::AddFloatToJsonObject(drone,"radius",.5);
    JsonHelper::AddFloatToJsonObject(drone,"speed",1);
    JsonHelper::AddFloatToJsonObject(drone, "battery_capacity", 100);
    Drone drone_ = Drone(drone);

    // Making new package
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

    //  Making new customer
    std::vector<float> custpos = {10, 10, 10};
    std::vector<float> custdir = {0,1,0};
    JsonHelper::AddStringToJsonObject(customer,"type","customer");
    JsonHelper::AddStdFloatVectorToJsonObject(customer,"position", custpos);
    JsonHelper::AddStdFloatVectorToJsonObject(customer,"direction", custdir);
    JsonHelper::AddStringToJsonObject(customer,"name","joe");
    JsonHelper::AddFloatToJsonObject(customer,"radius",4);
    Customer cust = Customer(customer);

    VehicleColorDecorator v_ =  VehicleColorDecorator(&drone_);

    ASSERT_EQ("0x00ff00", v_.getBatteryColor()); //  testing max battery color
    
    std::vector<std::vector<float>> toPackage;
    std::vector<std::vector<float>> packageToCust;
    toPackage.push_back(drone2Dir);
    packageToCust.push_back(drone2Dir);
    packageToCust.push_back(custpos);
    v_.setToPackage(toPackage);
    v_.setPackageToCust(packageToCust);
    toPackage.insert(toPackage.end(),packageToCust.begin(),packageToCust.end());
    p.setCustomer(&cust);
    v_.setPackage(&p);
    v_.setPath(toPackage);

    v_.update(50);
    ASSERT_EQ(50, v_.getBattery().getRemLife()); //  50% remaining battery
    ASSERT_EQ("0x807f00", v_.getBatteryColor()); //  50% battery color test

    v_.update(25);
    ASSERT_EQ(25, v_.getBattery().getRemLife()); // 25% remaining battery
    ASSERT_EQ("0xc03f00", v_.getBatteryColor()); // 25% battery color test 

    v_.update(25);
    ASSERT_EQ(0, v_.getBattery().getRemLife()); // 0% remaining battery
    ASSERT_EQ("0xff0000", v_.getBatteryColor()); // 0% battery color test 

}

TEST_F(DecoratorTest, RobotTestColorUpdate) {
    EntityBase::resetEntityCount();
    
    JsonHelper::AddStringToJsonObject(robot,"type","drone");
    std::vector<float> drone2Dir = {1,0,0};
    std::vector<float> pos = {0,0,0};
    JsonHelper::AddStdFloatVectorToJsonObject(robot,"direction",drone2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(robot,"position",pos);
    JsonHelper::AddStringToJsonObject(robot,"name","Standard");
    JsonHelper::AddFloatToJsonObject(robot,"radius",.5);
    JsonHelper::AddFloatToJsonObject(robot,"speed",1);
    JsonHelper::AddFloatToJsonObject(robot, "battery_capacity", 100);
    Robot robo_ = Robot(robot);

    // Making new package
    std::vector<float> pos2;
    pos2.push_back(5);
    pos2.push_back(5);
    pos2.push_back(5);
    JsonHelper::AddStringToJsonObject(package2,"type","package");
    JsonHelper::AddStdFloatVectorToJsonObject(package2,"position",drone2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(package2,"direction",pos2);
    JsonHelper::AddStringToJsonObject(package2,"name","package");
    JsonHelper::AddFloatToJsonObject(package2,"radius",4);
    Package p = Package(package2);

    //  Making new customer
    std::vector<float> custpos = {10, 10, 10};
    std::vector<float> custdir = {0,1,0};
    JsonHelper::AddStringToJsonObject(customer2,"type","customer");
    JsonHelper::AddStdFloatVectorToJsonObject(customer2,"position", custpos);
    JsonHelper::AddStdFloatVectorToJsonObject(customer2,"direction", custdir);
    JsonHelper::AddStringToJsonObject(customer2,"name","joe");
    JsonHelper::AddFloatToJsonObject(customer2,"radius",4);
    Customer cust = Customer(customer2);

    VehicleColorDecorator v_ =  VehicleColorDecorator(&robo_);

    ASSERT_EQ("0x00ff00", v_.getBatteryColor()); //  testing max battery color
    
    std::vector<std::vector<float>> toPackage;
    std::vector<std::vector<float>> packageToCust;
    toPackage.push_back(drone2Dir);
    packageToCust.push_back(drone2Dir);
    packageToCust.push_back(custpos);
    v_.setToPackage(toPackage);
    v_.setPackageToCust(packageToCust);
    toPackage.insert(toPackage.end(),packageToCust.begin(),packageToCust.end());
    p.setCustomer(&cust);
    v_.setPackage(&p);
    v_.setPath(toPackage);

    v_.update(50);
    ASSERT_EQ(50, v_.getBattery().getRemLife()); //  50% remaining battery
    ASSERT_EQ("0x807f00", v_.getBatteryColor()); //  50% battery color test

    v_.update(25);
    ASSERT_EQ(25, v_.getBattery().getRemLife()); // 25% remaining battery
    ASSERT_EQ("0xc03f00", v_.getBatteryColor()); // 25% battery color test 

    v_.update(25);
    ASSERT_EQ(0, v_.getBattery().getRemLife()); // 0% remaining battery
    ASSERT_EQ("0xff0000", v_.getBatteryColor()); // 0% battery color test


}
TEST_F(DecoratorTest, DroneDecoratorConstructorPicoJson) {
    EntityBase::resetEntityCount();

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

    Drone drone_ = Drone(drone2);
    VehicleColorDecorator decoratedDrone2 =  VehicleColorDecorator(&drone_);
    
    ASSERT_EQ("drone",decoratedDrone2.GetType());
    EXPECT_EQ("charlie",decoratedDrone2.GetName());
    ASSERT_FLOAT_EQ(4,decoratedDrone2.GetRadius());
    ASSERT_FLOAT_EQ(100,decoratedDrone2.GetSpeed());

    ASSERT_FLOAT_EQ(10,decoratedDrone2.GetPosition().at(0));
    ASSERT_FLOAT_EQ(15,decoratedDrone2.GetPosition().at(1));
    ASSERT_FLOAT_EQ(20,decoratedDrone2.GetPosition().at(2));
    EXPECT_EQ(3,decoratedDrone2.GetPosition().size());

    ASSERT_NEAR(.371390676,decoratedDrone2.GetDirection().at(0),.001);
    ASSERT_NEAR(.55706015,decoratedDrone2.GetDirection().at(1),.001);
    ASSERT_NEAR(.742781353,decoratedDrone2.GetDirection().at(2),.001);
    EXPECT_EQ(3,decoratedDrone2.GetDirection().size());
    EXPECT_EQ(0,decoratedDrone2.GetId());
    EXPECT_EQ(0,decoratedDrone2.GetVersion());
    ASSERT_EQ(false,decoratedDrone2.holdingPackage());
    ASSERT_EQ(false,decoratedDrone2.getMoving());
    EXPECT_EQ(false,decoratedDrone2.IsDynamic());
    ASSERT_TRUE(nullptr == decoratedDrone2.getPackage());
    ASSERT_EQ(0,decoratedDrone2.GetVersion());
    ASSERT_EQ(0,decoratedDrone2.getQueuedDeliveries().size());

    ASSERT_EQ(picojson::value(drone2).serialize(),picojson::value(decoratedDrone2.GetDetails()).serialize());
}
TEST_F(DecoratorTest, DroneDecoratorRadius) {
    EntityBase::resetEntityCount();
    JsonHelper::AddStringToJsonObject(drone3,"type","drone");
    std::vector<float> drone2Dir = {1,0,0};
    std::vector<float> pos = {0,0,0};
    JsonHelper::AddStdFloatVectorToJsonObject(drone3,"direction",drone2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(drone3,"position",pos);
    JsonHelper::AddStringToJsonObject(drone3,"name","Standard");
    JsonHelper::AddFloatToJsonObject(drone3,"radius",.5);
    JsonHelper::AddFloatToJsonObject(drone3,"speed",1);
    JsonHelper::AddFloatToJsonObject(drone3, "battery_capacity", 100);
    Drone drone_ = Drone(drone3);

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

    VehicleColorDecorator decoratedDrone3 =  VehicleColorDecorator(&drone_);

    EXPECT_FALSE(decoratedDrone3.holdingPackage()); // Drone is not holding package before the update
    std::vector<std::vector<float>> toPackage;
    std::vector<std::vector<float>> packageToCust;
    toPackage.push_back(drone2Dir);
    packageToCust.push_back(drone2Dir);
    packageToCust.push_back(custpos);
    decoratedDrone3.setToPackage(toPackage);
    decoratedDrone3.setPackageToCust(packageToCust);
    toPackage.insert(toPackage.end(),packageToCust.begin(),packageToCust.end());
    p.setCustomer(&cust);
    decoratedDrone3.setPackage(&p);
    decoratedDrone3.setPath(toPackage);
    decoratedDrone3.update(1);

    EXPECT_TRUE(decoratedDrone3.holdingPackage());
    decoratedDrone3.update(.01); // Pop off duplicated node at package
    decoratedDrone3.update(16.764); // Enough time to move directly to the package.

    // ASSERT FLAGS ARE BEING SET
    ASSERT_EQ(nullptr, decoratedDrone3.getPackage());
    ASSERT_EQ(false, decoratedDrone3.getMoving());
    ASSERT_EQ(0,p.GetPosition().at(0));
    ASSERT_EQ(-10000,p.GetPosition().at(1));
    ASSERT_EQ(0,p.GetPosition().at(2));
}
TEST_F(DecoratorTest, DroneDecoratorSetters) {
    Drone drone_ = Drone(drone4);
    VehicleColorDecorator v_ =  VehicleColorDecorator(&drone_);
    v_.setDirection({1,2,3});
    v_.setDynamic(true);
    v_.setPosition({3,2,1});

    EXPECT_TRUE(v_.IsDynamic());
    ASSERT_FLOAT_EQ(3, v_.GetPosition().at(0));
    ASSERT_FLOAT_EQ(2, v_.GetPosition().at(1));
    ASSERT_FLOAT_EQ(1, v_.GetPosition().at(2));
    ASSERT_EQ(3,v_.GetPosition().size());

    ASSERT_FLOAT_EQ(3, v_.GetDirection().at(2));
    ASSERT_FLOAT_EQ(2, v_.GetDirection().at(1));
    ASSERT_FLOAT_EQ(1, v_.GetDirection().at(0));
    ASSERT_EQ(3,v_.GetDirection().size());
}
TEST_F(DecoratorTest, RobotDecoratorConstructorPicoJson) {
    EntityBase::resetEntityCount();

    JsonHelper::AddStringToJsonObject(robot1,"type","robot");
    std::vector<float> drone2Pos;
    drone2Pos.push_back(10);
    drone2Pos.push_back(15);
    drone2Pos.push_back(20);
    JsonHelper::AddStdFloatVectorToJsonObject(robot1,"position",drone2Pos);

    std::vector<float> drone2Dir;
    drone2Dir.push_back(2);
    drone2Dir.push_back(3);
    drone2Dir.push_back(4);
    JsonHelper::AddStdFloatVectorToJsonObject(robot1,"direction",drone2Dir);
    JsonHelper::AddStringToJsonObject(robot1,"name","Hank");
    JsonHelper::AddFloatToJsonObject(robot1,"radius",4);
    JsonHelper::AddFloatToJsonObject(robot1,"speed",100);

    Robot robot1_ = Robot(robot1);
    VehicleColorDecorator colorRob =  VehicleColorDecorator(&robot1_);
    
    ASSERT_EQ("robot",colorRob.GetType());
    EXPECT_EQ("Hank",colorRob.GetName());
    ASSERT_FLOAT_EQ(4,colorRob.GetRadius());
    ASSERT_FLOAT_EQ(100,colorRob.GetSpeed());

    ASSERT_FLOAT_EQ(10,colorRob.GetPosition().at(0));
    ASSERT_FLOAT_EQ(15,colorRob.GetPosition().at(1));
    ASSERT_FLOAT_EQ(20,colorRob.GetPosition().at(2));
    EXPECT_EQ(3,colorRob.GetPosition().size());

    ASSERT_NEAR(.371390676,colorRob.GetDirection().at(0),.001);
    ASSERT_NEAR(.55706015,colorRob.GetDirection().at(1),.001);
    ASSERT_NEAR(.742781353,colorRob.GetDirection().at(2),.001);
    EXPECT_EQ(3,colorRob.GetDirection().size());
    EXPECT_EQ(0,colorRob.GetId());
    EXPECT_EQ(0,colorRob.GetVersion());
    ASSERT_EQ(false,colorRob.holdingPackage());
    ASSERT_EQ(false,colorRob.getMoving());
    EXPECT_EQ(false,colorRob.IsDynamic());
    ASSERT_TRUE(nullptr == colorRob.getPackage());
    ASSERT_EQ(0,colorRob.GetVersion());
    ASSERT_EQ(0,colorRob.getQueuedDeliveries().size());

    ASSERT_EQ(picojson::value(robot1).serialize(),picojson::value(colorRob.GetDetails()).serialize());

}
TEST_F(DecoratorTest, RobotDecoratorRadius) {
    EntityBase::resetEntityCount();
    
    JsonHelper::AddStringToJsonObject(robot2,"type","robot");
    std::vector<float> drone2Dir = {1,0,0};
    std::vector<float> pos = {0,0,0};
    JsonHelper::AddStdFloatVectorToJsonObject(robot2,"direction",drone2Dir);
    JsonHelper::AddStdFloatVectorToJsonObject(robot2,"position",pos);
    JsonHelper::AddStringToJsonObject(robot2,"name","Standard");
    JsonHelper::AddFloatToJsonObject(robot2,"radius",.5);
    JsonHelper::AddFloatToJsonObject(robot2,"speed",1);
    JsonHelper::AddFloatToJsonObject(robot2, "battery_capacity", 100);
    Robot rob_ = Robot(robot2);
    std::vector<float> custpos = {10, 10, 10};
    std::vector<float> custdir = {0,1,0};
    JsonHelper::AddStringToJsonObject(customer,"type","customer");
    JsonHelper::AddStdFloatVectorToJsonObject(customer,"position", custpos);
    JsonHelper::AddStdFloatVectorToJsonObject(customer,"direction", custdir);
    JsonHelper::AddStringToJsonObject(customer,"name","Bob");
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

    VehicleColorDecorator colorRob2 =  VehicleColorDecorator(&rob_);

    EXPECT_FALSE(colorRob2.holdingPackage()); // Drone is not holding package before the update
    std::vector<std::vector<float>> toPackage;
    std::vector<std::vector<float>> packageToCust;
    toPackage.push_back(drone2Dir);
    packageToCust.push_back(drone2Dir);
    packageToCust.push_back(custpos);
    colorRob2.setToPackage(toPackage);
    colorRob2.setPackageToCust(packageToCust);
    toPackage.insert(toPackage.end(),packageToCust.begin(),packageToCust.end());
    p.setCustomer(&cust);
    colorRob2.setPackage(&p);
    colorRob2.setPath(toPackage);
    colorRob2.update(1);

    EXPECT_TRUE(colorRob2.holdingPackage());
    colorRob2.update(.01); // Pop off duplicated node at package
    colorRob2.update(16.764); // Enough time to move directly to the package.

    // ASSERT FLAGS ARE BEING SET
    ASSERT_EQ(nullptr, colorRob2.getPackage());
    ASSERT_EQ(false, colorRob2.getMoving());
    ASSERT_EQ(0,p.GetPosition().at(0));
    ASSERT_EQ(-10000,p.GetPosition().at(1));
    ASSERT_EQ(0,p.GetPosition().at(2));
}
TEST_F(DecoratorTest, RobotDecoratorSetters) {
    Robot rob3_ = Robot(robot3);
    VehicleColorDecorator v_ =  VehicleColorDecorator(&rob3_);
    v_.setDirection({1,2,3});
    v_.setDynamic(true);
    v_.setPosition({3,2,1});

    EXPECT_TRUE(v_.IsDynamic());
    ASSERT_FLOAT_EQ(3, v_.GetPosition().at(0));
    ASSERT_FLOAT_EQ(2, v_.GetPosition().at(1));
    ASSERT_FLOAT_EQ(1, v_.GetPosition().at(2));
    ASSERT_EQ(3,v_.GetPosition().size());

    ASSERT_FLOAT_EQ(3, v_.GetDirection().at(2));
    ASSERT_FLOAT_EQ(2, v_.GetDirection().at(1));
    ASSERT_FLOAT_EQ(1, v_.GetDirection().at(0));
    ASSERT_EQ(3,v_.GetDirection().size());
}
TEST_F(DecoratorTest, DroneDecoratorRescheduleFlag) {
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
   JsonHelper::AddFloatToJsonObject(drone5, "battery_capacity", 1);
   Drone d = Drone(drone5);

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

   VehicleColorDecorator decoratedDrone5 =  VehicleColorDecorator(&d);
   
   toPackage.push_back(drone2Dir);
   packageToCust.push_back(drone2Dir);
   packageToCust.push_back(custpos);
   decoratedDrone5.setToPackage(toPackage);
   decoratedDrone5.setPackageToCust(packageToCust);
   toPackage.insert(toPackage.end(),packageToCust.begin(),packageToCust.end());
   p.setCustomer(&cust);
   decoratedDrone5.setPackage(&p);
   decoratedDrone5.setPath(toPackage);

   ASSERT_FLOAT_EQ(1, decoratedDrone5.getBattery().getRemLife()); 
   EXPECT_FALSE(decoratedDrone5.getRescheduleFlag());
   decoratedDrone5.update(1);
   decoratedDrone5.update(1);
   EXPECT_TRUE(decoratedDrone5.getRescheduleFlag());

   decoratedDrone5.update(1); // Update to check if battery drains at idle
   ASSERT_FLOAT_EQ(0, decoratedDrone5.getBattery().getRemLife());
}
TEST_F(DecoratorTest, RobotDecoratedRescheduleFlag) {
   EntityBase::resetEntityCount();

   JsonHelper::AddStringToJsonObject(robot4,"type","robot");
   std::vector<float> robot4Dir;
   robot4Dir.push_back(1);
   robot4Dir.push_back(0);
   robot4Dir.push_back(0);
   JsonHelper::AddStdFloatVectorToJsonObject(robot4,"direction",robot4Dir);
   JsonHelper::AddStringToJsonObject(robot4,"name","Standard");
   JsonHelper::AddFloatToJsonObject(robot4,"radius",1);
   JsonHelper::AddFloatToJsonObject(robot4,"speed",1);
   JsonHelper::AddFloatToJsonObject(robot4, "battery_capacity", 1);
   Robot r = Robot(robot4);

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

   VehicleColorDecorator decoratedRobot4 =  VehicleColorDecorator(&r);
   
   toPackage.push_back(robot4Dir);
   packageToCust.push_back(robot4Dir);
   packageToCust.push_back(custpos);
   decoratedRobot4.setToPackage(toPackage);
   decoratedRobot4.setPackageToCust(packageToCust);
   toPackage.insert(toPackage.end(),packageToCust.begin(),packageToCust.end());
   p.setCustomer(&cust);
   decoratedRobot4.setPackage(&p);
   decoratedRobot4.setPath(toPackage);

   ASSERT_FLOAT_EQ(1, decoratedRobot4.getBattery().getRemLife()); 
   EXPECT_FALSE(decoratedRobot4.getRescheduleFlag());
   decoratedRobot4.update(1);
   decoratedRobot4.update(1);
   EXPECT_TRUE(decoratedRobot4.getRescheduleFlag());

   decoratedRobot4.update(1); // Update to check if battery drains at idle
   ASSERT_FLOAT_EQ(0, decoratedRobot4.getBattery().getRemLife());
}
TEST_F(DecoratorTest, DroneDecoratorMovement) {
    JsonHelper::AddStringToJsonObject(drone6,"type","drone");
    std::vector<float> drone2Dir;
    drone2Dir.push_back(1);
    drone2Dir.push_back(0);
    drone2Dir.push_back(0);
    JsonHelper::AddStdFloatVectorToJsonObject(drone6,"direction",drone2Dir);
    JsonHelper::AddStringToJsonObject(drone6,"name","Standard");
    JsonHelper::AddFloatToJsonObject(drone6,"radius",1);
    JsonHelper::AddFloatToJsonObject(drone6,"speed",1);
    Drone moving = Drone(drone6);

    VehicleColorDecorator decoratedMoving =  VehicleColorDecorator(&moving);

    // Overshoot our point, clamp to dest
    std::vector<float> dest{1.5,0,0};
    decoratedMoving.moveTo(dest,2);
    ASSERT_NEAR(1.5,decoratedMoving.GetPosition().at(0),.001);
    ASSERT_NEAR(0,decoratedMoving.GetPosition().at(1),.001);
    ASSERT_NEAR(0,decoratedMoving.GetPosition().at(2),.001);
    ASSERT_NEAR(1.0,decoratedMoving.GetDirection().at(0),.001);
    ASSERT_NEAR(0,decoratedMoving.GetDirection().at(1),.001);
    ASSERT_NEAR(0,decoratedMoving.GetDirection().at(2),.001);

    // Undershoot
    std::vector<float> origin{0,0,0};
    decoratedMoving.moveTo(origin,.5);
    ASSERT_NEAR(1.0,decoratedMoving.GetPosition().at(0),.001);
    ASSERT_NEAR(0,decoratedMoving.GetPosition().at(1),.001);
    ASSERT_NEAR(0,decoratedMoving.GetPosition().at(2),.001);

    // Test direction switch
    ASSERT_NEAR(-1.0,decoratedMoving.GetDirection().at(0),.001);
    ASSERT_NEAR(0,decoratedMoving.GetDirection().at(1),.001);
    ASSERT_NEAR(0,decoratedMoving.GetDirection().at(2),.001);

    // Exact
    decoratedMoving.moveTo(origin,1);
    ASSERT_NEAR(0,decoratedMoving.GetPosition().at(0),.001);
    ASSERT_NEAR(0,decoratedMoving.GetPosition().at(1),.001);
    ASSERT_NEAR(0,decoratedMoving.GetPosition().at(2),.001);
    
}
TEST_F(DecoratorTest, RobotDecoratorMovement) {
    JsonHelper::AddStringToJsonObject(robot5,"type","robot");
    std::vector<float> robot5Dir;
    robot5Dir.push_back(1);
    robot5Dir.push_back(0);
    robot5Dir.push_back(0);
    JsonHelper::AddStdFloatVectorToJsonObject(robot5,"direction",robot5Dir);
    JsonHelper::AddStringToJsonObject(robot5,"name","Standard");
    JsonHelper::AddFloatToJsonObject(robot5,"radius",1);
    JsonHelper::AddFloatToJsonObject(robot5,"speed",1);
    Robot moving = Robot(robot5);

    VehicleColorDecorator decoratedMoving =  VehicleColorDecorator(&moving);

    // Overshoot our point, clamp to dest
    std::vector<float> dest{1.5,0,0};
    decoratedMoving.moveTo(dest,2);
    ASSERT_NEAR(1.5,decoratedMoving.GetPosition().at(0),.001);
    ASSERT_NEAR(0,decoratedMoving.GetPosition().at(1),.001);
    ASSERT_NEAR(0,decoratedMoving.GetPosition().at(2),.001);
    ASSERT_NEAR(1.0,decoratedMoving.GetDirection().at(0),.001);
    ASSERT_NEAR(0,decoratedMoving.GetDirection().at(1),.001);
    ASSERT_NEAR(0,decoratedMoving.GetDirection().at(2),.001);

    // Undershoot
    std::vector<float> origin{0,0,0};
    decoratedMoving.moveTo(origin,.5);
    ASSERT_NEAR(1.0,decoratedMoving.GetPosition().at(0),.001);
    ASSERT_NEAR(0,decoratedMoving.GetPosition().at(1),.001);
    ASSERT_NEAR(0,decoratedMoving.GetPosition().at(2),.001);

    // Test direction switch
    ASSERT_NEAR(-1.0,decoratedMoving.GetDirection().at(0),.001);
    ASSERT_NEAR(0,decoratedMoving.GetDirection().at(1),.001);
    ASSERT_NEAR(0,decoratedMoving.GetDirection().at(2),.001);

    // Exact
    decoratedMoving.moveTo(origin,1);
    ASSERT_NEAR(0,decoratedMoving.GetPosition().at(0),.001);
    ASSERT_NEAR(0,decoratedMoving.GetPosition().at(1),.001);
    ASSERT_NEAR(0,decoratedMoving.GetPosition().at(2),.001);
    
}
} //  namespace csci3081