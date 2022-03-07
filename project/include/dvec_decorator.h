/**
 * @file dvec_decorator.h
 */
#ifndef DVEC_DECORATOR
#define DVEC_DECORATOR

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <EntityProject/facade/delivery_system.h>
#include "delivery_vehicle.h"
#include <vector>
#include <string>

namespace csci3081 {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief A general decorator for delivery vehicles.
 *
 * This class will handle decorating the drone generically. This class passes all function calls to the DeliveryVehicle it points to.
 * All inheriting classes of this one will be responsible for the actual implementation of decoration.
 *
 */
class DVecDecorator : public DeliveryVehicle {
    public:
        /**
        * Constructor: Sets up delivery vehicle decorator. Decorates updating functionality of
        * vechicle.
        * numUpdates initially set at 0.
        */
        DVecDecorator(DeliveryVehicle* dvec);

        /**
        * Moves the delivery vehicle to a location specified by the Vector3D dest by updating the
        * delivery vehicle's Position member variable.
        *
        * @param[in] dest A Vector3D object describing the coordinates of the delivery vehicle's
        * target destination to move to.
        * @param[in] dt The amount of time to move the delivery vehicle.  This is so the delivery
        * vehicle does not move faster than its top speed.
        */
        void moveTo(std::vector<float> floatDest, float dt) override;

        /**
        * Moves the delivery vehicle forward in time. Decorates functionality of vehicle.
        * The only virtual method in dvec_decorator so extending classes can decorate update.
        *
        * @param[in] dt The amount of time to move the delivery vehicle by.
        */
        virtual void update(float dt) override;

        /**
        * Returns a boolean indicating whether the delivery vehicle is carrying a package or not.
        * @return A boolean signifying whether the delivery vehicle is carrying a package.
        */
        bool holdingPackage() override;

        /**
        * Returns a boolean indicating whether the delivery vehicle is moving or not.
        * @return A boolean signifying whether the delivery vehicle is moving in the simulation.
        *
        */
        bool getMoving() override;

        /**
        * Returns a queue of all the deliveries assigned to this delivery vehicle.
        * @return A queue of std::vector<IEntity*> which have size 2. The first element in the
        * vector is the package package pointer, the second element in the vector is the customer
        * pointer.
        */
        std::queue<std::vector<IEntity*>> getQueuedDeliveries() override;

        /**
        * Returns the top speed of the delivery vehicle.
        * @return A float describing the maximum speed the delivery vehicle can attain.
        */
        float GetSpeed() const override;

        /**
        * While delivering, the package holds the package assigned to the delivery vehicle. This method returns a pointer to that package.
        * @return A pointer to the package assigned to this delivery vehicle.
        */
        Package* getPackage() const override;

        /**
        * Returns the battery held by the drone to be used by outside users such as DeliverySimulation when checking for battery health to reschedule
        * @return A reference to the battery used by this delivery vehicle.
        */
        Battery& getBattery() override;

        /**
        * Returns whether the delivery vehicle needs its current delivery and all future deliveries to be allocated to another drone.
        * @return A boolean indicating whether we should reallocate all deliveries.
        */
        bool getRescheduleFlag() const override;

        /**
        *
        *
        * @return A list of 3D coordinates in a vector of vectors representing the route the vehicle will take from the package to the customer.
        */
        std::vector<std::vector<float>> getPackageToCust() override;

        /**
        *
        * @return A list of 3D coordinates in a vector of vectors representing the route the vehicle will take from its initial position to the package.
        */
        std::vector<std::vector<float>> getToPackage() override;

        /**
        *
        * @return A list of 3D coordinates in a vector of vectors representing the route the vehicle will take from its initial position, to the package, and then to the customer.
        */
        std::vector<std::vector<float>> getPath() override;

        /**
        * Puts a delivery into the delivery vehicle's queue of deliveries to make.
        * @param[in] pack A pointer to an IEntity describing the object to deliver.
        * @param[in] dest A pointer to an IEntity describing where to deliver the package.
        */
        void enqueueDelivery(IEntity* pack, IEntity* dest) override;

        /**
        * Sets all the flags of the delivery vehicle if the package the vehicle is delivering needs to be rescheduled.
        * Actual rescheduling is handled in delivery_simulation.cc
        */
        void updateReschedule() override;

        /**
        * Assigns the delivery vehicle to the package specified by the IEntity pointer passed in.
        * @param[in] package An IEntity* pointing to the package assigned to the delivery vehicle.
        */
        void setPackage(IEntity* package) override;

        /**
        * Sets the delivery vehicle's packageToCust member variable to a vector of vectors passed.
        * This is primarily used for testing purposes to set the path manually.
        * @param[in] p a vector containing vectors which contain 3 floats. This can be generated by an IStrategy or set manually.
        */
        void setPackageToCust(std::vector<std::vector<float>> p) override;

        /**
        * Sets the delivery vehicle's toPackage member variable to a vector of vectors passed.
        * This is primarily used for testing purposes to set the path manually.
        * @param[in] p a vector containing vectors which contain 3 floats. This can be generated by an IStrategy or set manually.
        */
        void setToPackage(std::vector<std::vector<float>> p) override;

        /**
        * Sets the delivery vehicle's path member variable to the vector of vectors passed in so it can make deliveries.
        * @param[in] p a vector containing vectors which contain 3 floats. This path is generated by an IStrategy's GetPath() method.
        * p should contain both from the drone's current location to the package and from the package to the customer.
        */
        void setPath(std::vector<std::vector<float>> p) override;

        /**
        * Sends a message to all observers of the delivery vehicle that it has stopped moving.
        *
        */
        void notifyIdle() override;

        /**
        * Sends a message to all observers of the delivery vehicle that it is on its way to the package
        *
        */
        void notifyToPackage() override;

        /**
        * Sends a message to all observers of the delivery vehicle that it is on its way to the customer.
        *
        */
        void notifyToCustomer() override;

        /**
        * Sends a message to the observer that the DeliveryVehicle's color is being updated.
        * @param[in] hexColor A string representing the RGB hex value to set the vehicle's color. Format should be something like "0xFFFFFF" where the last 6 characters of the string represent 3 hex numbers each with 2 digits representing the RGB value of the vehicle.
        */
        void setColor(std::string hexColor) override;

        /**
        * If the delivery vehicle is not on a delivery, when it is updated it will start a new delivery.
        * @param[in] graph an IGraph object describing the paths on the simulation.
        */
        void startNewDelivery(const IGraph* graph) override;

        // Overrided methods from entitybase
        int GetId() const override;
        const std::string& GetName() override;
        const std::string& GetType() const override; // Not overrided yet
        const std::vector<float>& GetPosition() const override;
        const std::vector<float>& GetDirection() const override;
        float GetRadius() const override;
        int GetVersion() const override; // Not used in iteration 1
        bool IsDynamic() const override;
        void setPosition(std::vector<float> p) override; // Nothing to override
        void setDynamic(bool status) override;
        void setDirection(std::vector<float> d) override;
        const picojson::object& GetDetails() override;

        // Overrided methods from entity_subject (observers)
        void attach(IEntityObserver* observer) override;
        void detach(IEntityObserver* observer) override;
        void notify(picojson::object& msg, IEntity& ent) override;

    protected:
        DeliveryVehicle* const decoratedVehicle;
        int numUpdates;

};

}  // namespace csci3081


#endif  // Dvec_Decorator.h
