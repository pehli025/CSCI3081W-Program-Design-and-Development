/**
 *@file entity_base.h
 */
#ifndef ENTITY_BASE_H_
#define ENTITY_BASE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <EntityProject/facade/delivery_system.h>
#include "entity_subject.h"

namespace csci3081 {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * The base class for creating entities.
 *
 * This class can be used as the base for all entities in the delivery system.
 * The overall design is up to you (the student), but all entities must implement
 * the IEntity interface.
 *
 * See the documentation for IEntity for more information
 */
class EntityBase : public IEntity, public EntitySubject {
    public:
    /**
    * Default constructor: This is needed to instantiate vehicles without specifying any information. This is primarily used 
    * by decorators since they do not have any picojson details associated with them and instead use the object that was created
    * with a picojson object already.
    */
        EntityBase();
        EntityBase(const picojson::object& val);
        virtual int GetId() const override;
        virtual const std::string& GetName() override;
        virtual const std::string& GetType() const; // Not overrided yet
        virtual const std::vector<float>& GetPosition() const override;
        virtual const std::vector<float>& GetDirection() const override;
        virtual float GetRadius() const override;
        virtual int GetVersion() const override; // Not used in iteration 1
        virtual bool IsDynamic() const override;
        virtual void setPosition(std::vector<float> p); // Nothing to override
        virtual void setDynamic(bool status);
        virtual void setDirection(std::vector<float> d);

        /**
        * @brief Resets the static counter variable, eCount which counts the number of entities that have been created.
        * Used for testing because IDs are dependent on the number of entities created.
        */
        static void resetEntityCount();
        virtual ~EntityBase() {}
        virtual const picojson::object& GetDetails() override;

    protected:
        picojson::object details_;
        int id;
        std::string name;
        std::string type;
        std::vector<float> position;
        std::vector<float> direction;
        float radius;
        int version;
        bool dynamic;

        static int eCount;
};
}  // namespace csci3081


#endif  // ENTITY_BASE_H_
