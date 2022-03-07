#include "delivery_simulation.h"
#include "entity_base.h"
#include "json_helper.h"
#include "drone.h"
#include "robot.h"
#include "customer.h"
#include "package.h"
#include "drone_factory.h"
#include "package_factory.h"
#include "customer_factory.h"
#include "robot_factory.h"
#include "delivery_vehicle.h"
#include "dvec_decorator.h"
#include "sim_helper.h"
#include <algorithm>
#include <queue>
#include <iostream>

namespace csci3081 {

DeliverySimulation::DeliverySimulation() {
	compFactory.AddFactory(new DroneFactory());
	compFactory.AddFactory(new CustomerFactory());
	compFactory.AddFactory(new PackageFactory());
	compFactory.AddFactory(new RobotFactory());
}

DeliverySimulation::~DeliverySimulation() {}

IEntity* DeliverySimulation::CreateEntity(const picojson::object& val) {
	IEntity* ret = compFactory.CreateEntity(val);
	// Attach observers to new entity
	for (IEntityObserver* obs : observers) {
		if (obs != nullptr) {
			EntityBase* castRet = static_cast<EntityBase*>(ret);
			castRet->attach(obs);
		}
	}
  	return ret;
}

void DeliverySimulation::AddFactory(IEntityFactory* factory) {
	std::cout << "Factory added automatically" << std::endl;
	compFactory.AddFactory(factory);
}

void DeliverySimulation::AddEntity(IEntity* entity) {
  entities_.push_back(entity);
}

void DeliverySimulation::SetGraph(const IGraph* graph) { this->graph = graph; }

void DeliverySimulation::ScheduleDelivery(IEntity* package, IEntity* dest) {
	schedule(package,dest,entities_);
}

void DeliverySimulation::AddObserver(IEntityObserver* observer) {
	this->observers.push_back(observer); // Just add it to the list of observers in delivery sim
}

void DeliverySimulation::RemoveObserver(IEntityObserver* observer) {
	observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
	for (IEntity* ent : entities_) { // Remove observer from every subject as well
		if (ent != nullptr) {
			// Cast to EntityBase since it multiply inherits EntitySubject which has observers
			EntityBase* castEntity = static_cast<EntityBase*>(ent);
			castEntity->detach(observer);
		}
	}
}

const std::vector<IEntity*>& DeliverySimulation::GetEntities() const { return entities_; }

void DeliverySimulation::Update(float dt) {
	updateEntities(dt, entities_, this->graph);
}


// DO NOT MODIFY THE FOLLOWING UNLESS YOU REALLY KNOW WHAT YOU ARE DOING
void DeliverySimulation::RunScript(const picojson::array& script, IEntitySystem* system) const {
  JsonHelper::PrintArray(script);
  IDeliverySystem* deliverySystem = dynamic_cast<IDeliverySystem*>(system);
	if (deliverySystem) {

	    // To store the unadded entities_
	    std::vector<IEntity*> created_entities;

		for (unsigned int i=0; i < script.size(); i++) {
			const picojson::object& object = script[i].get<picojson::object>();
			const std::string cmd = object.find("command")->second.get<std::string>();
			const picojson::object& params = object.find("params")->second.get<picojson::object>();
			// May want to replace the next few if-statements with an enum
			if (cmd == "createEntity") {
				IEntity* entity = NULL;
				entity = deliverySystem->CreateEntity(params);
				if (entity) {
					created_entities.push_back(entity);
				} else {
					std::cout << "Null entity" << std::endl;
				}
			}
			else if (cmd == "addEntity") {
				int ent_index = static_cast<int>(params.find("index")->second.get<double>());
				if (ent_index >= 0 && ent_index < created_entities.size()) {
					deliverySystem->AddEntity(created_entities[ent_index]);
				}
			}
			else if (cmd == "scheduleDelivery" ) {
				int pkg_index = static_cast<int>(params.find("pkg_index")->second.get<double>());
				int dest_index = static_cast<int>(params.find("dest_index")->second.get<double>());
				if (pkg_index >= 0 && pkg_index < system->GetEntities().size()) {
					IEntity* pkg = deliverySystem->GetEntities()[pkg_index];
					if (dest_index >= 0 && pkg_index < system->GetEntities().size()) {
						IEntity* cst = system->GetEntities()[dest_index];
						if (pkg && cst) {
							deliverySystem->ScheduleDelivery(pkg, cst);
						}
					}
				}
				else {
					std::cout << "Failed to schedule delivery: invalid indexes" << std::endl;
				}
			}
		}
	}
}

}
