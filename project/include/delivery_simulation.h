/**
 * @file delivery_simulation.h
 */
#ifndef DELIVERY_SIMULATION_H_
#define DELIVERY_SIMULATION_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <EntityProject/facade/delivery_system.h>
#include "composite_factory.h"
#include <vector>
#include <string>

namespace csci3081 {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief This is the facade for the delivery system
 *
 * This class will delegate operations for the whole drone delivery system.
 * See the documentation for IDeliverySystem for more information.
 */

// DO NOT MODIFY (unless you really know what you are doing)
class DeliverySimulation : public IDeliverySystem {
 public:
  /**
   * @brief Constructor: Sets up the delivery simulation.
   */
  DeliverySimulation();

  /// Destructor: This should free any memory that your program uses.
  ~DeliverySimulation();

  /**
  Given the picojson::object val, this creates an entity.
  Based on the type of entity, there may be different fields.
  You can see the vals that will be passed in the project/web/scenes
    directory. Some of the fields are for the backend system and you don't
    need to worry about them. (for instance, mesh, rotation, offset, etc.)

  Some fields in val that you will need to create the entity correctly:

    type: string (could be "drone/customer/package")

    name: string

    position: array (contains [x_position, y_position, z_position])

    direction: array (contains [x, y, z])

    speed: float

    battery_capacity: float

  Doesn't add the entity to the simulation until it is passed in via AddEntity
  */
  IEntity* CreateEntity(const picojson::object& val);

  /**
   *  @brief This function adds a factory to the composite factory.
   *  @param[in] factory An IEntityFactory* of the factory we wish to add to the composite factory.
   */
  void AddFactory(IEntityFactory* factory);

  /**
   *  @brief This adds an entity to the simulation
   *  @param[in] entity An IEntity* of the entity we wish to add to the simulation.
   */
  void AddEntity(IEntity* entity);

  /**
  * @brief This function stores a reference to the IGraph.
  * The IGraph can be used to get the path a vehicle should take when making deliveries
  * @param[in] graph The IGraph object to store
  */
  void SetGraph(const IGraph* graph);

  /**
  * This function tells the simulation that the IEntity* package should be delivered
  * to the IEntity* dest (which is likely a customer). How the delivery takes place
  * is entirely dependent on how you design your code, but it should involve a drone
  * navigating to the package, picking it up, and then moving to the customer and
  * dropping the package.
  * @brief This function tells the simulation that the IEntity* package should be delivered to the
  * IEntity* dest (which is likely a customer).
  *
  * The function finds the drone with the lowest numbered scheduled deliveries and pushes it to the
  * drone's queue of deliveries. Prints a warning if the package passed in is not a pointer to a
  * package or if the destination passed in is not a customer.
  *
  * @param[in] package A pointer to an IEntity object describing the package we must deliver.
  * @param[in] dest A pointer to an IEntity object describing the entity we must deliver to.
  */
  void ScheduleDelivery(IEntity* package, IEntity* dest);

  /** Observer functions will not be used in iteration1 */
  void AddObserver(IEntityObserver* observer);

  /** Observer functions will not be used in iteration1 */
  void RemoveObserver(IEntityObserver* observer);

  /**
  * @brief Returns a vector of all the IEntity objects in the simulation.
  */
  const std::vector<IEntity*>& GetEntities() const;

  /**
  * @brief Advances time in the simulation to move all the entities.
  * @param[in] dt A float describing how much time should pass in the simulation.
  *
  * This function is used to advance time in the simulation. float dt refers to the
  * amount of time the update call should advance the simulation by. For instance if a drone
  * moves 1 unit of distance per unit of time, and Update is called with dt=.05, then the
  * drone should move 1 * .05 = .05 units of distance.
  *
  * Currently just checks all drones and moves them around. Packages are moved by the drones.
  */
  void Update(float dt);

  /**
   * @brief You do not need to worry about this function
   *
   * This function takes care of turning json into function calls of your system.
   * YOU DO NOT NEED TO IMPLEMENT THIS
   *it is already implemented in the delivery_simulation.cc we have provided.
   */
  void RunScript(const picojson::array& script, IEntitySystem* system) const;

private:
    std::vector<IEntity*> entities_;
    std::vector<IEntityObserver*> observers;
    CompositeFactory compFactory; // Initialized automatically with the default constructor
    const IGraph* graph;
};

}  // namespace csci3081


#endif  // DELIVERY_SIMULATION_H_
