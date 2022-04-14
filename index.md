{% include lib/mathjax.html %}

# A Security Testbed for a Networked DES Control System

The contents of this repository are related to the submitted paper to the [16th Workshop on Discrete-Event Systems](https://www.ifac-control.org/conferences/discrete-event-systems-16th-wodes-2022).

## Process

In this section we describe the physical process in which we applied the proposed control solution. The general idea is the production of batches of a liquid that is the product of reactions that happen at different temperatures inside a reactor. A depiction of the process can be seen next.

![main_screen](https://user-images.githubusercontent.com/12836843/122487626-13e78380-cfb2-11eb-9f74-1fe73cbb9f6a.JPG)

The process start by the admission of three different liquids, after the Start button is pressed by an operator. It is assumed that the flow of each liquid is defined by a previous process and is out of scope in here. There are three on-off input valves, each one for a different liquid, that receive the same control action, that is, they open and close at the same time. Since the valves remain opened by the same amount of time, the proportion of each liquid in the tank is determined by their flow. 

The total amount of liquid in the tank, which has a cylindrical shape, is a controlled variable and it is measured by a level sensor. Once the level in the tank reaches the set point, the input valves are closed and the mixer is turned on. The mixer makes the mix more homogeneous and therefore diminishes the total time of the batch production.

After the mixer has started, a continuous-time PI controller makes the temperature reach a desired value and remain there for a predefined period of time. After that, the controller makes the temperature reach a second predefined value, which is lower than the previous one, for another amount of time. A heater installed in the tank makes it possible to increase the temperature. To allow the liquid to cool faster, there is a heat exchanger coupled in the tank. To make the liquid go through it, a pump is turned on. Once this part of the process is finished, the mixer and the pump stop and an output valve is opened. The valve will remain opened until the level sensor indicates that the tank has reached the low level. When this happens, the output valve is closed and the batch is finished. The system is now ready to produce another batch.

## Models

In the following we present the models of subsystems, specifications and reduced supervisors. The reduced supervisors were obtained by using the computational tool [Ultrades](https://github.com/lacsed/UltraDES), with the code available in [here](https://github.com/michelrodrigo/DES-control-system/blob/gh-pages/sources/models_github.linq).

### Subsystems

We introduce the automata models of the subsystems. The models are an abstraction of the real systems and we only consider in here events that are relevant to the cordination between them. Transitions with a streak are labelled with controllable events, while the ones without a streak are labelled with uncontrollable ones. All the models have an uncontrollable reset event  $$rst$$. This event is useful to restore the sync between the subsystem and supervisors if something fails.

#### Process

The process automaton does not represent a specific subsystem. Instead, it represents the whole system and it is useful when modeling specifications that restrain the behavior of a subsystem to the beggining or to the end of a production cycle. At the initial state the process is at state idle ($$I$$). After ocurrence of event $$start$$, it goes to state working ($$W$$). Once the process finishes, event $$finish$$ occurs and it goes to the initial state.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/162103287-8f99e6a9-0b7f-4e66-8805-0470efeaf98e.png" >
</p>


#### Input valve

The input valve is represented by a two-state automaton. At the initial state the valve is closed ($$C$$). It opens ($$O$$) with the occurrence of event $$V_{in}^{open}$$. In the open state, the sensor level may trigger event $$L_{H_1}$$, which is represented by a self-loop. The valve closes with event $$V_{in}^{close}$$. 

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/162103723-8fc3df3c-abc1-4d02-9ea0-4ac3df21dced.png" >
</p>


#### Output valve

The output valve is represented by a two-state automaton. At the initial state the valve is closed ($$C$$). It opens ($$O$$) with the occurrence of event $$V_{out}^{open}$$. In the open state, the sensor level may trigger event $$L_{L_1}$$, which is represented by a self-loop. The valve closes with event $$V_{out}^{close}$$.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/162103843-96929f5a-2a7f-48de-a547-9a9f9b3a0840.png">
</p>


#### Mixer

The behavior of the mixer is represented by a two-state automaton. At the initial state, the mixer is not working ($$I$$). Upon the ocurrence of event $$M^{on}$$, the mixer goes to state working ($$W$$). It remains there until event $$M^{off}$$ occurs, leading it back to the initial state.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/162103972-c000c865-bfd1-4df4-b40c-52e6b1032c71.png">
</p>


#### Pump

The behavior of the pump is represented by a two-state automaton. At the initial state, the pump is not working ($$I$$). Upon the ocurrence of event $$P^{on}$$, the pump goes to state working ($$W$$). It remains there until event $$P^{off}$$ occurs, leading it back to the initial state.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/162104090-d66e9644-13a1-4cb8-8096-8b3946a17537.png">
</p>


#### Temperature control

The behavior of the continuous-time temperature control is represented by a two-state automaton. The control is initially at the initial state ($$I$$). Once event $$T^{on}$$ occurs, the temperature control goes to state working ($$W$$). At this state, the continuous-time temperature controller controls the temperature of the liquid in the tank. How this is done, is not relevant for the DES control. For this reason, only events related to the completion of tasks are added in the model. Thus, event $$heated$$ indicates the the liquid has reached the higher temperature and remained in there for a given period of time while event $$cooled$$  represent that the liquid reached the lower temperature and also remained at it for a given amout of time. The control strategy, setpoints and the periods of time can be latter adjusted by the user.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/162104323-c057993c-2622-47e9-b83c-4813426c5455.png">
</p>


### Specifications and Supervisors

Next we present the specification and supervisor automata for the process. We adopt the Local Modular Supervisory Control approach for obtaining the supervisors, in which we apply a supervisor reduction technique, aiming to obtain even smaller automata. Furthermore, we represent the disablements at a given state of the supervisor by red dashed arrows. If an event is not disabled, then we assume it is enabled. 

#### Specification $$E_1$$ and Supervisor $$S_1$$

Specification $$E_1$$ conditions the closing of the input valve to the occurrence of event $$L_{H_1}$$, i.e., the input valve can only be closed when the tank is full. This is accomplished by disabling event $$V_{in}^{close}$$ at state 1 of supervisor $$S_1$$.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/162104639-fdfe7599-23f4-4d97-a238-c40525eb7616.png">
</p>


#### Specification $$E_2$$ and Supervisor $$S_2$$

Specification $$E_2$$ conditions the closing of the output valve to the occurrence of event $$L_{L_1}$$, i.e., the output valve can only be closed when the tank is empty. This is accomplished by disabling event $$V_{out}^{close}$$ at state 1 of supervisor $$S_2$$.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/162104751-293e45d4-b1c1-4665-a5a8-ab7ffddcebbd.png">
</p>


#### Specification $$E_3$$ and Supervisor $$S_3$$

Specification $$E_3$$ conditions the opening of the input valve to the occurrence of event $$start$$, i.e., the input valve can only be opened after the process has started. This is accomplished by disabling event $$V_{in}^{open}$$ at state 0 of supervisor $$S_3$$.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/162104955-177161b0-7ee1-436d-9b3e-c4394a3fea1b.png">
</p>


#### Specification $$E_4$$ and Supervisor $$S_4$$

Specification $$E_4$$ conditions the opening of the output valve to the occurrence of event $$cooled$$, i.e., the output valve can only be opened after the liquid has been cooled, which is the last step of production. This is accomplished by disabling event $$V_{out}^{open}$$ at state 0 of supervisor $$S_4$$.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/162105108-91246327-0938-4f6b-8f53-5dd82a87df14.png">
</p>


#### Specification $$E_5$$ and Supervisor $$S_5$$

Specification $$E_5$$ conditions the starting of the mixer to the occurrence of event $$L_{H_1}$$, i.e., the mixer can only start when the tank is full. This is accomplished by disabling event $$M^{on}$$ at state 0 of supervisor $$S_5$$.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/162105240-aa468e73-e852-4b54-bfbf-f0193e51e922.png">
</p>


#### Specification $$E_6$$ and Supervisor $$S_6$$

Specification $$E_6$$ conditions the stopping of the mixer to the occurrence of event $$cooled$$, i.e., the mixer can only stop after the liquid has been cooled, which is the last step of production. This is accomplished by disabling event $$M^{off}$$ at state 0 of supervisor $$S_6$$.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/162105348-099408cb-59c6-4013-9ab3-74729dcb230b.png">
</p>


#### Specification $$E_7$$ and Supervisor $$S_7$$

Specification $$E_7$$ conditions the starting of the pump to the occurrence of event $$heated$$, i.e., the pump can only start after the liquid has been heated. This is accomplished by disabling event $$P^{on}$$ at states 0 and 2 of supervisor $$S_7$$.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/162105482-78c87ec4-2b66-4d04-ae1d-3c067ae14e8a.png">
</p>

#### Specification $$E_8$$ and Supervisor $$S_8$$

Specification $$E_8$$ conditions the stopping  of the pump to the occurrence of event $$cooled$$, i.e., the pump can only stop after the liquid has been cooled, which is the last step of production. This is accomplished by disabling event $$P^{off}$$ at states 0 and 2 of supervisor $$S_8$$.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/162105594-587e0748-4c6d-4473-b58b-0611f33abea5.png">
</p>

#### Specification $$E_9$$ and Supervisor $$S_9$$

Specification $$E_9$$ conditions the starting of the temperature control to the occurrence of event $$L_{H_1}$$, i.e., the temperature control can only start after the tank is filled. This is accomplished by disabling event $$T^{on}$$ at state 0 of supervisor $$S_9$$.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/162105690-d9b81851-13d8-46ac-b90f-1e12262664b5.png">
</p>

#### Specification $$E_{10}$$ and Supervisor $$S_{10}$$

Specification $$E_{10}$$ conditions the stopping of the temperature control to the occurrence of event $$heated$$ followed by $$cooled$$, i.e., the temperature control can only stop after the temperature has reached the higher and lower setpoints. This is accomplished by disabling event $$T^{off}$$ at states 1 and 2 of supervisor $$S_{10}$$.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/162105818-d22a3a7a-4ee9-4c65-8cb6-88128ab44900.png">
</p>


### Hardware

In this section we describe the hardware employed to implement the control system. All nodes are implemented by Arduino boards. There is no restriction regarding the specific type of board to use in each node. Note, however, that depending on additional functionalities one want to add to a specific node, a board with higher  specifications may be needed.

The communication between devices is made by means of the [CAN protocol](https://www.ti.com/lit/an/sloa101b/sloa101b.pdf). We use the Arduino-compatible [CAN module MCP2515](https://www.ebay.com/itm/253369172743?_trkparms=ispr%3D1&hash=item3afdfac307:g:Z4wAAOSw9tlb4ifz&amdata=enc%3AAQAGAAAA4Nl%2F9N0iNYGQwO1Ki8eAB9GJ2ZgW11ahP9SFWkemlnqxOr041iMsZOreUfz6EGv%2BCABhGa9xo0VLJFZZUvveq2XqL5IEd4Mf9xQ45nB4v3hwVaOSHjQ5lpZmjWNs1x4z7CwO6qzNWdr4wYcklSlxkSaKbILHFg4wsQ%2Fq5rktKdsbP1MbV373Ub9E5V81Lb%2BHWdw1bGCdVpmTMwRBpkWmeeAvvjQctenVkNfL23SRbrVFm196NVCSFXCNEqWkBMvcbYXn1n%2F3yrL3n%2F0R9F%2BbJZkaQm%2F1EgXWsk38mMo3R1UO%7Ctkp%3ABFBMtKKU2oRg) as the interface between the devices and the CAN bus.

#### Controller

The main node of the system is the controller. It is implemented by a Arduino Mega board. The physical connections between the board and the MCP2515 module are shown in the figure below. Click [here](https://github.com/michelrodrigo/DES-control-system/blob/gh-pages/sources/controller.fzz) to access the fritzing file of this node.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/163293671-04c68655-bd53-42fe-a31c-581ce5f981a5.png">
</p>

#### Node 1

This node is implemented in a Arduino Uno board. Click [here](https://github.com/michelrodrigo/DES-control-system/blob/gh-pages/sources/node1.fzz) to access the fritzing file of this node.

We have connected to this node:
- Input valve (green LED): pin 4;
- Output valve (red LED): pin 3;
- Temperature sensor (output of RC circuit): pin A1;
- Heater (input of RC circuit): pin 5;
- CAN module MCP2515: connections are shown below.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/163298791-7a462059-242e-4cf4-a82c-93977e3ba51e.png">
</p>


#### Node 2

This node is implemented in a Arduino Nano board. Click [here](https://github.com/michelrodrigo/DES-control-system/blob/gh-pages/sources/node2.fzz) to access the fritzing file of this node. The LCD allows us to monitor continous time variables, such as temperature and level.


We have connected to this node:
- Mixer (yellow LED): pin D4;
- Pump (blue LED): pin D3;
- Level sensor (trimpot): pin A0;
- LCD display: pins A4 and A5;
- CAN module MCP2515: connections are shown below.


<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/163293179-eda765ce-f889-4e95-8bb6-93da1cfdee4c.png">
</p>

#### Node 3

This node is implemented in a Arduino Uno board. Click [here](https://github.com/michelrodrigo/DES-control-system/blob/gh-pages/sources/node3.fzz) to access the fritzing file of this node.

This node is used to implement the IDS. A identical node is used to implement the human-machine interface.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/163293176-f0578ac8-b765-4f5e-9e73-bfc466845a3a.png">
</p>


### Software


