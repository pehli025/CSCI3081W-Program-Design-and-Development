/**
 * @file robot_factory.h
 */
#ifndef ROBOT_FACTORY_H
#define ROBOT_FACTORY_H

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
 * @brief RobotFactory class used by the CompositeFactory class to help with the creation of Robots.
 * Called by the CompositeFactory class.
 */
class RobotFactory : public entity_project::IEntityFactory {
    public:
        /**
        * Default constructor does nothing. Class is used to create robot objects.
        */
        RobotFactory();

        /**
        * Creates and returns an IEntity* specified by the picojson::object passed in. Returns null
        * if the picojson::object does not specify a robot. Called by the CompositeFactory class.
        * @param[in] detail A picojson object blueprinting the details of the object's creation.
        * @return An IEntity pointer to the newly created robot. Returns null if the picojson::object
        * does not specify a robot.
        */
        IEntity* CreateEntity(const picojson::object& detail);

};

}  // namespace csci3081


#endif  // ROBOT_FACTORY_H
