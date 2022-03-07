#include "gtest/gtest.h"
#include "json_helper.h"
#include "customer.h"
#include "drone.h"
#include "robot.h"
#include "package.h"
#include "sim_helper.h"
#include "../include/composite_factory.h"
#include "../include/drone_factory.h"
#include "../include/customer_factory.h"
#include "../include/package_factory.h"
#include "../include/robot_factory.h"
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
class SimHelper : public ::testing::Test {
    protected:
        void SetUp() {
            dFac = new DroneFactory();
            cFac = new CustomerFactory();
            pFac = new PackageFactory();
            rFac = new RobotFactory();
            compFac = new CompositeFactory();
            obj1 = JsonHelper::CreateJsonObject();
            obj2 = JsonHelper::CreateJsonObject();
            obj3 = JsonHelper::CreateJsonObject();
            obj4 = JsonHelper::CreateJsonObject();
            JsonHelper::AddStringToJsonObject(obj1,"type","drone");
            std::vector<float> dronePos;
            dronePos.push_back(0);
            dronePos.push_back(0);
            dronePos.push_back(0);
            std::vector<float> droneDir;
            droneDir.push_back(-1);
            droneDir.push_back(-1);
            droneDir.push_back(-1);
            JsonHelper::AddStdFloatVectorToJsonObject(obj1,"position",dronePos);
            JsonHelper::AddStdFloatVectorToJsonObject(obj1,"direction",droneDir);
            JsonHelper::AddStringToJsonObject(obj1,"name","charlie");
            JsonHelper::AddFloatToJsonObject(obj1,"radius",1);
            JsonHelper::AddFloatToJsonObject(obj1,"speed",1);
            JsonHelper::AddStringToJsonObject(obj1,"path","beeline");

            JsonHelper::AddStringToJsonObject(obj4,"type","robot");
            JsonHelper::AddFloatToJsonObject(obj4,"battery_capacity",60);
            std::vector<float> robotPos;
            robotPos.push_back(0);
            robotPos.push_back(0);
            robotPos.push_back(0);
            std::vector<float> robotDir;
            robotDir.push_back(-1);
            robotDir.push_back(-1);
            robotDir.push_back(-1);
            JsonHelper::AddStdFloatVectorToJsonObject(obj4,"position",robotPos);
            JsonHelper::AddStdFloatVectorToJsonObject(obj4,"direction",robotDir);
            JsonHelper::AddStringToJsonObject(obj4,"name","delta");
            JsonHelper::AddFloatToJsonObject(obj4,"radius",1);
            JsonHelper::AddFloatToJsonObject(obj4,"speed",1);
            JsonHelper::AddStringToJsonObject(obj4,"path","beeline");

            std::vector<float> custPos; // (10, 0, 10)
            custPos.push_back(10);
            custPos.push_back(0);
            custPos.push_back(10);
            JsonHelper::AddStringToJsonObject(obj2,"type","customer");
            JsonHelper::AddStdFloatVectorToJsonObject(obj2,"position",custPos);
            JsonHelper::AddStdFloatVectorToJsonObject(obj2,"direction",custPos);
            JsonHelper::AddStringToJsonObject(obj2,"name","beta");
            JsonHelper::AddFloatToJsonObject(obj2,"radius",1);

            std::vector<float> packagePos; // (5,0,0)
            packagePos.push_back(10);
            packagePos.push_back(0);
            packagePos.push_back(0);
            std::vector<float> packageDir;
            packageDir.push_back(1.5);
            packageDir.push_back(1.5);
            packageDir.push_back(1.5);
            JsonHelper::AddStringToJsonObject(obj3,"type","package");
            JsonHelper::AddStdFloatVectorToJsonObject(obj3,"position",packagePos);
            JsonHelper::AddStdFloatVectorToJsonObject(obj3,"direction",packageDir);
            JsonHelper::AddStringToJsonObject(obj3,"name","charlie");
            JsonHelper::AddFloatToJsonObject(obj3,"radius",1);
            compFac->AddFactory(dFac);
            compFac->AddFactory(cFac);
            compFac->AddFactory(pFac);
            compFac->AddFactory(rFac);
        }
        void TearDown() {
            delete compFac;
        }
        DroneFactory* dFac;
        CustomerFactory* cFac;
        PackageFactory* pFac;
        RobotFactory* rFac;
        CompositeFactory* compFac;
        picojson::object obj1;
        picojson::object obj2;
        picojson::object obj3;
        picojson::object obj4;
        picojson::object obj5;
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/
TEST_F(SimHelper, ScheduleTest) {
    std::vector<IEntity*> entities;
    entities.push_back(compFac->CreateEntity(obj1)); // Drone
    entities.push_back(compFac->CreateEntity(obj2)); // Customer
    entities.push_back(compFac->CreateEntity(obj3)); // Package
    entities.push_back(compFac->CreateEntity(obj4)); // Robot
    schedule(entities.at(2), entities.at(1), entities);
    Drone* d = static_cast<Drone*>(entities.at(0));
    Robot* r = static_cast<Robot*>(entities.at(3));
    ASSERT_EQ(1, d->getQueuedDeliveries().size()); // Drone has 1 delivery
    ASSERT_EQ(0, r->getQueuedDeliveries().size()); // Robot has 0 delivery
    schedule(entities.at(2), entities.at(1), entities);
    ASSERT_EQ(1, d->getQueuedDeliveries().size()); // Drone has 1
    ASSERT_EQ(1, r->getQueuedDeliveries().size()); // Robot has 1
    schedule(entities.at(2), entities.at(1), entities);
    ASSERT_EQ(2, d->getQueuedDeliveries().size()); // Drone has 2
    ASSERT_EQ(1, r->getQueuedDeliveries().size()); // Robot has 1
}

TEST_F(SimHelper, UpdateTest) {
    std::vector<IEntity*> entities;
    entities.push_back(compFac->CreateEntity(obj1)); // Drone
    entities.push_back(compFac->CreateEntity(obj2)); // Customer
    entities.push_back(compFac->CreateEntity(obj3)); // Package
    entities.push_back(compFac->CreateEntity(obj3)); // Package again
    entities.push_back(compFac->CreateEntity(obj4)); // Robot
    schedule(entities.at(2),entities.at(1), entities);
    schedule(entities.at(3),entities.at(1), entities);
    Drone* d = static_cast<Drone*>(entities.at(0));
    Robot* r = static_cast<Robot*>(entities.at(4));
    updateEntities(.01, entities, nullptr);

    // Assert that the delivery vehicles stayed in the same place after update since the updateEntities method will have just called their startNewDelivery method.
    ASSERT_FLOAT_EQ(0, d->GetPosition().at(0));
    ASSERT_FLOAT_EQ(0, d->GetPosition().at(1));
    ASSERT_FLOAT_EQ(0, d->GetPosition().at(2));
    ASSERT_FLOAT_EQ(0, r->GetPosition().at(0));
    ASSERT_FLOAT_EQ(0, r->GetPosition().at(1));
    ASSERT_FLOAT_EQ(0, r->GetPosition().at(2));
    // Path sizes will now be initialized to 4 since they started a delivery
    ASSERT_EQ(4, d->getPackageToCust().size());
    ASSERT_EQ(4, r->getPackageToCust().size());
    ASSERT_EQ(4, d->getToPackage().size());
    ASSERT_EQ(4, r->getToPackage().size());
    // Delivery queue is popped
    ASSERT_EQ(0, d->getQueuedDeliveries().size());
    ASSERT_EQ(0, r->getQueuedDeliveries().size());
    updateEntities(.01,entities,nullptr);
    // Assert that they have moved to their origin position 
    ASSERT_FLOAT_EQ(0, d->GetPosition().at(0));
    ASSERT_FLOAT_EQ(0, d->GetPosition().at(1));
    ASSERT_FLOAT_EQ(0, d->GetPosition().at(2));
    ASSERT_FLOAT_EQ(0, r->GetPosition().at(0));
    ASSERT_FLOAT_EQ(0, r->GetPosition().at(1));
    ASSERT_FLOAT_EQ(0, r->GetPosition().at(2));
    // Update by a large time change to deplete battery of one entity
    updateEntities(60,entities,nullptr);
    // Assert that the entities have moved vertically since they are on beeline strategy
    ASSERT_NEAR(0,d->GetPosition().at(0),.0001);
    ASSERT_NEAR(60,d->GetPosition().at(1),.0001);
    ASSERT_NEAR(0,d->GetPosition().at(2),.0001);

    ASSERT_NEAR(0,r->GetPosition().at(0),.0001);
    ASSERT_NEAR(0,r->GetPosition().at(1),.0001); // Robot has no change in verticality. Robot beeline path is weird.
    ASSERT_NEAR(0,r->GetPosition().at(2),.0001);
    updateEntities(.01, entities, nullptr);
    EXPECT_TRUE(r->getRescheduleFlag()); // Reschedule the robot since battery is depleted
    EXPECT_FALSE(d->getRescheduleFlag()); // Don't reschedule the drone since battery is healthy
    EXPECT_TRUE(r->getPackage() != nullptr); // Both entities package property still not null
    EXPECT_TRUE(d->getPackage() != nullptr);
    updateEntities(.01,entities, nullptr);
    EXPECT_TRUE(r->getPackage() == nullptr); // Robot switches to null next frame
    EXPECT_TRUE(d->getPackage() != nullptr);
    ASSERT_EQ(1, d->getQueuedDeliveries().size()); // Drone gets another queued delivery upon battery death.
    updateEntities(401, entities, nullptr); // Have drone go max vertical, robot inert
    ASSERT_NEAR(0,d->GetPosition().at(0),.0001);
    ASSERT_NEAR(400,d->GetPosition().at(1),.0001);
    ASSERT_NEAR(0,d->GetPosition().at(2),.0001);

    ASSERT_NEAR(0,r->GetPosition().at(0),.0001);
    ASSERT_NEAR(0,r->GetPosition().at(1),.0001);
    ASSERT_NEAR(0,r->GetPosition().at(2),.0001);
    updateEntities(401, entities, nullptr); // Have drone go above package, robot inert
    ASSERT_NEAR(10,d->GetPosition().at(0),.0001);
    ASSERT_NEAR(400,d->GetPosition().at(1),.0001);
    ASSERT_NEAR(0,d->GetPosition().at(2),.0001);

    ASSERT_NEAR(0,r->GetPosition().at(0),.0001);
    ASSERT_NEAR(0,r->GetPosition().at(1),.0001);
    ASSERT_NEAR(0,r->GetPosition().at(2),.0001);

    updateEntities(401, entities, nullptr); // Have drone go to package, robot inert
    ASSERT_NEAR(10,d->GetPosition().at(0),.0001);
    ASSERT_NEAR(0,d->GetPosition().at(1),.0001);
    ASSERT_NEAR(0,d->GetPosition().at(2),.0001);

    ASSERT_NEAR(0,r->GetPosition().at(0),.0001);
    ASSERT_NEAR(0,r->GetPosition().at(1),.0001);
    ASSERT_NEAR(0,r->GetPosition().at(2),.0001);

    updateEntities(.01, entities, nullptr); // Pop off the duplicate node in the path at the package location
    ASSERT_NEAR(10,d->GetPosition().at(0),.0001);
    ASSERT_NEAR(0,d->GetPosition().at(1),.0001);
    ASSERT_NEAR(0,d->GetPosition().at(2),.0001);

    ASSERT_NEAR(0,r->GetPosition().at(0),.0001);
    ASSERT_NEAR(0,r->GetPosition().at(1),.0001);
    ASSERT_NEAR(0,r->GetPosition().at(2),.0001);

    updateEntities(401, entities, nullptr); // Have drone go above package, robot inert
    ASSERT_NEAR(10,d->GetPosition().at(0),.0001);
    ASSERT_NEAR(400,d->GetPosition().at(1),.0001);
    ASSERT_NEAR(0,d->GetPosition().at(2),.0001);

    ASSERT_NEAR(0,r->GetPosition().at(0),.0001);
    ASSERT_NEAR(0,r->GetPosition().at(1),.0001);
    ASSERT_NEAR(0,r->GetPosition().at(2),.0001);

    updateEntities(401, entities, nullptr); // Have drone go above customer, robot inert
    ASSERT_NEAR(10,d->GetPosition().at(0),.0001);
    ASSERT_NEAR(400,d->GetPosition().at(1),.0001);
    ASSERT_NEAR(10,d->GetPosition().at(2),.0001);

    ASSERT_NEAR(0,r->GetPosition().at(0),.0001);
    ASSERT_NEAR(0,r->GetPosition().at(1),.0001);
    ASSERT_NEAR(0,r->GetPosition().at(2),.0001);

    updateEntities(401, entities, nullptr); // Have drone deliver to customer, robot inert, package teleport
    ASSERT_NEAR(10,d->GetPosition().at(0),.0001);
    ASSERT_NEAR(0,d->GetPosition().at(1),.0001);
    ASSERT_NEAR(10,d->GetPosition().at(2),.0001);

    ASSERT_NEAR(0,r->GetPosition().at(0),.0001);
    ASSERT_NEAR(0,r->GetPosition().at(1),.0001);
    ASSERT_NEAR(0,r->GetPosition().at(2),.0001);
    Package* p = static_cast<Package*>(entities.at(2));
    // Teleport
    ASSERT_NEAR(0,p->GetPosition().at(0),.0001);
    ASSERT_NEAR(-10000,p->GetPosition().at(1),.0001);
    ASSERT_NEAR(0,p->GetPosition().at(2),.0001);
}


}  // namespace csci3081
