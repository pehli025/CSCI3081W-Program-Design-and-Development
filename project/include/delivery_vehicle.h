#ifndef DELIVERY_VEHICLE_H
#define DELIVERY_VEHICLE_H

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
#include "strategy.h"
#include <queue>

namespace csci3081 {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief An abstract class representing a delivery vehicle. Contains methods to move the delivery
 * vehicle and the package it is holding if it is holding it.
 *
 */
class DeliveryVehicle : public csci3081::EntityBase {
    public:
        /**
        * Constructor: Reads in the picojson::object and sets remaining values based on the values
        * in val. Does not depend on type of delivery vehicle
        * @param[in] val A picojson object specifying the delivery vehicle.
        */
        DeliveryVehicle(const picojson::object& val);

        /**
        * Default constructor: This is needed to instantiate vehicles without specifying any
        * information. This is primarily used by decorators since they do not have any picojson
        * details associated with them and instead use the object that was created
        * with a picojson object already.
        */
        DeliveryVehicle();

        /**
        * Returns a boolean indicating whether the delivery vehicle is carrying a package or not.
        * @return A boolean signifying whether the delivery vehicle is carrying a package.
        */
        virtual bool holdingPackage();

        /**
        * Returns a boolean indicating whether the delivery vehicle is moving or not.
        * @return A boolean signifying whether the delivery vehicle is moving in the simulation.
        *
        */
        virtual bool getMoving();

        /**
        * Returns a queue of all the deliveries assigned to this delivery vehicle.
        * @return A queue of std::vector<IEntity*> which have size 2. The first element in the vector
        * is the package package pointer, the second element in the vector is the customer pointer.
        */
        virtual std::queue<std::vector<IEntity*>> getQueuedDeliveries();

        /**
        * Returns the top speed of the delivery vehicle.
        * @return A float describing the maximum speed the delivery vehicle can attain.
        */
        virtual float GetSpeed() const;

        /**
        * While delivering, the package holds the package assigned to the delivery vehicle. This method
        * returns a pointer to that package.
        * @return A pointer to the package assigned to this delivery vehicle.
        */
        virtual Package* getPackage() const;

        /**
        * Returns the battery held by the drone to be used by outside users such as DeliverySimulation
        * when checking for battery health to reschedule
        * @return A reference to the battery used by this delivery vehicle.
        */
        virtual Battery& getBattery();

        /**
        * Returns whether the delivery vehicle needs its current delivery and all future deliveries to
        * be allocated to another drone.
        * @return A boolean indicating whether we should reallocate all deliveries.
        */
        virtual bool getRescheduleFlag() const;

        /**
        * 
        * @return A list of 3D coordinates in a vector of vectors representing the route the vehicle will take from the package to the customer.
        */
        virtual std::vector<std::vector<float>> getPackageToCust();

        /**
        * @return A list of 3D coordinates in a vector of vectors representing the route the vehicle will take from its initial position to the package.
        *
        */
        virtual std::vector<std::vector<float>> getToPackage();

        /**
        *
        * @return A list of 3D coordinates in a vector of vectors representing the route the vehicle will take from its initial position, to the package, and then to the customer.
        */
        virtual std::vector<std::vector<float>> getPath();

        /**
        * Moves the delivery vehicle to a location specified by the Vector3D dest by updating the
        * delivery vehicle's Position member variable.
        *
        * @param[in] dest A Vector3D object describing the coordinates of the delivery vehicle's target
        * destination to move to.
        * @param[in] dt The amount of time to move the delivery vehicle.  This is so the delivery
        * vehicle does not move faster than its top speed.
        */
        virtual void moveTo(std::vector<float> floatDest, float dt) = 0; // Will be overridden

        /**
        * Puts a delivery into the delivery vehicle's queue of deliveries to make.
        * @param[in] pack A pointer to an IEntity describing the object to deliver.
        * @param[in] dest A pointer to an IEntity describing where to deliver the package.
        */
        virtual void enqueueDelivery(IEntity* pack, IEntity* dest);

        /**
        * Sets all the flags of the delivery vehicle if the package the vehicle is delivering needs
        * to be rescheduled. Actual rescheduling is handled in delivery_simulation.cc
        */
        virtual void updateReschedule();

        /**
        * Assigns the delivery vehicle to the package specified by the IEntity pointer passed in.
        * @param[in] package An IEntity* pointing to the package assigned to the delivery vehicle.
        */
        virtual void setPackage(IEntity* package);

        /**
        * Sets the delivery vehicle's packageToCust member variable to a vector of vectors passed.
        * This is primarily used for testing purposes to set the path manually.
        * @param[in] p a vector containing vectors which contain 3 floats. This can be generated by
        * an IStrategy or set manually.
        */
        virtual void setPackageToCust(std::vector<std::vector<float>> p);

        /**
        * Sets the delivery vehicle's toPackage member variable to a vector of vectors passed.
        * This is primarily used for testing purposes to set the path manually.
        * @param[in] p a vector containing vectors which contain 3 floats. This can be generated by
        * an IStrategy or set manually.
        */
        virtual void setToPackage(std::vector<std::vector<float>> p);

        /**
        * Sets the delivery vehicle's path member variable to the vector of vectors passed in so it
        * can make deliveries.
        * @param[in] p a vector containing vectors which contain 3 floats. This path is generated by
        * an IStrategy's GetPath() method.
        * p should contain both from the drone's current location to the package and from the
        * package to the customer.
        */
        virtual void setPath(std::vector<std::vector<float>> p);

        /**
        * Sends a message to all observers of the delivery vehicle that it has stopped moving.
        *
        */
        virtual void notifyIdle();

        /**
        * Sends a message to all observers of the delivery vehicle that it is on its way to the package
        *
        */
        virtual void notifyToPackage();

        /**
        * Sends a message to all observers of the delivery vehicle that it is on its way to the
        * customer.
        */
        virtual void notifyToCustomer();

        /**
        * If the delivery vehicle is not on a delivery, when it is updated it will start a new delivery.
        * @param[in] graph an IGraph object describing the paths on the simulation.
        */
        virtual void startNewDelivery(const IGraph* graph);

        /**
        * Moves the delivery vehicle forward in time. If it is on a delivery, the delivery vehicle's
        * position will be updated along with pickups or dropoffs on packages.
        * Also drains the battery.
        *
        * @param[in] dt The amount of time to move the delivery vehicle by.
        */
        virtual void update(float dt) = 0;

        /**
        * Sends a message to the observer that the DeliveryVehicle's color is being updated.
        * @param[in] hexColor A string representing the RGB hex value to set the vehicle's color. Format should be something like "0xFFFFFF" where the last 6 characters of the string represent 3 hex numbers each with 2 digits representing the RGB value of the vehicle.
        */
        virtual void setColor(std::string hexColor);

    protected:
        Battery battery;
        Package* package;
        float speed;
        bool isMoving;
        bool hasPackage;
        bool rescheduleFlag;
        std::vector<std::vector<float>> path;
        std::vector<std::vector<float>> toPackage;
        std::vector<std::vector<float>> packageToCust;
        int destIndex;
        std::queue<std::vector<IEntity*>> queuedDeliveries;
        IStrategy* strategy;

};

}

#endif
