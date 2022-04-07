---
layout: base
---


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

We introduce the automata models of the subsystems. The models are an abstraction of the real systems and we only consider in here events that are relevant to the cordination between them. Transitions with a streak are labelled with controllable events, while the ones without a streak are labelled with uncontrollable ones. All the models have an uncontrollable reset event  `rst`. This event is useful to restore the sync between the subsystem and supervisors if something fails.

 #### Process

The process automaton does not represent a specific subsystem. Instead, it represents the whole system and it is useful when modeling specifications that restrain the behavior of a subsystem to the beggining or to the end of a production cycle. At the initial state the process is at state idle (I). After ocurrence of event `start`, it goes to state working (W). Once the process finishes, event `finish` occurs and it goes to the initial state.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/159518608-cc1402e0-1007-4a5d-b5d4-3b4bcb4b0da7.png" >
</p>



#### Input valve

The input valve is represented by a two-state automaton. At the initial state the valve is closed (C). It opens (O) with the occurrence of event `V_{in}^{open}`. In the open state, the sensor level may trigger event `L_{H_1}`, which is represented by a self-loop. The valve closes with event `V_{in}^{close}`.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/159517964-e3ced316-afd7-4970-8a70-942b165b0f72.png" >
</p>

#### Output valve

The output valve is represented by a two-state automaton. At the initial state the valve is closed (C). It opens (O) with the occurrence of event `V_{out}^{open}`. In the open state, the sensor level may trigger event `L_{L_1}`, which is represented by a self-loop. The valve closes with event `V_{out}^{close}`.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/159518667-0b8ffd7f-62ea-4470-851f-eb4c2ce3c508.png">
</p>

#### Mixer

The behavior of the mixer is represented by a two-state automaton. At the initial state, the mixer is not working (I). Upon the ocurrence of event `M^{on}`, the mixer goes to state working (W). It remains there until event `M^{off}` occurs, leading it back to the initial state.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/159518739-c6749eb3-9d74-416f-bd5a-2de85a6ef662.png">
</p>

#### Pump

The behavior of the pump is represented by a two-state automaton. At the initial state, the pump is not working (I). Upon the ocurrence of event `P^{on}`, the pump goes to state working (W). It remains there until event `P^{off}` occurs, leading it back to the initial state.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/159518827-a1bd05bb-cac1-4705-a669-f3efa4e31176.png">
</p>

#### Temperature control

The behavior of the continuous-time temperature control is represented by a two-state automaton. The control is initially at the initial state (I). Once event `T^{on}` occurs, the temperature control goes to state working (W). At this state, the continuous-time temperature controller controls the temperature of the liquid in the tank. How this is done, is not relevant for the DES control. For this reason, only events related to the completion of tasks are added in the model. Thus, event `heated` indicates the the liquid has reached the higher temperature and remained in there for a given period of time while event `cooled`  represent that the liquid reached the lower temperature and also remained at it for a given amout of time. The control strategy, setpoints and the periods of time can be latter adjusted by the user.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/159518900-812a702e-fa14-42d1-8796-07d0c0b2897a.png">
</p>

### Specifications and Supervisors

Next we present the specification and supervisor automata for the process. We adopt the Local Modular Supervisory Control approach for obtaining the supervisors, in which we apply a supervisor reduction technique, aiming to obtain even smaller automata. Furthermore, we represent the disablements at a given state of the supervisor by red dashed arrows. If an event is not disabled, then we assume it is enabled. 

#### Specification `E_1` and Supervisor `S_1`

Specification `E_1` conditions the closing of the input valve to the occurrence of event `L_{H_1}`, i.e., the input valve can only be closed when the tank is full. This is accomplished by disabling event `V_{in}^{close}` at state 1 of supervisor `S_1`.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/159519826-83859420-4224-4aa3-b50e-29ed1d588ecd.png">
</p>

#### Specification `E_2` and Supervisor `S_2`

Specification `E_2` conditions the closing of the output valve to the occurrence of event `L_{L_1}`, i.e., the output valve can only be closed when the tank is empty. This is accomplished by disabling event `V_{out}^{close}` at state 1 of supervisor `S_2`.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/159520233-77fbb392-d96c-46bf-ba7d-37d81683021f.png">
</p>

#### Specification `E_3` and Supervisor `S_3`

Specification `E_3` conditions the opening of the input valve to the occurrence of event `start`, i.e., the input valve can only be opened after the process has started. This is accomplished by disabling event `V_{in}^{open}` at state 0 of supervisor `S_3`.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/159520358-8955fe6b-b18c-4b1f-b599-d0857535678c.png">
</p>

#### Specification `E_4` and Supervisor `S_4`

Specification `E_4` conditions the opening of the output valve to the occurrence of event `cooled`, i.e., the output valve can only be opened after the liquid has been cooled, which is the last step of production. This is accomplished by disabling event `V_{out}^{open}` at state 0 of supervisor `S_4`.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/159537798-eab43e0e-bc17-4b76-a827-f7e586237ddf.png">
</p>

#### Specification `E_5` and Supervisor `S_5`

Specification `E_5` conditions the starting of the mixer to the occurrence of event `L_{H_1}`, i.e., the mixer can only start when the tank is full. This is accomplished by disabling event `M^{on}` at state 0 of supervisor `S_5`.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/159520894-0f999c8e-83b6-4816-8777-5f746af0fe9f.png">
</p>

#### Specification `E_6` and Supervisor `S_6`

Specification `E_6` conditions the stopping of the mixer to the occurrence of event `cooled`, i.e., the mixer can only stop after the liquid has been cooled, which is the last step of production. This is accomplished by disabling event `M^{off}` at state 0 of supervisor `S_6`.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/159520979-2c525817-a4f6-460b-870d-fa2f5758b695.png">
</p>

#### Specification `E_7` and Supervisor `S_7`

Specification `E_7` conditions the starting of the pump to the occurrence of event `heated`, i.e., the mixer can only stop after the liquid has been cooled, which is the last step of production. This is accomplished by disabling event `M^{off}` at state 0 of supervisor `S_6`.

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/159537737-93b8a6ce-702f-4823-9201-6e504ca88744.png">
</p>



#### Specification `E_8` and Supervisor `S_8`

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/159521257-aa894c7e-2df9-42f5-9466-ec1e3b87bd22.png">
</p>

#### Specification `E_9` and Supervisor `S_9`

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/159521292-27901cac-efbb-4938-94ce-d65ae6c55a64.png">
</p>

#### Specification `E_10` and Supervisor `S_10`

<p align="center">
  <img src="https://user-images.githubusercontent.com/12836843/159521307-a671df0a-a4a7-4dac-ad20-61f4b8b6e5d4.png">
</p>

### Hardware


### Software


