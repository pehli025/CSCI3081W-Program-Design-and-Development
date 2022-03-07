#include "gtest/gtest.h"
#include <EntityProject/project_settings.h>
#include "../include/delivery_simulation.h"
#include <EntityProject/entity.h>
#include "json_helper.h"
#include "../include/composite_factory.h"
#include "../include/drone_factory.h"
#include "../include/customer_factory.h"
#include "../include/package_factory.h"
#include "../include/robot_factory.h"
#include "drone.h"
#include "customer.h"
#include "package.h"
#include "robot.h"


#include <iostream>

namespace csci3081 {

using entity_project::IEntity;

class FactoryTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    system = dynamic_cast<IDeliverySystem*>(GetEntitySystem("default"));
  }
  virtual void TearDown() {}

  IDeliverySystem* system;
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(FactoryTest, DroneCreated) {
  picojson::object obj = JsonHelper::CreateJsonObject();
  JsonHelper::AddStringToJsonObject(obj, "type", "drone");
  std::vector<float> position_to_add;
  position_to_add.push_back(498.292);
  position_to_add.push_back(253.883);
  position_to_add.push_back(-228.623);
  JsonHelper::AddStdFloatVectorToJsonObject(obj, "position", position_to_add);
  std::vector<float> direction_to_add;
  direction_to_add.push_back(1);
  direction_to_add.push_back(0);
  direction_to_add.push_back(0);
  JsonHelper::AddStdFloatVectorToJsonObject(obj, "direction", direction_to_add);
  IEntity* entity = system->CreateEntity(obj);

  // Checks that the returned entity is not NULL
  ASSERT_NE(entity, nullptr) << "The entity created";
  system->AddEntity(entity);
  ASSERT_EQ(1, system->GetEntities().size());
  ASSERT_FLOAT_EQ(entity->GetPosition()[0], position_to_add[0]);
  ASSERT_FLOAT_EQ(entity->GetPosition()[1], position_to_add[1]);
  ASSERT_FLOAT_EQ(entity->GetPosition()[2], position_to_add[2]);
  ASSERT_FLOAT_EQ(entity->GetDirection()[0], direction_to_add[0]);
  ASSERT_FLOAT_EQ(entity->GetDirection()[1], direction_to_add[1]);
  ASSERT_FLOAT_EQ(entity->GetDirection()[2], direction_to_add[2]);

  // Checks that when GetDetails() is called, the entity returns 
  //  the picojson object that was used to initialize it
  ASSERT_EQ(picojson::value(system->GetEntities()[0]->GetDetails()).serialize(), picojson::value(obj).serialize());

}

class AllFactoryUnitTests : public ::testing::Test {
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
            dronePos.push_back(1);
            dronePos.push_back(1);
            dronePos.push_back(1);
            std::vector<float> droneDir;
            droneDir.push_back(-1);
            droneDir.push_back(-1);
            droneDir.push_back(-1);
            JsonHelper::AddStdFloatVectorToJsonObject(obj1,"position",dronePos);
            JsonHelper::AddStdFloatVectorToJsonObject(obj1,"direction",droneDir);
            JsonHelper::AddStringToJsonObject(obj1,"name","charlie");
            JsonHelper::AddFloatToJsonObject(obj1,"radius",1);
            JsonHelper::AddFloatToJsonObject(obj1,"speed",1);

            JsonHelper::AddStringToJsonObject(obj4,"type","robot");
            JsonHelper::AddFloatToJsonObject(obj4,"battery_capacity",60);
            std::vector<float> robotPos;
            robotPos.push_back(1);
            robotPos.push_back(1);
            robotPos.push_back(1);
            std::vector<float> robotDir;
            robotDir.push_back(-1);
            robotDir.push_back(-1);
            robotDir.push_back(-1);
            JsonHelper::AddStdFloatVectorToJsonObject(obj4,"position",robotPos);
            JsonHelper::AddStdFloatVectorToJsonObject(obj4,"direction",robotDir);
            JsonHelper::AddStringToJsonObject(obj4,"name","delta");
            JsonHelper::AddFloatToJsonObject(obj4,"radius",1);
            JsonHelper::AddFloatToJsonObject(obj4,"speed",1);

            std::vector<float> custPos;
            custPos.push_back(10);
            custPos.push_back(10);
            custPos.push_back(10);
            JsonHelper::AddStringToJsonObject(obj2,"type","customer");
            JsonHelper::AddStdFloatVectorToJsonObject(obj2,"position",custPos);
            JsonHelper::AddStdFloatVectorToJsonObject(obj2,"direction",custPos);
            JsonHelper::AddStringToJsonObject(obj2,"name","beta");
            JsonHelper::AddFloatToJsonObject(obj2,"radius",1);

            std::vector<float> packagePos;
            packagePos.push_back(5);
            packagePos.push_back(5);
            packagePos.push_back(5);
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
};

