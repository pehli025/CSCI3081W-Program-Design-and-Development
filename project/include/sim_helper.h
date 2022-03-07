/**
 * @file sim_helper.h
 */
#ifndef SIM_HELPER_H
#define SIM_HELPER_H

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <EntityProject/facade/delivery_system.h>
#include "composite_factory.h"
#include <vector>
#include <string>

namespace csci3081 {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief This header file is a container for declarations of helper methods to help with the logic
 * of the DeliverySimulation class.
 * This class contains methods to schedule and update entities.
 */

/**
* Handles the logic of scheduling a delivery between two entities, package and dest.
* package->GetType() should return "package" and
* dest->GetType() should return "customer".
* @param[in] package A pointer to an IEntity describing the package location
* @param[in] dest A pointer to an IEntity describing the location the package should be delivered to
* @param[in] entities A reference to a list of pointers to all entities in the simulation. This set
* is used to choose which entity is handling the delivery of the package
*/
void schedule(IEntity* package, IEntity* dest, std::vector<IEntity*>& entities);

/**
* Handles the logic of updating the status of every entity in the simulation. Calls update() on
* entities that inherit from DeliveryVehicle,
* starts a new delivery if an inheritor from DeliveryVehicle is not currently making a delivery,
* and also handles rescheduling logic if an
* inheritor from DeliveryVehicle battery has died while making a delivery.
* @param[in] dt The amount of time that has passed in the simulation. This value comes from the user
* as they change the value on the "simulation speed" slider on the webpage.
* @param[in] entities A reference to a list of pointers to all entities in the simulation. All
* entities in this list will have their status updated each frame.
* @param[in] graph A pointer to a graph of nodes representing the pathing in the simulation scene.
* This graph will be used by entities to update their position.
*/
void updateEntities(float dt, std::vector<IEntity*>& entities, const IGraph* graph);

}


#endif  // DELIVERY_SIMULATION_H_
