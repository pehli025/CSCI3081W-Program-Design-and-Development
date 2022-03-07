#ifndef DRONE_H
#define DRONE_H

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
 * @brief A class representing a drone. Contains methods to move the drone and the package it is
 * holding if it is holding it.
 *
 */
class Drone : public csci3081::DeliveryVehicle {
    public:
    /**
    * Constructor: Reads in the picojson::object and creates a drone based on the values in val.
    * @param[in] val A picojson object specifying the drone.
    */
        Drone(const picojson::object& val);


    /**
    * Moves the drone to a location specified by the Vector3D dest by updating the drone's Position
    * member variable.
    * @param[in] dest A Vector3D object describing the coordinates of the drone's target destination
    * to move to.
    * @param[in] dt The amount of time to move the drone.  This is so the drone does not move faster
    * than its top speed.
    *
    */
        void moveTo(std::vector<float> floatDest, float dt) override; // Overriding entity base


    /**
    * Moves the drone forward in time. If it is on a delivery, the drone's position will be updated
    * along with pickups or dropoffs on packages.
    * Also drains the battery.
    *
    * @param[in] dt The amount of time to move the drone by.
    */
        void update(float dt) override;
};

}

#endif  //DRONE_H
