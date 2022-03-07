#include "dvec_decorator.h"

namespace csci3081 {

DVecDecorator::DVecDecorator(DeliveryVehicle* dvec) : decoratedVehicle(dvec) {
    numUpdates = 0;
}

void DVecDecorator::moveTo(std::vector<float> floatDest, float dt) {
    decoratedVehicle->moveTo(floatDest, dt);
}

void DVecDecorator::update(float dt) {
    decoratedVehicle->update(dt);
}

// Overriding methods in Delivery Vehicle
bool DVecDecorator::holdingPackage() { return decoratedVehicle->holdingPackage(); }
bool DVecDecorator::getMoving() { return decoratedVehicle->getMoving(); }
std::queue<std::vector<IEntity*>> DVecDecorator::getQueuedDeliveries() {
  return decoratedVehicle->getQueuedDeliveries();
}
float DVecDecorator::GetSpeed() const { return decoratedVehicle->GetSpeed(); }
Package* DVecDecorator::getPackage() const { return decoratedVehicle->getPackage(); }
Battery& DVecDecorator::getBattery() { return decoratedVehicle->getBattery(); }
bool DVecDecorator::getRescheduleFlag() const { return decoratedVehicle->getRescheduleFlag(); }
std::vector<std::vector<float>> DVecDecorator::getPackageToCust() {
  return decoratedVehicle->getPackageToCust();
}
std::vector<std::vector<float>> DVecDecorator::getToPackage() {
  return decoratedVehicle->getToPackage();
}
std::vector<std::vector<float>> DVecDecorator::getPath() { return decoratedVehicle->getPath(); }
void DVecDecorator::enqueueDelivery(IEntity* pack, IEntity* dest) {
  decoratedVehicle->enqueueDelivery(pack, dest);
}
void DVecDecorator::updateReschedule() { decoratedVehicle->updateReschedule(); }
void DVecDecorator::setPackage(IEntity* package) { decoratedVehicle->setPackage(package); }
void DVecDecorator::setPackageToCust(std::vector<std::vector<float>> p) {
  decoratedVehicle->setPackageToCust(p);
}
void DVecDecorator::setToPackage(std::vector<std::vector<float>> p) {
  decoratedVehicle->setToPackage(p);
}
void DVecDecorator::setPath(std::vector<std::vector<float>> p) {decoratedVehicle->setPath(p); }
void DVecDecorator::notifyIdle() { decoratedVehicle->notifyIdle(); }
void DVecDecorator::notifyToPackage() { decoratedVehicle->notifyToPackage(); }
void DVecDecorator::notifyToCustomer() { decoratedVehicle->notifyToCustomer(); }
void DVecDecorator::startNewDelivery(const IGraph* graph) {
  decoratedVehicle->startNewDelivery(graph);
}

// Overriding methods in EntityBase
int DVecDecorator::GetId() const { return decoratedVehicle->GetId(); }
const std::string& DVecDecorator::GetName() { return decoratedVehicle->GetName(); }
const std::string& DVecDecorator::GetType() const { return decoratedVehicle->GetType(); }
const std::vector<float>& DVecDecorator::GetPosition() const {
  return decoratedVehicle->GetPosition();
}
const std::vector<float>& DVecDecorator::GetDirection() const {
  return decoratedVehicle->GetDirection();
}
float DVecDecorator::GetRadius() const { return decoratedVehicle->GetRadius(); }
int DVecDecorator::GetVersion() const { return decoratedVehicle->GetVersion(); }
bool DVecDecorator::IsDynamic() const { return decoratedVehicle->IsDynamic(); }
void DVecDecorator::setPosition(std::vector<float> p) { decoratedVehicle->setPosition(p); }
void DVecDecorator::setDynamic(bool status) { decoratedVehicle->setDynamic(status); }
void DVecDecorator::setDirection(std::vector<float> p) { decoratedVehicle->setDirection(p); }
const picojson::object& DVecDecorator::GetDetails() { return decoratedVehicle->GetDetails(); }

// Overriding methods in EntitySubject
void DVecDecorator::attach(IEntityObserver* observer) { decoratedVehicle->attach(observer); }
void DVecDecorator::detach(IEntityObserver* observer) { decoratedVehicle->detach(observer); }
void DVecDecorator::notify(picojson::object& msg, IEntity& ent) {
  decoratedVehicle->notify(msg, ent);
}
void DVecDecorator::setColor(std::string hexColor) { decoratedVehicle->setColor(hexColor); }

}
