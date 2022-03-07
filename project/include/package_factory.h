/**
 * @file package_factory.h
 */
#ifndef PACKAGE_FACTORY_H
#define PACKAGE_FACTORY_H

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
 * @brief PackageFactory class used by the CompositeFactory class to help with the creation of Packages.
 * Called by the CompositeFactory class.
 */
class PackageFactory : public entity_project::IEntityFactory {
    public:
        /**
        * Constructor: Does nothing, class is a wrapper for creating Package entities.
        */
        PackageFactory();

        /**
        * Creates a package entity based on the picojson::object passed in. Returns null if the
        * picojson::object type is not a package. Called by the CompositeFactory class
        * @param[in] detail A picojson::object containing the specifics for constructing the package.
        * @return An IEntity pointer to the newly created package. Returns null if the
        * picojson::object does not specify a package.
        */
        IEntity* CreateEntity(const picojson::object& detail);


    private:
};

}  // namespace csci3081


#endif  // PACKAGE_FACTORY_H
