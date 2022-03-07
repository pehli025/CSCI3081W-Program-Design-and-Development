/**
 * @file entity_subject.h
 */
#ifndef ENTITY_SUBJECT_H
#define ENTITY_SUBJECT_H

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
 * @brief Entity subject class that is used by entity objects to notify observers of the subject.
 * Notifies any observers that are added to the delivery simulation.
 */
class EntitySubject {
    public:
        /**
        * Constructor: Does nothing. This class just contains methods to add, remove and notify observers. Observers will be added via DeliverySimulation
        */
        EntitySubject();

        /**
        * Adds an IEntityObserver pointer to the observer we wish to notify
        * @param[in] observer Pointer to an IEntityObserver object to add to the list of observers inside the subject.
        */
        virtual void attach(IEntityObserver* observer);

        /**
        * Removes the IEntityObserver pointer from the list of observers in the subject.
        * @param[in] observer Pointer to the IEntityObserver object we wish to remove from the list of observers inside the subject.
        */
        virtual void detach(IEntityObserver* observer);

        /**
        * Notifies all observers watching the EntitySubject.
        * @param[in] msg A picojson object that will be sent to the observers to parse details. The message is constructed by the
        * concrete observers depending on what event just happened.
        */
        virtual void notify(picojson::object& msg, IEntity& ent);

    protected:
        std::vector<IEntityObserver*> observers;
};

}  // namespace csci3081


#endif  // ENTITY_SUBJECT_H
