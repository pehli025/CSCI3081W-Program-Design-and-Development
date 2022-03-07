/*! \mainpage CSCI 3081 Delivery Simulation Project - Iteration 3
* Authors: Team 25 - Gavin Huang, Evan Koo, Jessica Moore, Josh Pehling
*
* \section intro_sec Introduction
* In this project, we are creating a simulation environment that represents a delivery
system. This document outlines the design implementation we have chosen in order to create the simulation. To retrieve the simulation, follow the steps given in the section \ref obtain_sec. To run the simulation, follow the steps given in the section \ref run_sec.

For further information about the design of the simulation, please read section \ref design_sec. Additionally, for insight about the design patterns used, please visit the sections \ref factory_sec, \ref observer_sec, \ref route_sec, and \ref feature_sec.

Visit \ref team_sec for team meeting information.
*
* \section obtain_sec Obtaining the Simulation
* To obtain the simulation, navigate to your desired directory and clone the Team 25 Git repository by using the command `git clone https://github.umn.edu/umn-csci-3081-s21/repo-iter3-01-25.git`. Then navigate to the project directory and run the `make` command to compile the simulation. The simulation is now ready to be run.
*
* \section run_sec Running the Simulation
* To run the simulation, you will need to have docker installed on your device. First, build the
docker image by running the command `bin/build-env.sh` in the base repository directory. Next, run the docker image with `bin/run-env.sh`. Once inside the docker image, navigate to the `project` directory and run `make`. Then run the web server with `./bin/run.sh`. Once the web server gives the output "Starting server...", you can open any web browser and browse to `http://127.0.0.1:8081/.` Here, you can view the simulation.
*
* \section test_sec Running the Unit Tests
* To run unit tests, navigate to the project directory and run `make test`. Then run `./bin/test.sh`.
*
* \section doc_sec Generating Documentation
* To generate documentation, first build the project with `make` in the `project` directory.
Next, navigate to `project/docs` and run `doxygen Doxyfile`. Then open `project/docs/html/index.html` in your web browser to view the documentation.
*
* \section design_sec Overall Design Implementation
* The package delivery system is made up of drones and robots that deliver packages to customers, and is generally structured in the following way: A JSON file feeds information to the simulation about how drones, robots, customers, and packages should be constructed and added to the simulation, then the simulation schedules and carries out deliveries. In order to create the package delivery simulation environment, a facade design pattern was used to hide the complexities of the simulation. Our drones, robots, customers, and packages are represented by their parent interface \link entityproject::IEntity IEntity \endlink - we will refer to these as entities. Entities are created based on the specifications given by the JSON file in factories. The \ref uml_sec at the bottom of this page shows how the entities, factories, and other simulation factors interact.

The \link csci3081::DeliverySimulation DeliverySimulation \endlink class acts as our facade, and is responsible for creating and updating entities in the simulation. The function RunScript shown in the UML diagram parses the instructions given by the input JSON file, which gives information about how and when entities should be created, added to the simulation, and updated within the simulation. When an entity should be created, the facade calls upon the factories to create the correct entity. When an entity should be added to the simulation, the facade adds it. When a package should be scheduled to be delivered, the facade assigns a drone to deliver it. As the simulation moves forward in time, the facade updates each entity so they behave appropriately within the simulation. In order to do this, the facade relies upon entities inheriting from a generalized interface.

As shown in the UML diagram, \link csci3081::Drone Drone\endlink, \link csci3081::Robot Robot\endlink, \link csci3081::Customer Customer\endlink, and \link csci3081::Package Package\endlink inherit from the class \link csci3081::EntityBase EntityBase\endlink, which is our base implementation of the IEntity interface. EntityBase implements all the virtual functions given in IEntity on a high level. This reduces redundancy and simplifies the individual implementations of Drone, Robot, Customer, and Package by instantiating their shared functionality upfront. From there, the inherited classes capture their individual purposes by instantiating specialized member variables and functions. While these entities are similar in that they share existence within the simulation, they have very different functions. A drone or robot navigates to the package then brings it to the customer, the package is inanimate until the drone or robot picks it up, and the customer stays put and excitedly waits for its package. Using the IEntity interface allows the facade to update all entities simultaneously while hiding the differences in entity functionality.

Drones are able to move in a number of different ways within the simulation. It can reach the package by moving directly towards it (above the landscape) or through the shortest path on the street. For more information about the implementation of these different kinds of routes, please visit the section \ref route_sec.

As previously stated, all entities are created by factories. For our simulation, we implemented an abstract composite factory pattern. An in-depth discussion of the abstract composite factory pattern, as well as comparison of different factory pattern choices, can be found in the section \ref factory_sec.

Our simulation gives notifications to the user when there is a significant update to the state of an entity. Users are notified when a package is scheduled, picked up, and delivered, or when a drone or robot stops/starts moving. These notifications are created by observers, through our use of the observer design pattern. An overview of our implementation of observers can be found in the section \ref observer_sec.

In iteration 3, we implemented a new feature that indicates the battery life of vehicles (drones and robots) in the simulation by their color. As the battery life of the vehicle depletes, it changes from green to red. A great scene in which to see the color change functionality is "web/scenes/all_features_no_recharge_station.json". For more information about how we implemented this feature, please refer to \ref feature_sec.
*
* \section factory_sec Factory Design Pattern Discussion
* Our simulation implements the composite factory pattern in the \link csci3081::CompositeFactory CompositeFactory \endlink class. This section explains the advantages and drawbacks of different factory design patterns, and why the composite factory pattern was chosen.
*
* \subsection simple_factory Simple/Concrete Factory Pattern Overview
* The simple factory pattern is a design pattern for pushing the creation of objects to a single class. In that class, a large if-else chain creates objects depending on the type of data that is passed in as seen below:

\image html SimpleFactory.png width=50% height=50%

If we were to use the simple factory pattern in our package delivery simulation, the SimpleEntityFactory would just look at all the picojson objects that are passed in and create different entities based on the type in the picojson object.

One advantage of using the simple factory pattern over just having the giant if-else chain in the delivery simulation class is that it keeps the object creation and the object's usage separate. This allows us to reuse code that we've written before for other projects. For example, if a military strategist wanted to use our drone, customer, and package entities, we could give them the SimpleEntityFactory to create entities in their own simulation, and we would not have to copy the code that we've written in DeliverySimulation over to the MilitarySimulation (see below)
\image html MilitarySimpleFactory.png width=50% height=50%
In addition to not having to rewrite code, we can also change code in one place and all the clients (DeliverySimulation and MilitarySimulation) will receive the changes. If we notice a bug in our simple factory and need to fix it, we only have to change the simple factory class instead of changing any code that is written in both MilitarySimulation and DeliverySimulation. The simple factory pattern is a straightforward way to modularize code so that object creation and object usage are separate.

One disadvantage of using the simple factory pattern is that the simple factory pattern still violates the open-closed principle stating that classes should allow for its behavior to be extended without changing the source code (however the open-closed principle violates the open-closed principle less than a giant if-else chain in DeliverySimulation). Suppose we wanted to add a new class, Tank, that implements the IEntity interface. We would write the class, but then we would need to chain another if-else to the CreateEntity() method in the simple factory as seen below.

\image html TankSimpleFactory.png width=50% height=50%

While the simple factory method is good for when the type of objects you wish to instantiate remains the same, it becomes unwieldly when you want to extend the use of it and allow the factory to instantiate different types. It is also unwieldly to use if you wish to no longer support the creation of some type of object. You would delete that object's class, and then you would have to go into the CreateEntity() method and delete the if-else branch that creates the deleted class.
*
* \subsection abstract_factory Abstract Factory Pattern Overview
* The abstract factory pattern is designed to create objects that belong to a specific group of objects. It has an abstract factory interface that all extending factories must implement. By overriding these methods, the inheriting factories can define their own group of objects to create. For example, if we had two types of drones, DeliveryDrone and MilitaryDrone, they would be composed of different parts. The DeliveryDrone would have a standard lithium ion battery to power it and a standard propulsion system to help it fly. On the other hand, the MilitaryDrone would have a solar powered battery for power and a rocket propulsion system to help it fly.

\image html AbstractFactory.png width=50% height=50%

Now whenever we have a DeliveryDrone made up of specific delivery drone parts, we can go out to the DeliveryDroneFactory to get parts specialized for a delivery drone.  If we wanted to create a MilitaryDrone, we can call the MilitaryDroneFactory instead. The abstract factory pattern's strength comes from its ability to group related objects together for object creation.

The downside of using the abstract factory pattern is that adding a new group of objects to the factory can be costly. Suppose that our drones now needed a camera. We would have to create an abstract Camera class and create concrete implementations of the Camera class. After that, we would have to redefine the abstract DroneFactory interface in order to support producing cameras. Then all of our extending classes (MilitaryDroneFactory and DeliveryDroneFactory) now have to be changed to support a Camera.  Although the abstract factory pattern ensures consistency in the objects being created, it is costly to add new groups or types of objects to the abstract factory.
*
* \subsection composite_factory Composite Factory Pattern Overview
* The composite factory pattern was used during this project iteration in order to create objects. The composite factory pattern treats factories like nodes in a tree. A factory with other factories in it will recursively call the child factory's CreateEntity() method, while any leaf factory will try to create a new object with its CreateEntity() method. In our specific implementation, the CompositeFactory was a factory holding the other 3 factories, DroneFactory, PackageFactory and CustomerFactory. Since CompositeFactory was a factory holding other types of factories, it would call CreateEntity() on its children whereas since Drone/Package/CustomerFactory were leaf factories (not having any children) they would try to instantiate objects directly.

\image html CompositeFactory.png width=50% height=50%

One advantage of handling object creation like this is that the code is easily extendable. Due to polymorphism, the program knows which class's CreateEntity method is being called so the program can recursively traverse across the tree of factories calling their CreateEntity() methods. Adding a new entity like a Tank will not change the code much in the CompositeFactory class, we only need to add the TankFactory to the list of factories inside the composite factory to allow for Tank object creation. We also need to implement the TankFactory's CreateEntity() method, but that does not violate the open-closed principle.

\image html CompositeModified.png width=50% height=50%

One downside of the composite factory pattern is that the composite factory can hold too many things. The composite factory's "factories" member variable is only restricted to  contain IEntityFactory* (pointers of IEntityFactory). This means that it can hold objects of type CompositeFactory as well so we could have a composite factory holding a composite factory. We could also have multiple drone factories ending up in our vector of factories. Both of these outcomes could lead to undesired results. Or maybe in the future when we design a new type of IEntityFactory that should never be a child of CompositeFactory, it is still possible to add that new type to CompositeFactory because the program sees that they're both IEntityFactory objects so it is up to us, the programmers, to make sure that these collisions never happen with the composite factories.
*
* \section observer_sec Observer Pattern Discussion
* Within the simulation, observers are used to notify the user when the state of a scheduled delivery has been updated. Our implementation of the observer pattern facilitates these notifications. The observer pattern describes the relationship between a subject object (the object being observed for updates) and an observer object (the object that looks for updates in the subject and creates notifications about them). The below diagram illustrates how these objects interact: \link entity_project::WebSceneViewer WebSceneViewer\endlink and \link entity_project::EntityConsoleLogger EntityConsoleLogger\endlink are the concrete observers, and the \link csci3081::EntitySubject EntitySubject\endlink is the subject.

\image html observer_uml.png width=50% height=50%

In order to implement the observer pattern, we created the EntitySubject class, which is a new parent class of entities made to perform the observing functionality. As entities enter the simulation, the WebSceneViewer and the EntityConsoleLogger are attached to the entity. Then, when the state of the entity changes (for example, if a package is picked up), the entity notifies the observer. The observer collects the information given by the entity and notifies the user appropriately; WebSceneViewer displays notifications for users in the simulation viewer, while EntityConsoleLogger logs notifications in the console. This system of notifications is very useful for users who would like to track the exact status of deliveries within the simulation.
*
* \section route_sec Designing and Implementing the Routes
* The simulation accommodates several different types of routes that drones can take to deliver packages. In order to create these routes, we implemented the strategy design pattern. The strategy pattern encapsulates an action into a class hierarchy of strategies, so that objects who need to perform that action hold a pointer to the type of strategy they need. The sample UML diagram below shows how this pattern is implemented.

\image html strategy_uml.png width=50% height=50%

We applied this to our simulation by creating an interface, \link csci3081::IStrategy IStrategy \endlink, representing a strategy to calculate routes. It has one function, \link csci3081::IStrategy::GetPath() GetPath\endlink, which is individually defined by each route type. There are 3 strategies that can be used: smart, beeline, and parabolic routes. The implementations of these routes inherit from this interface. Each vehicle (which needs to calculate routes in order to deliver packages) contains a pointer to an IStrategy. This allows for us to delegate the type of strategy a vehicle should use to calculate routes dynamically, within the constructor of the objects.

This design pattern was a challenge for us to wrap our heads around. <a href="https://www.geeksforgeeks.org/strategy-pattern-set-2/">GeeksForGeeks</a> and <a href="https://sourcemaking.com/design_patterns/strategy/cpp/1">SourceMaking</a> both provide fleshed-out examples of the strategy pattern that we found incredibly useful while creating this design.
*
* \subsection smart_route Smart Route Overview
* A smart route uses the A* shortest-path algorithm to determine the fastest on-ground route to from the starting point to the destination. In the \link csci3081::Smart Smart\endlink class, the GetPath function uses the IGraph class given by the CSCI 3081 staff. Given a starting and ending point, the \link entity_project::IGraph::GetPath() IGraph GetPath \endlink function generates a list of positions (in order) that the vehicle should go to. The drone then follows the route to its destination, as illustrated by the following simulation screenshot.

\image html smart.png "Robot following a smart route to a customer." width=75%

To understand how the smart route is created, it’s helpful to think of the simulation in terms of nodes on a graph. Street intersections are graph nodes, and streets between each intersection are edges between the nodes. These nodes and edges go around the landscape of the simulation so that the vehicle does not collide with any buildings while on the ground. Based on these nodes, and nodes that represent the starting and ending point, the IGraph class can create a smart route. For a better understanding of how the A* algorithm finds the shortest path, <a href="https://www.geeksforgeeks.org/a-search-algorithm/">GeeksForGeeks has a great article explaining it</a>.
*
* \subsection beeline_route Beeline Route Overview
* A beeline route is just as simple as it sounds: the drone rises above the buildings, moves directly to its destination, then lowers down to the intended point on the ground. <a href="https://piazza.com/class/kjuk3ynsg1f6os?cid=186">This Piazza post</a> was a great source of insight for us to understand how this should work. The straightforward beeline motion is shown below.

\image html beeline.png "Drone following a beeline route to a customer." width=75%

The \link csci3081::Beeline Beeline \endlink class was created to represent this route. Its GetPath function generates a list of positions in a line above the landscape for the drone to follow.
*
* \subsection parabolic_route Parabolic Route Overview
* A parabolic route extends above the simulation landscape in a parabolic fashion - this was one of the hardest pieces of the simulation to implement, as the math is a bit complicated. We created the \link csci3081::Parabolic Parabolic \endlink class to implement the GetPath function for this route, which generates a list of positions in an arc shape. The drone should move as though it had been launched into projectile motion, illustrated below in a diagram from <a href="https://flexbooks.ck12.org/cbook/ck-12-physics-flexbook-2.0/section/4.3/primary/lesson/projectile-motion-for-an-object-launched-at-an-angle-phys">FlexBooks</a>.

\image html parabolic.png "Diagram of parabolic/projectile motion." width=30%

When a drone takes this route, it moves in an arc from its starting point to its destination. Its x and z positions move directly to the destination (just like a beeline route), and its y position follows a quadratic curve to create the arc above the landscape. To more thoroughly understand how to mathematically implement the parabolic route, <a href="https://canvas.umn.edu/courses/217398/files/20535292/download">this document provided by the 3081 staff</a> is an excellent resource.
*
* \section feature_sec Visual Battery Indicators on Vehicles (NEW)
* The additional feature that we selected to include in our simulation was a visual indicator of battery level for vehicles (drones & robots). A great scene in which to see the color change functionality is "web/scenes/all_features_no_recharge_station.json". All vehicles are equipped with a color-changing device to display battery level. When the simulation begins and all vehicles start at 100%, they are green; when they run out of battery, they are red. As they move and therefore deplete battery life, the color shifts closer to red. This feature also works hand-in-hand with our simulation observers - as color is updated, users are notified.

To implement this feature, we employed the decorator design pattern. This pattern allows us to add ornamental functionality to components at runtime by creating a “decorator” class, which wraps another component so it can dynamically add onto the original functionality. When designing our implementation, the biggest challenge for us was wrapping our heads around how the decorator is able to execute the new functionality in addition to the original - we’re used to working with patterns that replace the original function. <a href="https://www.baeldung.com/java-decorator-pattern">Baeldung has a great diagram</a> that was really helpful for us as we worked through how the decorator should interact with the component. Our simulation utilizes this concept by wrapping our vehicle classes in the \link csci3081::VehicleColorDecorator VehicleColorDecorator\endlink class. The VehicleColorDecorator inherits from the \link csci3081::DVecDecorator DVecDecorator\endlink class; this class has been created, instead of a single concrete decorator, so that in future development we would be able to add more decorators for other additional functions. We have included a diagram below to illustrate how this wrapping occurs.

\image html decorator_uml.png width=50% height=50%

After we create a vehicle in our delivery simulation facade, a VehicleColorDecorator is also created that references the vehicle. Therefore, when a vehicle needs to be updated, the \link csci3081::VehicleColorDecorator::update() Update\endlink function is called for the VehicleColorDecorator instance that wraps the vehicle. This executes the vehicle’s normal updating functionality, as well as the VehicleColorDecorator’s color changing effect.

The decorator pattern was a great choice for the battery indicator feature because it’s an additional function that not all vehicles necessarily need to have. If we were to make a better drone that doesn’t use a battery, then we would want to be able to separate the color-changing battery indicator from the vehicle. This pattern’s greatest advantage is that it provides an alternative to creating new subclasses; it’s a very elegant way to add optional capabilities to an already existing set of classes. Not all features have to be immediately accounted for in the system, because the decorator pattern allows them to be added later. It’s also efficient because an object’s capabilities can be adjusted at runtime. On the other hand, this pattern can be cumbersome because instantiating many instances of the component will end up requiring wrapping - sometimes several times. This requires wrapping decorators in other decorators, which means passing the control through multiple layers of classes before reaching the component class, which can end up being very redundant. If a feature is not intended to be optional, the decorator design pattern shouldn’t be used.

When designing our battery color indicator, we also considered just integrating the battery indicator functionality directly into the delivery vehicle classes. This would have worked perfectly fine, but it would have cluttered the code in the delivery vehicle class redundancy and inhibited future development by making the delivery vehicle class responsible for too many functions. The decorator pattern gives a lot more flexibility to add more features in the future. To do this, we would just have to create another class that extends from the DVecDecorator class and we would be able to reuse the code from DVecDecorator. Another design pattern that we considered was the strategy pattern. This could have been a good alternate pattern because it also would sped up future development by delegating coloring the drone and adding additional features to external strategy classes instead of writing it directly into the delivery vehicle class. However, using a strategy design is more cumbersome because it requires adding some member variable to the delivery vehicle to keep track of which strategy to add functionality to the delivery vehicle. The decorator seemed more appropriate as we could layer on more functionalities to a delivery vehicle while the strategy pattern is good for when you want to execute one algorithm. If we wanted to add more functionality to the drone's movement, say have it rotate while it's moving and change color, we would have to implement a strategy that does both. With the decorator pattern we can layer a color change functionality onto the vehicle and then layer a spinning functionality onto the vehicle without having to write a strategy that does both.
*
* \section team_sec Team Documentation
*
* \subsection iter3_team Iteration 3 Team Documentation (NEW)
* We had 3 meetings to convene on how we would split the work.
- <b>April 21st, 2021:</b> We began iteration 3 by meeting on Discord to decide which feature we wanted to implement. After deciding on the visual indicator feature, we discussed the decorator design pattern to make sure we all understood it. Evan was assigned to building our iteration 3 repository, and Josh, Gavin, and Jess were assigned to working together to design & update the UML diagram.
- <b>April 25th, 2021:</b> In our next meeting, we delegated tasks for this iteration. Evan was assigned to implement the indicator feature. Josh and Gavin were assigned creating unit tests for the indicator feature and tests for our remaining classes. Jess was assigned doxygen mainpage documentation.
- <b>April 29th, 2021:</b> In our final meeting of the semester on Zoom, we ironed out the final details of what we needed to accomplish. Evan was assigned final debugging. Josh & Gavin handled the final google tests that we needed to write, and Jess did last-minute style changes.
*
* \subsection iter2_team Iteration 2 Team Documentation
* We had 3 meetings to convene on how we would split the work.
- <b>March 25th, 2021:</b> Our iteration 2 kickoff meeting on Discord was spent going over what was required for deliverable 1 and creating our group repository. Josh was assigned the task of creating our group repository. Evan and Gavin worked together later to complete deliverable 1, where Gavin designed the Robot class inheritance and Evan worked on the Robot and Observer implementations. Because Jess was unable to attend the meeting, she was not assigned a task.
- <b>April 13th, 2021:</b> In our second meeting on Discord, we generally split up tasks for deliverable 2. Josh and Gavin were assigned priority one, fixing our Google tests from the last iteration, as all of our Google tests needed quite a bit of work. Evan had already completed priority two as extra credit in the previous iteration, so he was assigned to refine that code and priority 3, drone routes. Jess was assigned the Doxygen mainpage documentation.
- <b>April 19th, 2021:</b> We used our final iteration 2 meeting on Discord to go over our progress and assign all remaining tasks. Josh, Gavin, and Evan split the remaining Google tests and submitted final GitHub issues. Jess was assigned to ensuring google style compliance, fixing doxygen headers, and documenting code.
*
* \subsection role_team Roles of Team Members
* The roles of each team member were as follows:
- <b>Development Lead:</b> Evan Koo
- <b>Scheduler:</b> Josh Pehling
- <b>Reporter:</b> Jess Moore
- <b>Project Manager:</b> Gavin Huang
*
* \section uml_sec UML Diagram
* \image html uml.png "Delivery Simulation UML Diagram" width=100%
*/
