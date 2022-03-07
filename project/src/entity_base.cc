#include "entity_base.h"
#include "json_helper.h"
#include "vector3d.h"
#include "vector2d.h"
#include <vector>

namespace csci3081 {

int EntityBase::eCount = 0;
EntityBase::EntityBase() : EntitySubject() {}
EntityBase::EntityBase(const picojson::object& val) : EntitySubject() { // Parses picojson object and assigns values
    if (JsonHelper::ContainsKey(val,"name")) { this->name = JsonHelper::GetString(val, "name"); }
    else { this->name = "Default name"; }
    if (JsonHelper::ContainsKey(val,"position")) {
      this->position = JsonHelper::GetStdFloatVector(val,"position");
    } else { this->position = std::vector<float>(3,0); }
    if (JsonHelper::ContainsKey(val,"direction")) {
        std::vector<float> notNormalVec = JsonHelper::GetStdFloatVector(val,"direction");
        Vector3D normalizedDir = Vector3D::convertToVector3D(notNormalVec);
        normalizedDir.normalize();
        std::vector<float> normalVec = normalizedDir.toStdVector();
        this->direction = normalVec;
    } else { this->direction = std::vector<float>(3,0); }
    if (JsonHelper::ContainsKey(val,"radius")) {
      this->radius = JsonHelper::GetDouble(val,"radius");
    } else { this-> radius = 1; }
    details_ = val;
    version = 0;
    this->id = EntityBase::eCount; // Give unique id
    EntityBase::eCount += 1; // Increment to keep ids unique
}

int EntityBase::GetId() const { return id; }

const std::string& EntityBase::GetName() { return name; }

const std::string& EntityBase::GetType() const { return type; }

const std::vector<float>& EntityBase::GetPosition() const { return position; }

const std::vector<float>& EntityBase::GetDirection() const { return direction; }

float EntityBase::GetRadius() const { return radius; }

int EntityBase::GetVersion() const { return 0; } // Not used in iteration 1 or 2

bool EntityBase::IsDynamic() const { return dynamic; }

const picojson::object& EntityBase::GetDetails() { return details_; }

void EntityBase::setPosition(std::vector<float> p) { position = p; }

void EntityBase::setDynamic(bool status) { this->dynamic = true; }

void EntityBase::setDirection(std::vector<float> d) { direction = d; }

void EntityBase::resetEntityCount() { EntityBase::eCount = 0; }

}
