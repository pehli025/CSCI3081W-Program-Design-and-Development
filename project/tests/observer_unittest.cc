#include "gtest/gtest.h"
#include "json_helper.h"
#include "drone.h"
#include "robot.h"
#include "package.h"
#include "dvec_decorator.h"
#include "dvec_color_dec.h"
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

class ObserverTest : public ::testing::Test {
    protected:
        void SetUp() {

            // Setup delivery vehicles
            drone = JsonHelper::CreateJsonObject();
            drone2 = JsonHelper::CreateJsonObject();
            robot = JsonHelper::CreateJsonObject();
            robot2 = JsonHelper::CreateJsonObject();
            package = JsonHelper::CreateJsonObject();
            customer = JsonHelper::CreateJsonObject();
            JsonHelper::AddStringToJsonObject(drone,"type","drone");
            JsonHelper::AddStringToJsonObject(robot,"type","robot");
            JsonHelper::AddStringToJsonObject(drone2,"type","drone");
            JsonHelper::AddStringToJsonObject(robot2,"type","robot");
            std::vector<float> pos;
            pos.push_back(0);
            pos.push_back(0);
            pos.push_back(0);
            std::vector<float> dir;
            dir.push_back(0);
            dir.push_back(0);
            dir.push_back(0);
            JsonHelper::AddStdFloatVectorToJsonObject(drone,"position",pos);
            JsonHelper::AddStdFloatVectorToJsonObject(robot,"position",pos);
            JsonHelper::AddStdFloatVectorToJsonObject(drone,"direction",dir);
            JsonHelper::AddStdFloatVectorToJsonObject(robot,"direction",dir);
            JsonHelper::AddStringToJsonObject(drone,"name","Drone");
            JsonHelper::AddStringToJsonObject(robot,"name","Robot");
            JsonHelper::AddFloatToJsonObject(drone,"radius",1);
            JsonHelper::AddFloatToJsonObject(robot,"radius",1);
            JsonHelper::AddFloatToJsonObject(drone,"speed",1);
            JsonHelper::AddFloatToJsonObject(robot,"speed",1);
            JsonHelper::AddStdFloatVectorToJsonObject(drone2,"position",pos);
            JsonHelper::AddStdFloatVectorToJsonObject(robot2,"position",pos);
            JsonHelper::AddStdFloatVectorToJsonObject(drone2,"direction",dir);
            JsonHelper::AddStdFloatVectorToJsonObject(robot2,"direction",dir);
            JsonHelper::AddStringToJsonObject(drone2,"name","Drone");
            JsonHelper::AddStringToJsonObject(robot2,"name","Robot");
            JsonHelper::AddFloatToJsonObject(drone2,"radius",1);
            JsonHelper::AddFloatToJsonObject(robot2,"radius",1);
            JsonHelper::AddFloatToJsonObject(drone2,"speed",1);
            JsonHelper::AddFloatToJsonObject(robot2,"speed",1);

            // Setup packages and customers
            std::vector<float> packPos{1,0,0};
            std::vector<float> custPos{0,0,1};
            JsonHelper::AddStringToJsonObject(customer,"type","customer");
            JsonHelper::AddStdFloatVectorToJsonObject(customer,"position",custPos);
            JsonHelper::AddStdFloatVectorToJsonObject(customer,"direction",custPos); // Just reuse vector
            JsonHelper::AddStringToJsonObject(customer,"name","customer");
            JsonHelper::AddFloatToJsonObject(customer,"radius",1);

            JsonHelper::AddStringToJsonObject(package,"type","package");
            JsonHelper::AddStdFloatVectorToJsonObject(package,"position",packPos);
            JsonHelper::AddStdFloatVectorToJsonObject(package,"direction",packPos);
            JsonHelper::AddStringToJsonObject(package,"name","package");
            JsonHelper::AddFloatToJsonObject(package,"radius",1);



        }
        picojson::object drone;
        picojson::object robot;
        picojson::object drone2;
        picojson::object robot2;
        picojson::object package;
        picojson::object customer;
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(ObserverTest, DetachAttachTest) {
    EntityBase::resetEntityCount();
 
    Drone d = Drone(drone);
    Robot r = Robot(robot);
    Package p = Package(package);
    Customer c = Customer(customer);
    entity_project::EntityConsoleLogger logger;
    d.attach(&logger);
    testing::internal::CaptureStdout();
    d.notifyIdle();
    std::cout << std::flush;
    std::string output = testing::internal::GetCapturedStdout();
    // Test that correct output to stdout with 1 observer on drone.
    ASSERT_EQ("Entity #0: {\"type\":\"notify\",\"value\":\"idle\"}\n", output);
    r.attach(&logger);
    r.attach(&logger);
    testing::internal::CaptureStdout();
    r.notifyIdle();
    std::cout << std::flush;
    output = testing::internal::GetCapturedStdout();
    // Test that two messages output to stdout with 2 observers on robot notifying idle.
    // Implicitly tests that the size of observers changes upon attach
    ASSERT_EQ("Entity #1: {\"type\":\"notify\",\"value\":\"idle\"}\nEntity #1: {\"type\":\"notify\",\"value\":\"idle\"}\n", output);

    d.detach(&logger);
    testing::internal::CaptureStdout();
    d.notifyIdle();
    std::cout << std::flush;
    output = testing::internal::GetCapturedStdout();
    // Assert an empty string in stdout after detaching everything from drone.
    ASSERT_EQ("",output);

    r.detach(&logger);
    testing::internal::CaptureStdout();
    r.notifyIdle();
    std::cout << std::flush;
    output = testing::internal::GetCapturedStdout();
    // Assert an empty string in stdout after detaching the duplicate observer
    ASSERT_EQ("",output);

    std::vector<std::vector<float>> path;
    std::vector<float> p1{0,0,0};
    std::vector<float> p2{1,0,0};
    std::vector<float> p3{1,0,1};
    std::vector<float> p4{0,0,1};
    std::vector<std::vector<float>> q;
    q.push_back(p1);
    path.push_back(p1);
    path.push_back(p2);
    path.push_back(p3);
    path.push_back(p4);
    r.setPackageToCust(path);
    r.setToPackage(q);
    r.attach(&logger);
    testing::internal::CaptureStdout();
    r.notifyToPackage();
    std::cout << std::flush;
    output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("Entity #1: {\"path\":[[0,0,0]],\"type\":\"notify\",\"value\":\"moving\"}\n",output);

    testing::internal::CaptureStdout();
    r.notifyToCustomer();
    std::cout << std::flush;
    output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("Entity #1: {\"path\":[[0,0,0],[1,0,0],[1,0,1],[0,0,1]],\"type\":\"notify\",\"value\":\"moving\"}\n",output);

    p.attach(&logger);
    testing::internal::CaptureStdout();
    p.notifyEnRoute();
    std::cout << std::flush;
    output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("Entity #2: {\"type\":\"notify\",\"value\":\"en route\"}\n",output);

    testing::internal::CaptureStdout();
    p.notifyScheduled();
    std::cout << std::flush;
    output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("Entity #2: {\"type\":\"notify\",\"value\":\"scheduled\"}\n",output);

    testing::internal::CaptureStdout();
    p.notifyDelivered();
    std::cout << std::flush;
    output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("Entity #2: {\"type\":\"notify\",\"value\":\"delivered\"}\n",output);
}
TEST_F(ObserverTest, DecoratorDetachAttachTest) {
    EntityBase::resetEntityCount();
    Drone drone_ = Drone(drone2);
    Robot robot_ = Robot(robot2);

    VehicleColorDecorator d = VehicleColorDecorator(&drone_);
    VehicleColorDecorator r = VehicleColorDecorator(&robot_);
    
    Package p = Package(package);
    Customer c = Customer(customer);
    entity_project::EntityConsoleLogger logger;
    d.attach(&logger);
    testing::internal::CaptureStdout();
    d.notifyIdle();
    std::cout << std::flush;
    std::string output = testing::internal::GetCapturedStdout();
    // Test that correct output to stdout with 1 observer on drone.
    ASSERT_EQ("Entity #0: {\"type\":\"notify\",\"value\":\"idle\"}\n", output);
    r.attach(&logger);
    r.attach(&logger);
    testing::internal::CaptureStdout();
    r.notifyIdle();
    std::cout << std::flush;
    output = testing::internal::GetCapturedStdout();
    // Test that two messages output to stdout with 2 observers on robot notifying idle.
    // Implicitly tests that the size of observers changes upon attach
    ASSERT_EQ("Entity #1: {\"type\":\"notify\",\"value\":\"idle\"}\nEntity #1: {\"type\":\"notify\",\"value\":\"idle\"}\n", output);

    d.detach(&logger);
    testing::internal::CaptureStdout();
    d.notifyIdle();
    std::cout << std::flush;
    output = testing::internal::GetCapturedStdout();
    // Assert an empty string in stdout after detaching everything from drone.
    ASSERT_EQ("",output);

    r.detach(&logger);
    testing::internal::CaptureStdout();
    r.notifyIdle();
    std::cout << std::flush;
    output = testing::internal::GetCapturedStdout();
    // Assert an empty string in stdout after detaching the duplicate observer
    ASSERT_EQ("",output);

    std::vector<std::vector<float>> path;
    std::vector<float> p1{0,0,0};
    std::vector<float> p2{1,0,0};
    std::vector<float> p3{1,0,1};
    std::vector<float> p4{0,0,1};
    std::vector<std::vector<float>> q;
    q.push_back(p1);
    path.push_back(p1);
    path.push_back(p2);
    path.push_back(p3);
    path.push_back(p4);
    r.setPackageToCust(path);
    r.setToPackage(q);
    r.attach(&logger);
    testing::internal::CaptureStdout();
    r.notifyToPackage();
    std::cout << std::flush;
    output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("Entity #1: {\"path\":[[0,0,0]],\"type\":\"notify\",\"value\":\"moving\"}\n",output);

    testing::internal::CaptureStdout();
    r.notifyToCustomer();
    std::cout << std::flush;
    output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("Entity #1: {\"path\":[[0,0,0],[1,0,0],[1,0,1],[0,0,1]],\"type\":\"notify\",\"value\":\"moving\"}\n",output);

    p.attach(&logger);
    testing::internal::CaptureStdout();
    p.notifyEnRoute();
    std::cout << std::flush;
    output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("Entity #2: {\"type\":\"notify\",\"value\":\"en route\"}\n",output);

    testing::internal::CaptureStdout();
    p.notifyScheduled();
    std::cout << std::flush;
    output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("Entity #2: {\"type\":\"notify\",\"value\":\"scheduled\"}\n",output);

    testing::internal::CaptureStdout();
    p.notifyDelivered();
    std::cout << std::flush;
    output = testing::internal::GetCapturedStdout();
    ASSERT_EQ("Entity #2: {\"type\":\"notify\",\"value\":\"delivered\"}\n",output);
}

}  // namespace csci3081