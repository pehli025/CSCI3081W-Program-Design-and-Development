#include "json_helper.h"
#include "entity_subject.h"
#include <vector>
#include <iostream>

namespace csci3081 {

EntitySubject::EntitySubject() {} // Does nothing, wrapper for methods to implement

void EntitySubject::attach(IEntityObserver* observer) {
    this->observers.push_back(observer);
}

void EntitySubject::detach(IEntityObserver* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void EntitySubject::notify(picojson::object& msg, IEntity& ent) {
    for (IEntityObserver* obs : observers) {
        if (obs != nullptr) {
            picojson::value converted = JsonHelper::ConvertPicojsonObjectToValue(msg);
            obs->OnEvent(converted,ent);
        }
    }
}

}
