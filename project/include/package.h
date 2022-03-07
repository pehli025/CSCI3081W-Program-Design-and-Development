#ifndef PACKAGE_H
#define PACKAGE_H

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "entity_base.h"
#include "customer.h"
#include <vector>
#include <string>
#include "vector3d.h"
#include "vector2d.h"

namespace csci3081 {
/**
 * @brief A class representing a package. Contains methods to get the customer and the destination
 * of the package.
 *
 */
class Package : public csci3081::EntityBase {
    public:

        /**
        * Constructor: Reads in the picojson object passed in and creates a new package based off
        * the details in the picojson::object
        * @param[in] val A picojson::object specifying the package details.
        */
        Package(const picojson::object& val);

        /**
        * Returns the customer this package is being delivered to.
        * @return Returns the customer that this package was assigned to when it was scheduled for
        * delivery.
        */
        Customer* getCustomer();

        /**
        * Gets the location of the package's destination (customer destination).
        * @return An object of type Vector3D specifying the coordinates of the package's
        * destination.
        */
        Vector3D getDestination();

        /**
        * Sets the package's customer member variable to the IEntity pointer passed in.
        * @param[in] c A pointer to a customer.
        */
        void setCustomer(Customer* c);

        /**
        * Sends a message to all observers of the package that it has been scheduled
        */
        void notifyScheduled();

        /**
        * Sends a message to all observers of the package that it is en route to the customer
        */
        void notifyEnRoute();

        /**
        * Sends a message to all observers of the package that it has been delivered to the
        * customer.
        */
        void notifyDelivered();

    private:
        Vector3D dest;
        float weight;
        Customer* cust;

};

}

#endif  // PACKAGE_H
