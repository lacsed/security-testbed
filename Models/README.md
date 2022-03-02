## Process

In here we describe the implemented process.

![main_screen](https://user-images.githubusercontent.com/12836843/122487626-13e78380-cfb2-11eb-9f74-1fe73cbb9f6a.JPG)

The general idea is the production of batches of a liquid that is the product of reactions that happen at different temperatures inside a reactor.

The process start by the admission of three different liquids, after the Start button is pressed by an operator. It is assumed that the flow of each liquid is defined by a previous process and is out of scope in here. There are three on-off input valves, each one for a different liquid, that receive the same control action, that is, they open and close at the same time. Since the valves remain opened by the same amount of time, the proportion of each liquid in the tank is determined by their flow. 

The total amount of liquid in the tank, which has a cylindrical shape, is a controlled variable and it is measured by a level sensor. Once the level in the tank reaches the set point, the input valves are closed and the mixer is turned on. The mixer makes the mix more homogeneous and therefore diminishes the total time of the batch production.

After the mixer has started, a continuous-time PI controller makes the temperature reach a desired value and remain there for a predefined period of time. After that, the controller makes the temperature reach a second predefined value, which is lower than the previous one, for another amount of time. A heater installed in the tank makes it possible to increase the temperature. To allow the liquid to cool faster, there is a heat exchanger coupled in the tank. To make the liquid go through it, a pump is turned on. Once this part of the process is finished, the mixer and the pump stop and an output valve is opened. The valve will remain opened until the level sensor indicates that the tank has reached the low level. When this happens, the output valve is closed and the batch is finished. The system is now ready to produce another batch.

[Back](https://github.com/michelrodrigo/DES-control-system)

## Models
 
### Subsystems

We introduce the models of the subsystems. The models are an abstraction of the real systems and we only consider in here events that are relevant to the cordination between them. Transitions with a streak are labelled with controllable events, while the ones without a streak are labelled with uncontrollable ones. All the models have an uncontrollable reset event  $`rst`$. This event is useful to restore the sync between the subsystem and supervisors if something fails.

*Process*

The process automaton does not represent a specific subsystem. Instead, it represents the whole system and it is useful when modeling specifications that restrain the behavior of a subsystem to the beggining or to the end of a production cycle.

![process](https://user-images.githubusercontent.com/12836843/156402392-99313e49-8ec7-458c-805d-fe45e9d9bd43.jpg)

*Input valve*

![vin](https://user-images.githubusercontent.com/12836843/156402437-d80991ed-fe4d-4caa-acda-a229ef2596b6.jpg)

*Output valve*

![vout](https://user-images.githubusercontent.com/12836843/156402495-f9c936e2-a5e7-4660-a01b-99fcf1b86487.jpg)

*Mixer*

![mixer](https://user-images.githubusercontent.com/12836843/156402531-a00ed6e7-dcb8-45ff-9f44-d54f70201e94.jpg)

*Pump*

![pump](https://user-images.githubusercontent.com/12836843/156402558-1dc0f94c-0552-492d-bede-76c76ad3f0a1.jpg)

*Temperature Control*

![temp](https://user-images.githubusercontent.com/12836843/156402601-ad5af8ed-f961-4cf4-962b-f733af145759.jpg)


[Back](https://github.com/michelrodrigo/DES-control-system)

### Specifications and Supervisors

*Opening of the input valve*

The first specification (left) restricts the opening of the input valve to the beggining of the process. Event $`V_{in}^{open}`$ can only occur after the occurrence of event $`start`$. The associated supervisor (right) disables event $`V_{in}^{open}`$ at state 0.

![E4](https://user-images.githubusercontent.com/12836843/156402768-21f5d5c6-50e8-4295-be0b-e26336947f6b.jpg)

*Closure of the input valve*

The following specification (left) is related to the conditions in which the input valve can be closed.  Thus, event $`V_{in}^{close}`$ can only occur after at least one occurrence of event $`L_{H_1}`$. To achieve this behavior, the associated reduced supervisor (right) disables event $`V_{in}^{close}`$ at state 1.

![E2](https://user-images.githubusercontent.com/12836843/156251621-a105b1b1-9e3b-4f3d-a6ef-8a6487138843.jpg)

*Opening of the output valve*

The first specification (left) restricts the opening of the output valve to the end of the cooling process, which is represented by event $`cooled`$.. Thus $`V_{out}^{open}`$ can only occur after the occurrence of event $`cooled`$. The associated supervisor (right) disables event $`V_{out}^{open}`$ at state 0.

![E5](https://user-images.githubusercontent.com/12836843/156403486-ab91a945-c4cd-4158-96a8-cc9a9019d6d7.jpg)

*Closure of the output valve*

The next specification (left) is related to the conditions in which the output valve can be closed.  Thus, event $`V_{out}^{close}`$ can only occur after at least one occurrence of event $`L_{L_1}`$. To achieve this behavior, the associated reduced supervisor (right) disables event $`V_{out}^{close}`$ at state 1.

![E3](https://user-images.githubusercontent.com/12836843/156403671-b692526d-f75c-4f57-a5d9-2d69206e9355.jpg)

*Starting of mixer*

The next specification (left) is related to the conditions in which the mixer can be turned on.  Thus, event $`M^{on}`$ can only occur after at least one occurrence of event $`L_{H_1}`$. To achieve this behavior, the associated reduced supervisor (right) disables event $`M^{on}`$ at state 0.

![E6](https://user-images.githubusercontent.com/12836843/156404292-f937331e-63e9-4007-9235-f365c9716a0c.jpg)

*Stopping of mixer*

The next specification (left) is related to the conditions in which the mixer can be turned off.  Thus, event $`M^{off}`$ can only occur after at least one occurrence of event $`cooled`$. To achieve this behavior, the associated reduced supervisor (right) disables event $`M^{off}`$ at state 0.

![E7](https://user-images.githubusercontent.com/12836843/156404519-0dcc78ef-27f1-4af2-a4e4-2a335c175d83.jpg)

*Starting of pump*

The next specification (left) is related to the conditions in which the pump can be turned on.  Thus, event $`P^{on}`$ can only occur after at least one occurrence of event $`heated`$. To achieve this behavior, the associated reduced supervisor (right) disables event $`P^{on}`$ at states 0 and 2.

![E8](https://user-images.githubusercontent.com/12836843/156404859-c2650f8e-4167-48fe-8b91-2fb345ed4eb4.jpg)

*Stopping of pump*

The next specification (left) is related to the conditions in which the pump can be turned off.  Thus, event $`P^{off}`$ can only occur after at least one occurrence of event $`cooled`$. To achieve this behavior, the associated reduced supervisor (right) disables event $`P^{off}`$ at states 0 and 2.

![E9](https://user-images.githubusercontent.com/12836843/156404967-ef6a76f3-cab8-4b6e-bd24-97722321fb39.jpg)

*Starting of temperature control*

The next specification (left) is related to the conditions in which the temperature control can be turned on.  Thus, event $`T^{on}`$ can only occur after at least one occurrence of event $`L_{H_1}`$. To achieve this behavior, the associated reduced supervisor (right) disables event $`T^{on}`$ at state 0.

![E10](https://user-images.githubusercontent.com/12836843/156405280-0f967e5f-9417-42cc-b891-b565484533ad.jpg)

*Stopping of temperature control*

The next specification (left) is related to the conditions in which the temperature control can be turned off.  Thus, event $`T^{off}`$ can only occur after at least one occurrence of event $`heated`$ followed by at least one ocurrence of event $`cooled`$. To achieve this behavior, the associated reduced supervisor (right) disables event $`T^{off}`$ at states 1 and 2.

![E11](https://user-images.githubusercontent.com/12836843/156405569-db6a2ad7-8c6c-486c-aadc-dc10a66ffca1.jpg)
 
 # Add link to Ultrades code with models
 
[Back](https://github.com/michelrodrigo/DES-control-system)





