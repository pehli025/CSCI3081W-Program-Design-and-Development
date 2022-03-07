/**
 * @file drone_factory.h
 */
#ifndef DRONE_FACTORY_H
#define DRONE_FACTORY_H

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <EntityProject/facade/delivery_system.h>
#include <vector>
#include <string>

namespace csci3081 {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief DroneFactory class used by the CompositeFactory class to help with the creation of Drones.
 * Called by the CompositeFactory class.
 */
class DroneFactory : public entity_project::IEntityFactory {
    public:
        /**
        * Default constructor does nothing. Class is used to create drone objects.
        */
        DroneFactory();

        /**
        * Creates and returns an IEntity* specified by the picojson::object passed in. Returns null
        * if the picojson::object does not specify a drone. Called by the CompositeFactory class.
        * @param[in] detail A picojson object blueprinting the details of the object's creation.
        * @return An IEntity pointer to the newly created drone. Returns null if the
        * picojson::object does not specify a drone.
        */
        IEntity* CreateEntity(const picojson::object& detail);

};

}  // namespace csci3081


#endif  // DRONE_FACTORY_H
