<Query Kind="Statements">
  <Reference>&lt;RuntimeDirectory&gt;\System.ValueTuple.dll</Reference>
  <NuGetReference>UltraDES</NuGetReference>
  <Namespace>UltraDES</Namespace>
</Query>

// States
var s = Enumerable.Range(0, 20).Select(ii => new State($"s{ii}", ii == 0 ? Marking.Marked : Marking.Unmarked)).ToArray();


// Events
Event start = new Event("start", Controllability.Uncontrollable);
Event finish = new Event("finish", Controllability.Uncontrollable);
Event L_H1 = new Event("L_H1", Controllability.Uncontrollable);
Event L_L1 = new Event("L_L1", Controllability.Uncontrollable);
Event Vin_open = new Event("Vin_open", Controllability.Controllable);
Event Vin_close = new Event("Vin_close", Controllability.Controllable);
Event Vout_open = new Event("Vout_open", Controllability.Controllable);
Event Vout_close = new Event("Vout_close", Controllability.Controllable);
Event P_on = new Event("P_on", Controllability.Controllable);
Event P_off = new Event("P_off", Controllability.Controllable);
Event M_on = new Event("M_on", Controllability.Controllable);
Event M_off = new Event("M_off", Controllability.Controllable); 
Event T_on = new Event("T_on", Controllability.Controllable); 
Event T_off = new Event("T_off", Controllability.Controllable); 
Event heated = new Event("heated", Controllability.Uncontrollable);
Event cooled = new Event("cooled", Controllability.Uncontrollable);
Event rst = new Event("rst", Controllability.Uncontrollable);

//Plant automata
var PROCESS = new DeterministicFiniteAutomaton(new[]
{
	new Transition(s[0], start, s[1]),
	new Transition(s[1], finish, s[0]),
	new Transition(s[0], rst, s[0]),
	new Transition(s[1], rst, s[0]),

  }, s[0], "PROCESS");

var VIN = new DeterministicFiniteAutomaton(new[]
{
	new Transition(s[0], Vin_open, s[1]),
	new Transition(s[1], L_H1, s[1]),
	new Transition(s[1], Vin_close, s[0]),
	new Transition(s[0], rst, s[0]),
	new Transition(s[1], rst, s[0]),

  }, s[0], "VIN");

var VOUT = new DeterministicFiniteAutomaton(new[]
{
	new Transition(s[0], Vout_open, s[1]),
	new Transition(s[1], L_L1, s[1]),
	new Transition(s[1], Vout_close, s[0]),
	new Transition(s[0], rst, s[0]),
	new Transition(s[1], rst, s[0]),

	}, s[0], "VOUT");


var MIXER = new DeterministicFiniteAutomaton(new[]
{
	new Transition(s[0], M_on, s[1]),
	new Transition(s[1], M_off, s[0]),
	new Transition(s[0], rst, s[0]),
	new Transition(s[1], rst, s[0]),

	}, s[0], "MIXER");

var PUMP = new DeterministicFiniteAutomaton(new[]
{
	new Transition(s[0], P_on, s[1]),
	new Transition(s[1], P_off, s[0]),
	new Transition(s[0], rst, s[0]),
	new Transition(s[1], rst, s[0]),

	}, s[0], "PUMP");

var TEMP = new DeterministicFiniteAutomaton(new[]
{
	new Transition(s[0], T_on, s[1]),
	new Transition(s[1], heated, s[1]),
	new Transition(s[1], cooled, s[1]),
	new Transition(s[1], T_off, s[0]),
	new Transition(s[0], rst, s[0]),
	new Transition(s[1], rst, s[0]),

	}, s[0], "TEMP");

//Specifcation automata

var E1 = new DeterministicFiniteAutomaton(new[]
{
	new Transition(s[0], start, s[1]),
	new Transition(s[1], Vin_open, s[0]),
	new Transition(s[1], rst, s[0]),
	new Transition(s[0], rst, s[0]),	

  }, s[0], "E1");

var E2 = new DeterministicFiniteAutomaton(new[]
{
	new Transition(s[0], L_H1, s[1]),
	new Transition(s[1], Vin_close, s[0]),
	new Transition(s[1], L_H1, s[1]),
	new Transition(s[0], rst, s[0]),
	new Transition(s[1], rst, s[0]),	

  }, s[0], "E2");

var E3 = new DeterministicFiniteAutomaton(new[]
{
	new Transition(s[0], cooled, s[1]),
	new Transition(s[1], Vout_open, s[0]),
	new Transition(s[1], cooled, s[1]),
	new Transition(s[0], rst, s[0]),
	new Transition(s[1], rst, s[0]),

  }, s[0], "E3");

var E4 = new DeterministicFiniteAutomaton(new[]
{
	new Transition(s[0], L_L1, s[1]),
	new Transition(s[1], Vout_close, s[0]),
	new Transition(s[1], L_L1, s[1]),
	new Transition(s[0], rst, s[0]),
	new Transition(s[1], rst, s[0]),

  }, s[0], "E4");

var E5 = new DeterministicFiniteAutomaton(new[]
{
	new Transition(s[0], L_H1, s[1]),
	new Transition(s[1], M_on, s[0]),
	new Transition(s[1], L_H1, s[1]),
	new Transition(s[0], rst, s[0]),
	new Transition(s[1], rst, s[0]),

  }, s[0], "E5");

var E6 = new DeterministicFiniteAutomaton(new[]
{
	new Transition(s[0], cooled, s[1]),
	new Transition(s[1], M_off, s[0]),
	new Transition(s[1], cooled, s[1]),
	new Transition(s[0], rst, s[0]),
	new Transition(s[1], rst, s[0]),

  }, s[0], "E6");

var E7 = new DeterministicFiniteAutomaton(new[]
{
	new Transition(s[0], heated, s[1]),
	new Transition(s[1], P_on, s[0]),
	new Transition(s[1], heated, s[1]),
	new Transition(s[0], rst, s[0]),
	new Transition(s[1], rst, s[0]),

  }, s[0], "E7");

var E8 = new DeterministicFiniteAutomaton(new[]
{
	new Transition(s[0], cooled, s[1]),
	new Transition(s[1], P_off, s[0]),
	new Transition(s[1], cooled, s[1]),
	new Transition(s[0], rst, s[0]),
	new Transition(s[1], rst, s[0]),

  }, s[0], "E8");

var E9 = new DeterministicFiniteAutomaton(new[]
{
	new Transition(s[0], L_H1, s[1]),
	new Transition(s[1], T_on, s[0]),
	new Transition(s[1], L_H1, s[1]),
	new Transition(s[0], rst, s[0]),
	new Transition(s[1], rst, s[0]),

  }, s[0], "E9");

var E10 = new DeterministicFiniteAutomaton(new[]
{
	new Transition(s[0], heated, s[1]),	
	new Transition(s[0], cooled, s[0]),	
	new Transition(s[1], heated, s[1]),
	new Transition(s[1], cooled, s[2]),
	new Transition(s[2], heated, s[2]),
	new Transition(s[2], cooled, s[2]),
	new Transition(s[0], rst, s[0]),
	new Transition(s[1], rst, s[0]),
	new Transition(s[2], rst, s[0]),

  }, s[0], "E10");

var sups = DeterministicFiniteAutomaton.LocalModularSupervisor(new[] {PROCESS, VIN, VOUT, MIXER, PUMP, TEMP }, new[] { E1, E2, E3, E4, E5, E6, E7, E8, E9, E10});
