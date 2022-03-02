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

We introduce the models of the subsystems. The models are an abstraction of the real systems and we only consider in here events that are relevant to the cordination between them. Transitions with a streak are labelled with controllable events, while the ones without a streak are labelled with uncontrollable ones.

Process

![process](https://user-images.githubusercontent.com/12836843/156244315-d515c71d-462f-4573-9efd-d8ecadb2f699.jpg) 

Input valve

![vin](https://user-images.githubusercontent.com/12836843/156243984-ccadc934-324c-4f45-8ff1-ccf22a816bf1.jpg)

Output valve

![vout](https://user-images.githubusercontent.com/12836843/156244252-2bf9299e-1ae3-4a76-a7b2-75d3ba68d734.jpg)

Mixer

![mixer](https://user-images.githubusercontent.com/12836843/156244339-4c313b4d-c457-4911-9282-24ec2f01273c.jpg)

Pump

![pump](https://user-images.githubusercontent.com/12836843/156245188-807ea470-01df-4e59-aa9e-3b9e7cee91ab.jpg)

Temperature Control

![temp](https://user-images.githubusercontent.com/12836843/156244390-6c817a34-647e-4fdf-95c5-1771d9234168.jpg)

[Back](https://github.com/michelrodrigo/DES-control-system)

### Specifications and Supervisors

The first specification (left) restricts the opening of the input valve to the beggining of the process. Event $`V_{in}^{open}`$ can only occur after the occurrence of event $`start`$. The associated supervisor (right) disables event $`V_{in}^{open}`$ at state 0.

![E4](https://user-images.githubusercontent.com/12836843/156268551-aeca3454-4130-4dd8-ab80-186fbd43ea81.jpg)

The following specification (left) is related to the conditions in which the input valve can be closed.  Thus, event $`V_{in}^{close}`$ can only occur after at least one occurrence of event $`L_{H_1}`$. To achieve this behavior, the associated reduced supervisor (right) disables event $`V_{in}^{close}`$ at state 1.

![E2](https://user-images.githubusercontent.com/12836843/156251621-a105b1b1-9e3b-4f3d-a6ef-8a6487138843.jpg)

The next specification (left) is related to the conditions in which the output valve can be closed.  Thus, event $`V_{out}^{close}`$ can only occur after at least one occurrence of event $`L_{L_1}`$. To achieve this behavior, the associated reduced supervisor (right) disables event $`V_{out}^{close}`$ at state 1.

![E3](https://user-images.githubusercontent.com/12836843/156263459-2872176b-66fb-4e78-90d6-1adfd9a9600f.jpg)

The following specification (left) conditions the opening of the output valve to the conclusion of the temperature control. Hence, event $`V_{out}^{open}`$ can only occur after  the occurrence of event $`cooled`$. To achieve this behavior, the associated reduced supervisor (right) disables event $`V_{out}^{open}`$ at state 0.


![E5](https://user-images.githubusercontent.com/12836843/156269907-b3712585-df01-4a6d-8fa2-9cfaf5f7cbc9.jpg)
