/**
 * @file battery.h
 */
#ifndef BATTERY_H
#define BATTERY_H

/*******************************************************************************
 * Includes
 ******************************************************************************/
namespace csci3081 {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief This is a general battery class used by vehicles in the simulation.
 *
 */
class Battery {
    public:
        /**
        * Constructor: Sets up battery with a maximum charge defaulted to 10000.
        * Charge is in units of simulation time.
        * Battery's remaining life gets initialized to mCharge and noCharge is false.
        */
        Battery(float mCharge = 10000);

        /**
        * Uses the battery so the charge it has left decreases.
        * @return void. Just decreasing remainingLife member variable by timeUsed.
        */
        void useBattery(float timeUsed);

        /**
        * Gets the maximum charge the battery can hold.
        * @return float representing how much simulation time the battery can be used before it
        * dies when battery is at full charge.
        */
        float getMaxLife();

        /**
        * Gets the remaining charge the battery has.
        * @return float representing how much simulation time the battery can be used before it
                        dies.
        */
        float getRemLife();

    private:
        float maxCharge;        // Maximum charge
        float remainingLife;    // Battery life remaining
        bool noCharge;          // True = has charge left; False = no batterly life remaining

};

}  // namespace csci3081


#endif  // BATTERY_H
