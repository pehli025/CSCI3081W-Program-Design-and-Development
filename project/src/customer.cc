#include "entity_base.h"
#include "json_helper.h"
#include "customer.h"
#include "vector3d.h"
#include "vector2d.h"

namespace csci3081 {

Customer::Customer(const picojson::object& val) : EntityBase(val) {
    dest = Vector3D(GetPosition().at(0),GetPosition().at(1),GetPosition().at(2));
    dynamic = false;
    type = "customer";
}

Vector3D Customer::getDest() const { return this->dest; }

}
