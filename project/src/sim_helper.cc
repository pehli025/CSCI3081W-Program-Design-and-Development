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
#include <algorithm>
#include <queue>
#include <iostream>

namespace csci3081 {

void schedule(IEntity* package, IEntity* dest, std::vector<IEntity*>& entities) {
	// Cast IEntity* to EntityBase* since the GetType method is in the EntityBase class.
	EntityBase* castPackage = static_cast<EntityBase*>(package);
	EntityBase* castDest = static_cast<EntityBase*>(dest);

	// Warn users that their deliveries are weird and the types should be package and customer.
	if (castPackage->GetType().compare("package") != 0) {
		std::cout << "Warning, deliverable entity type is not a package. Proceeding anyways."
							<< std::endl;
	}
	if (castDest->GetType().compare("customer") != 0) {
		std::cout << "Warning delivery location entity type is not a customer. Proceeding anyways."
							<< std::endl;
		std::cout << "Delivery location is of actual type: " << castDest->GetType() << std::endl;
	}

	// Iterate through all entities in simulation and find drone with least number of deliveries
	int minimumEnqueuedDeliveries = 9999999;
	DeliveryVehicle* fewestDeliveryDrone = nullptr;
	for (int i = 0; i < entities.size(); i++) {
		EntityBase* castEntity = static_cast<EntityBase*>(entities.at(i));
		if (castEntity->GetType().compare("drone") == 0 || castEntity->GetType().compare("robot") == 0) {
			DeliveryVehicle* currDrone = static_cast<DeliveryVehicle*>(entities.at(i));
			// If positive battery and smaller queued deliveries
			if (currDrone->getQueuedDeliveries().size() < minimumEnqueuedDeliveries &&
				currDrone->getBattery().getRemLife() > 0) {
				fewestDeliveryDrone = currDrone;
				minimumEnqueuedDeliveries = currDrone->getQueuedDeliveries().size();
			}
			// If we're a drone and not holding a package, we'll schedule it.
		}
	}
	if (fewestDeliveryDrone == nullptr) {
		std::cout << "No entity in the simulation that can perform deliveries. Not scheduling delivery"
							<< std::endl;
		return;
	}
	fewestDeliveryDrone->enqueueDelivery(package,dest);
}

void updateEntities(float dt, std::vector<IEntity*>& entities, const IGraph* graph) {
	std::vector<std::string> deliveryVehicleTypes{"drone","robot"}; // Enum for delivery vehicles
	for (int i = 0; i < entities.size(); i++) {
		EntityBase* castEntity = static_cast<EntityBase*>(entities.at(i));

		// See a delivery vehicle
		if (std::find(std::begin(deliveryVehicleTypes), std::end(deliveryVehicleTypes),
			castEntity->GetType()) != std::end(deliveryVehicleTypes)) {
			DeliveryVehicle* vehicle = static_cast<DeliveryVehicle*>(castEntity);
			if (vehicle->getRescheduleFlag()) {
				Package* rescheduledPackage = vehicle->getPackage();
				Customer* rescheduledCustomer = rescheduledPackage->getCustomer();
				// Reschedule the delivery the vehicle is on
				schedule(rescheduledPackage,rescheduledCustomer, entities);
				// Reschedule all future deliveries the vehicle is on
				while (vehicle->getQueuedDeliveries().size() != 0) {
					// Copy front of queue into front
					std::vector<IEntity*> front = vehicle->getQueuedDeliveries().front();
					vehicle->getQueuedDeliveries().pop(); // Pop from front of queue
					schedule(front.at(0), front.at(1), entities);
				}
				vehicle->updateReschedule();
			}
			// Start new delivery if no package assigned to drone
			else if (vehicle->getPackage() == nullptr) {
				vehicle->startNewDelivery(graph);
			}
			else { // Move drone regularly if package
				vehicle->update(dt);
			}
		}
	}
}

}
