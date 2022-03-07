#include "drone.h"
#include "json_helper.h"
#include "battery.h"
#include "package.h"
#include "vector_operators.h"
#include "vector3d.h"
#include "vector2d.h"
#include <iostream>
#include "strategy.h"
#include "Parabolic.h"
#include "Smart.h"
#include "Beeline.h"
#include <iomanip> // For converting ints to hex strings

namespace csci3081 {
  
DeliveryVehicle::DeliveryVehicle() : EntityBase() {}

DeliveryVehicle::DeliveryVehicle(const picojson::object& val) : EntityBase(val) {
    if (JsonHelper::ContainsKey(val,"speed")) { this->speed = JsonHelper::GetDouble(val,"speed"); }
    else { this->speed = 10; }
    if (JsonHelper::ContainsKey(val,"path")) {
        if (JsonHelper::GetString(val,"path").compare("parabolic") == 0) {
            this->strategy = new Parabolic();
        }
        else if (JsonHelper::GetString(val,"path").compare("beeline") == 0) {
            this->strategy = new Beeline();
        }
        else if (JsonHelper::GetString(val,"path").compare("smart") == 0) {
            this->strategy = new Smart();
        }
        else {
            std::cout << "Json object had path value but not recognized" << std::endl;
        }
    }
    else {
        this->strategy = new Smart(); // Default to a smart path
    }
    isMoving = false;
    dynamic = false;
    hasPackage = false;
    package = nullptr;
    rescheduleFlag = false;
    destIndex = 0;
    if(JsonHelper::ContainsKey(val, "battery_capacity")){
        battery = Battery(JsonHelper::GetDouble(val, "battery_capacity")); // init battery
    }
}

bool DeliveryVehicle::holdingPackage() { return hasPackage; }

bool DeliveryVehicle::getMoving() { return isMoving;}

float DeliveryVehicle::GetSpeed() const { return this->speed; }

Package* DeliveryVehicle::getPackage() const { return this->package; }

bool DeliveryVehicle::getRescheduleFlag() const { return this->rescheduleFlag; }

Battery& DeliveryVehicle::getBattery() { return this->battery; }

std::vector<std::vector<float>> DeliveryVehicle::getPackageToCust() { return this->packageToCust; }

std::vector<std::vector<float>> DeliveryVehicle::getToPackage() { return this->toPackage; }

std::vector<std::vector<float>> DeliveryVehicle::getPath() { return this->path; }

void DeliveryVehicle::setPackage(IEntity* package) {
    if(package == nullptr) { return; }
    EntityBase* castPackage = static_cast<EntityBase*>(package);
    if (castPackage->GetType().compare("package") != 0) {
        std::cout << "Warning entity type of package set is not package, setting anyway" << std::endl;
    }
    this->package = static_cast<Package*>(package);
    destIndex = 0;
}

void DeliveryVehicle::setPackageToCust(std::vector<std::vector<float>> p) {
    this->packageToCust = p;
}

void DeliveryVehicle::setToPackage(std::vector<std::vector<float>> p) {
    this->toPackage = p;
}

void DeliveryVehicle::setPath(std::vector<std::vector<float>> p) {
    this->path = p;
}

void DeliveryVehicle::enqueueDelivery(IEntity* pack, IEntity* dest) {
    std::vector<IEntity*> packageAndDest{pack,dest};
    this->queuedDeliveries.push(packageAndDest);
}

void DeliveryVehicle::startNewDelivery(const IGraph* graph) {
    if (this->queuedDeliveries.size() == 0) {
        return;
    }
    if (!holdingPackage() && this->package == nullptr) { // Saftey check to make sure we don't pop deliveries accidentally
        std::vector<IEntity*> delivery = this->queuedDeliveries.front();
        this->queuedDeliveries.pop();
        this->setDynamic(true);
        this->isMoving = true;
        Package* castPackage = static_cast<Package*>(delivery.at(0));
        Customer* castCustomer = static_cast<Customer*>(delivery.at(1));
        this->setPackage(castPackage);
        castPackage->setCustomer(castCustomer);
        std::vector<std::vector<float>> toPackage = strategy->GetPath(graph, this->GetPosition(), castPackage->GetPosition());
        std::vector<std::vector<float>> packageToCust = strategy->GetPath(graph, package->GetPosition(), castCustomer->GetPosition());
        this->toPackage = toPackage; // Copy to package into member variable
        this->notifyToPackage(); // Notify path to package
        this->packageToCust = packageToCust; // Copy package to cust into member variable
        toPackage.insert(toPackage.end(),packageToCust.begin(),packageToCust.end()); // Concatenate paths
        this->setPath(toPackage); // Set whole path member variable as the concatenation.
        castPackage->notifyScheduled();
    }
}

void DeliveryVehicle::updateReschedule() {
    // Do not update package position leave floating.
    if (hasPackage) {
        getPackage()->setPosition(this->position);
        getPackage()->setDirection(this->direction);
    }
    rescheduleFlag = false;
    isMoving = false;
    hasPackage = false;
    path.clear();
    packageToCust.clear();
    toPackage.clear();
    destIndex = 0;
    // dynamic = false;
    this->notifyIdle();
    package = nullptr;
}

std::queue<std::vector<IEntity*>> DeliveryVehicle::getQueuedDeliveries() {
    return this->queuedDeliveries;
}

void DeliveryVehicle::notifyIdle() {
    picojson::object message = JsonHelper::CreateJsonObject();
    JsonHelper::AddStringToJsonObject(message, "type", "notify");
    JsonHelper::AddStringToJsonObject(message, "value", "idle");
    IEntity* castVehicle = static_cast<IEntity*>(this);
    notify(message,*castVehicle);
}

void DeliveryVehicle::notifyToCustomer() {
    picojson::object message = JsonHelper::CreateJsonObject();
    JsonHelper::AddStringToJsonObject(message, "type", "notify");
    JsonHelper::AddStringToJsonObject(message, "value", "moving");
    JsonHelper::AddStdVectorVectorFloatToJsonObject(message, "path", getPackageToCust());
    IEntity* castVehicle = static_cast<IEntity*>(this);
    notify(message,*castVehicle);

}

void DeliveryVehicle::notifyToPackage() {
    picojson::object message = JsonHelper::CreateJsonObject();
    JsonHelper::AddStringToJsonObject(message, "type", "notify");
    JsonHelper::AddStringToJsonObject(message, "value", "moving");
    JsonHelper::AddStdVectorVectorFloatToJsonObject(message, "path", getToPackage());
    IEntity* castVehicle = static_cast<IEntity*>(this);
    notify(message,*castVehicle);
}

void DeliveryVehicle::setColor(std::string hexColor) {
    JsonHelper::AddStringToJsonObject(details_, "color", hexColor);
}

}