TEST_F(AllFactoryUnitTests, DroneFactory) {
    IEntity* newDrone = dFac->CreateEntity(obj1);
    EXPECT_TRUE(newDrone != nullptr);
    Drone* d = static_cast<Drone*>(newDrone);
    EXPECT_EQ("charlie",d->GetName());
    ASSERT_FLOAT_EQ(1,d->GetRadius());
    ASSERT_FLOAT_EQ(1,d->GetSpeed());

    ASSERT_FLOAT_EQ(1,d->GetPosition().at(0));
    ASSERT_FLOAT_EQ(1,d->GetPosition().at(1));
    ASSERT_FLOAT_EQ(1,d->GetPosition().at(2));
    EXPECT_EQ(3,d->GetPosition().size());

    ASSERT_NEAR(-0.57735026919,d->GetDirection().at(0),.001);
    ASSERT_NEAR(-0.57735026919,d->GetDirection().at(1),.001);
    ASSERT_NEAR(-0.57735026919,d->GetDirection().at(2),.001);
    EXPECT_EQ(3,d->GetDirection().size());
    EXPECT_EQ(0,d->GetVersion());
    ASSERT_EQ(false,d->holdingPackage());
    ASSERT_EQ(false,d->getMoving());
    EXPECT_EQ(false,d->IsDynamic());

    ASSERT_FLOAT_EQ(10000,d->getBattery().getMaxLife());
    ASSERT_FLOAT_EQ(10000,d->getBattery().getRemLife());

    ASSERT_EQ(picojson::value(obj1).serialize(),picojson::value(d->GetDetails()).serialize());
}

TEST_F(AllFactoryUnitTests, RobotFactory) {
    IEntity* newRobot = rFac->CreateEntity(obj4);
    EXPECT_TRUE(newRobot != nullptr);
    Robot* r = static_cast<Robot*>(newRobot);
    EXPECT_EQ("delta",r->GetName());
    ASSERT_FLOAT_EQ(1,r->GetRadius());
    ASSERT_FLOAT_EQ(1,r->GetSpeed());

    ASSERT_FLOAT_EQ(1,r->GetPosition().at(0));
    ASSERT_FLOAT_EQ(1,r->GetPosition().at(1));
    ASSERT_FLOAT_EQ(1,r->GetPosition().at(2));
    EXPECT_EQ(3,r->GetPosition().size());

    ASSERT_NEAR(-0.57735026919,r->GetDirection().at(0),.001);
    ASSERT_NEAR(-0.57735026919,r->GetDirection().at(1),.001);
    ASSERT_NEAR(-0.57735026919,r->GetDirection().at(2),.001);
    EXPECT_EQ(3,r->GetDirection().size());
    EXPECT_EQ(0,r->GetVersion());
    ASSERT_EQ(false,r->holdingPackage());
    ASSERT_EQ(false,r->getMoving());
    EXPECT_EQ(false,r->IsDynamic());

    ASSERT_FLOAT_EQ(60,r->getBattery().getMaxLife());
    ASSERT_FLOAT_EQ(60,r->getBattery().getRemLife());

    ASSERT_EQ(picojson::value(obj4).serialize(),picojson::value(r->GetDetails()).serialize());
}

TEST_F(AllFactoryUnitTests, CustomerFactory) {
    IEntity* cust = cFac->CreateEntity(obj2);
    Customer* c2 = static_cast<Customer*>(cust);
    ASSERT_FLOAT_EQ(10,c2->GetPosition().at(0));
    ASSERT_FLOAT_EQ(10,c2->GetPosition().at(1));
    ASSERT_FLOAT_EQ(10,c2->GetPosition().at(2));

    ASSERT_NEAR(0.57735026919,c2->GetDirection().at(0),.001);
    ASSERT_NEAR(0.57735026919,c2->GetDirection().at(1),.001);
    ASSERT_NEAR(0.57735026919,c2->GetDirection().at(2),.001);

    ASSERT_EQ(3,c2->GetPosition().size());
    ASSERT_EQ(3,c2->GetDirection().size());

    ASSERT_EQ("beta",c2->GetName());
    ASSERT_FLOAT_EQ(1,c2->GetRadius());
    ASSERT_EQ(0,c2->GetVersion());
    ASSERT_EQ(false,c2->IsDynamic());
    c2->setDynamic(true);
    ASSERT_EQ(true,c2->IsDynamic());

    ASSERT_EQ(picojson::value(obj2).serialize(),picojson::value(c2->GetDetails()).serialize());
}

