#include "entity_base.h"
#include "json_helper.h"
#include "package.h"
#include "customer.h"
#include <iostream>

namespace csci3081 {

Package::Package(const picojson::object& val) : EntityBase(val) {
    type = "package";
    weight = 1; // Default, not used yet
    cust = nullptr;
}

Customer* Package::getCustomer() { return this->cust; }

void Package::setCustomer(Customer* c) {
    this->cust = c;
    dynamic = false;
    dest = Vector3D(c->GetPosition().at(0),c->GetPosition().at(1),c->GetPosition().at(2));
}

Vector3D Package::getDestination() { return this->dest; }

void Package::notifyScheduled() {
    picojson::object message = JsonHelper::CreateJsonObject();
    JsonHelper::AddStringToJsonObject(message,"type","notify");
    JsonHelper::AddStringToJsonObject(message,"value","scheduled");
    IEntity* castPackage = static_cast<IEntity*>(this);
    notify(message,*castPackage);
}

void Package::notifyEnRoute() {
    picojson::object message = JsonHelper::CreateJsonObject();
    JsonHelper::AddStringToJsonObject(message,"type","notify");
    JsonHelper::AddStringToJsonObject(message,"value","en route");
    IEntity* castPackage = static_cast<IEntity*>(this);
    notify(message,*castPackage);
}

void Package::notifyDelivered() {
    picojson::object message = JsonHelper::CreateJsonObject();
    JsonHelper::AddStringToJsonObject(message,"type","notify");
    JsonHelper::AddStringToJsonObject(message,"value","delivered");
    IEntity* castPackage = static_cast<IEntity*>(this);
    notify(message,*castPackage);
}

}
