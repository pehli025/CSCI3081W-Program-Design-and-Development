/**
 * @file composite_factory.h
 */
#ifndef COMPOSITE_FACTORY_H
#define COMPOSITE_FACTORY_H

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
 * @brief Composite factory class that is used to generate Entity objects in the simulation.
 * Calls the Customer/Drone/Package Factory classes recursively to instantiate the objects.
 */
class CompositeFactory : public entity_project::IEntityFactory {
    public:

        /**
        * Constructor: Does nothing. The factories added to the composite factory are up to the simulation
        */
        CompositeFactory();

        /**
        * Adds an IEntityFactory pointer to the composite factory.
        * @param[in] newFact Pointer to an IEntityFactory object to add to the list of factories inside this composite factory.
        */
        void AddFactory(IEntityFactory* newFact);

        /**
        * Creates and returns a pointer to an IEntity object specified by the picojson object passed in.
        * Actual object creation is left to the subfactories.
        * @param[in] detail A picojson::object specifying the details of the object's construction.
        */
        IEntity* CreateEntity(const picojson::object& detail);

        /**
        * Destructor: Frees all the IEntityFactory* objects in factories.
        */
        ~CompositeFactory();

    private:
        std::vector<IEntityFactory*> factories;

};

}  // namespace csci3081


#endif  // COMPOSITE_FACTORY_H