TEST_F(AllFactoryUnitTests, PackageFactory) {
    IEntity* pack = pFac->CreateEntity(obj3);
    Package* p = static_cast<Package*>(pack);
    ASSERT_EQ(5,p->GetPosition().at(0));
    ASSERT_EQ(5,p->GetPosition().at(1));
    ASSERT_EQ(5,p->GetPosition().at(2));

    ASSERT_NEAR(0.57735026919,p->GetDirection().at(0),.001);
    ASSERT_NEAR(0.57735026919,p->GetDirection().at(1),.001);
    ASSERT_NEAR(0.57735026919,p->GetDirection().at(2),.001);

    ASSERT_EQ("charlie",p->GetName());
    ASSERT_FLOAT_EQ(1,p->GetRadius());

    Customer* c = new Customer(JsonHelper::CreateJsonObject());
    p->setCustomer(c);
    EXPECT_FALSE(p->getCustomer() == nullptr);
    ASSERT_FLOAT_EQ(0,p->getDestination().getX());
    ASSERT_FLOAT_EQ(0,p->getDestination().getY());
    ASSERT_FLOAT_EQ(0,p->getDestination().getZ());

    EXPECT_FALSE(p->IsDynamic());

    ASSERT_EQ(0,p->GetVersion());
    ASSERT_EQ(picojson::value(obj3).serialize(),picojson::value(p->GetDetails()).serialize());
}

TEST_F(AllFactoryUnitTests, CompositeFactory) {
    IEntity* drone = compFac->CreateEntity(obj1);
    IEntity* customer = compFac->CreateEntity(obj2);
    IEntity* package = compFac->CreateEntity(obj3);
    ASSERT_FALSE(drone == nullptr);
    ASSERT_FALSE(customer == nullptr);
    ASSERT_FALSE(package == nullptr);
    Drone* d = static_cast<Drone*>(drone);
    Customer* c2 = static_cast<Customer*>(customer);
    Package* p = static_cast<Package*>(package);
    EXPECT_EQ("charlie",d->GetName());
    ASSERT_FLOAT_EQ(1,d->GetRadius());
    ASSERT_FLOAT_EQ(1,d->GetSpeed());

    ASSERT_FLOAT_EQ(1,d->GetPosition().at(0));
    ASSERT_FLOAT_EQ(1,d->GetPosition().at(1));
    ASSERT_FLOAT_EQ(1,d->GetPosition().at(2));
    EXPECT_EQ(3,d->GetPosition().size());

    ASSERT_NEAR(-0.57735026919,d->GetDirection().at(0),.001);
    ASSERT_NEAR(-0.57735026919,d->GetDirection().at(1),.001);
    ASSERT_NEAR(-0.57735026919,d->GetDirection().at(2),.001);
    EXPECT_EQ(3,d->GetDirection().size());
    EXPECT_EQ(0,d->GetVersion());
    ASSERT_EQ(false,d->holdingPackage());
    ASSERT_EQ(false,d->getMoving());
    EXPECT_EQ(false,d->IsDynamic());

    ASSERT_EQ(picojson::value(obj1).serialize(),picojson::value(d->GetDetails()).serialize());
    ASSERT_FLOAT_EQ(10,c2->GetPosition().at(0));
    ASSERT_FLOAT_EQ(10,c2->GetPosition().at(1));
    ASSERT_FLOAT_EQ(10,c2->GetPosition().at(2));

    ASSERT_NEAR(0.57735026919,c2->GetDirection().at(0),.001);
    ASSERT_NEAR(0.57735026919,c2->GetDirection().at(1),.001);
    ASSERT_NEAR(0.57735026919,c2->GetDirection().at(2),.001);

    ASSERT_EQ(3,c2->GetPosition().size());
    ASSERT_EQ(3,c2->GetDirection().size());

    ASSERT_EQ("beta",c2->GetName());
    ASSERT_FLOAT_EQ(1,c2->GetRadius());
    ASSERT_EQ(0,c2->GetVersion());
    ASSERT_EQ(false,c2->IsDynamic());
    c2->setDynamic(true);
    ASSERT_EQ(true,c2->IsDynamic());

    ASSERT_EQ(picojson::value(obj2).serialize(),picojson::value(c2->GetDetails()).serialize());
    ASSERT_EQ(5,p->GetPosition().at(0));
    ASSERT_EQ(5,p->GetPosition().at(1));
    ASSERT_EQ(5,p->GetPosition().at(2));

    ASSERT_NEAR(0.57735026919,p->GetDirection().at(0),.001);
    ASSERT_NEAR(0.57735026919,p->GetDirection().at(1),.001);
    ASSERT_NEAR(0.57735026919,p->GetDirection().at(2),.001);

    ASSERT_EQ("charlie",p->GetName());
    ASSERT_FLOAT_EQ(1,p->GetRadius());

    Customer* c = new Customer(JsonHelper::CreateJsonObject());
    p->setCustomer(c);
    EXPECT_FALSE(p->getCustomer() == nullptr);
    ASSERT_FLOAT_EQ(0,p->getDestination().getX());
    ASSERT_FLOAT_EQ(0,p->getDestination().getY());
    ASSERT_FLOAT_EQ(0,p->getDestination().getZ());

    EXPECT_FALSE(p->IsDynamic());

    ASSERT_EQ(0,p->GetVersion());
    ASSERT_EQ(picojson::value(obj3).serialize(),picojson::value(p->GetDetails()).serialize());
}

}  // namespace csci3081
