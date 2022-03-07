#include "battery.h"

namespace csci3081 {

Battery::Battery(float mCharge) {
    maxCharge = mCharge;
    remainingLife = maxCharge;
    noCharge = false;
}

void Battery::useBattery(float timeUsed) {
    if (noCharge || remainingLife <= 0) { return; }
    remainingLife -= timeUsed;
    if (remainingLife <= 0) { noCharge = true; }
}

float Battery::getMaxLife() { return maxCharge; }
float Battery::getRemLife() { return remainingLife; }

}
