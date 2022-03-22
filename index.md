{% include_layouts/default.html %}

# A Security Testbed for a Networked DES Control System

The contents of this repository are related to the submitted paper to the [16th Workshop on Discrete-Event Systems](https://www.ifac-control.org/conferences/discrete-event-systems-16th-wodes-2022).

## Process

In this section we describe the physical process in which we applied the proposed control solution. The general idea is the production of batches of a liquid that is the product of reactions that happen at different temperatures inside a reactor. A depiction of the process can be seen next.

![main_screen](https://user-images.githubusercontent.com/12836843/122487626-13e78380-cfb2-11eb-9f74-1fe73cbb9f6a.JPG)

The process start by the admission of three different liquids, after the Start button is pressed by an operator. It is assumed that the flow of each liquid is defined by a previous process and is out of scope in here. There are three on-off input valves, each one for a different liquid, that receive the same control action, that is, they open and close at the same time. Since the valves remain opened by the same amount of time, the proportion of each liquid in the tank is determined by their flow. 

The total amount of liquid in the tank, which has a cylindrical shape, is a controlled variable and it is measured by a level sensor. Once the level in the tank reaches the set point, the input valves are closed and the mixer is turned on. The mixer makes the mix more homogeneous and therefore diminishes the total time of the batch production.

After the mixer has started, a continuous-time PI controller makes the temperature reach a desired value and remain there for a predefined period of time. After that, the controller makes the temperature reach a second predefined value, which is lower than the previous one, for another amount of time. A heater installed in the tank makes it possible to increase the temperature. To allow the liquid to cool faster, there is a heat exchanger coupled in the tank. To make the liquid go through it, a pump is turned on. Once this part of the process is finished, the mixer and the pump stop and an output valve is opened. The valve will remain opened until the level sensor indicates that the tank has reached the low level. When this happens, the output valve is closed and the batch is finished. The system is now ready to produce another batch.

## Models

### Subsystems

We introduce the automata models of the subsystems. The models are an abstraction of the real systems and we only consider in here events that are relevant to the cordination between them. Transitions with a streak are labelled with controllable events, while the ones without a streak are labelled with uncontrollable ones. All the models have an uncontrollable reset event  $$rst$$ $$\Sigma$$. This event is useful to restore the sync between the subsystem and supervisors if something fails.

*Process*

The process automaton does not represent a specific subsystem. Instead, it represents the whole system and it is useful when modeling specifications that restrain the behavior of a subsystem to the beggining or to the end of a production cycle.


<img src="https://user-images.githubusercontent.com/12836843/156402392-99313e49-8ec7-458c-805d-fe45e9d9bd43.jpg" width="100" height="100">
<img width="250" height="400" src="https://raw.githubusercontent.com/12836843/156402392-99313e49-8ec7-458c-805d-fe45e9d9bd43.jpg" />

### Hardware

### Software


