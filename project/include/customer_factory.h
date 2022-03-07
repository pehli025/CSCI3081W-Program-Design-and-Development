/**
 * @file customer_factory.h
 */
#ifndef CUSTOMER_FACTORY_H
#define CUSTOMER_FACTORY_H

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
 * @brief Customer factory class used by the CompositeFactory class to instantiate Customers.
 * Called by the CompositeFactory class.
 */
class CustomerFactory : public entity_project::IEntityFactory {
    public:
        /**
        * Constructor: Defaults to nothing. This class is meant to generate customer objects.
        */
        CustomerFactory();

        /**
        * Creates and returns an entity object specified by the picojson object passed in. Called by
        * the composite factory class.
        * @param[in] detail A picojson::object specifying the details of the object's construction.
        * @return An IEntity* to the newly created customer. Returns null if the picojson object is
        * not for a customer.
        */
        IEntity* CreateEntity(const picojson::object& detail);
    private:
};

}  // namespace csci3081


#endif  // CUSTOMER_FACTORY_H
