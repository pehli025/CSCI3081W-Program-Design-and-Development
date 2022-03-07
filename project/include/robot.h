#ifndef ROBOT_H
#define ROBOT_H

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "entity_base.h"
#include "package.h"
#include "battery.h"
#include <vector>
#include <string>
#include "vector3d.h"
#include "vector2d.h"
#include <queue>
#include "delivery_vehicle.h"

namespace csci3081 {
/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief A class representing a robot. Contains methods to move the robot and the package it is
 * holding if it is holding it.
 *
 * Robots behave exactly like robots we implemented in the first iteration except they are grounded
 * (cannot fly) so their coordinates are restricted.
 */
class Robot : public csci3081::DeliveryVehicle {
    public:
    /**
    * Constructor: Reads in the picojson::object and creates a robot based on the values in val.
    * @param[in] val A picojson object specifying the robot.
    */
        Robot(const picojson::object& val);

    /**
    * Moves the robot to a location specified by the Vector3D dest by updating the robot's Position
    * member variable. Overriding method in DeliveryVehicle
    * @param[in] dest A Vector3D object describing the coordinates of the robot's target destination
    * to move to.
    * @param[in] dt The amount of time to move the robot.  This is so the robot does not move faster
    * than its top speed.
    *
    */
        void moveTo(std::vector<float> floatDest, float dt) override;

    /**
    * Moves the robot forward in time. If it is on a delivery, the robot's position will be updated
    * along with pickups or dropoffs on packages.
    * Also drains the battery.
    *
    * @param[in] dt The amount of time to move the robot by.
    */
        void update(float dt) override;

};

}

#endif
