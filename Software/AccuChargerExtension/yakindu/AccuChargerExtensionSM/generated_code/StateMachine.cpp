
#include "StateMachine.h"
#include <string.h>

/*! \file Implementation of the state machine 'accuChargerExtensionSM'
*/

StateMachine::StateMachine()
{
	
	
	stateConfVectorPosition = 0;
	
	timer = null;
}

StateMachine::~StateMachine()
{
}

const sc_integer StateMachine::DefaultSCI::NrOfChargingPorts = 4;
const sc_integer StateMachine::DefaultSCI::SamplingTime_ms = 5;
const sc_integer StateMachine::DefaultSCI::AccuStateUndef = 8;
const sc_integer StateMachine::DefaultSCI::AccuStateFull = 1;
const sc_integer StateMachine::DefaultSCI::AccuStateRun = 2;


void StateMachine::init()
{
	for (int i = 0; i < maxOrthogonalStates; ++i)
		stateConfVector[i] = AccuChargerExtensionSM_last_state;
	
	stateConfVectorPosition = 0;

	clearInEvents();
	clearOutEvents();
	
	/* Default init sequence for statechart accuChargerExtensionSM */
	iface.chargingCurrent_raw = 0;
	iface.accuState = 0;
	ifaceInternalSCI.actualChargingPort = 0;
	ifaceInternalSCI.chargingCurrent_filtered_raw = 0.0;
	ifaceInternalSCI.a = 0.0;
	ifaceInternalSCI.chargingTime_s = 0;
}

void StateMachine::enter()
{
	/* Default enter sequence for statechart accuChargerExtensionSM */
	/* 'default' enter sequence for region (1) chargingCurrentFilter */
	/* Default react sequence for initial entry  */
	react__choice0_();
	/* 'default' enter sequence for region (2) charge batteries */
	/* Default react sequence for initial entry  */
	react_react__choice0_();
}

void StateMachine::exit()
{
	/* Default exit sequence for statechart accuChargerExtensionSM */
	/* Default exit sequence for region (1) chargingCurrentFilter */
	/* Handle exit of all possible states (of accuChargerExtensionSM._1__chargingCurrentFilter) at position 0... */
	switch(stateConfVector[ 0 ])
	{
		case filter :
		{
			/* Default exit sequence for state filter */
			stateConfVector[0] = AccuChargerExtensionSM_last_state;
			stateConfVectorPosition = 0;
			break;
		}
		default: break;
	}
	/* Default exit sequence for region (2) charge batteries */
	/* Handle exit of all possible states (of accuChargerExtensionSM._2__charge_batteries) at position 1... */
	switch(stateConfVector[ 1 ])
	{
		case off :
		{
			/* Default exit sequence for state off */
			stateConfVector[1] = AccuChargerExtensionSM_last_state;
			stateConfVectorPosition = 1;
			/* Exit action for state 'off'. */
			timer->unsetTimer(this, (sc_eventid)(&timeEvents[0]));
			break;
		}
		case endOfCharging :
		{
			/* Default exit sequence for state endOfCharging */
			stateConfVector[1] = AccuChargerExtensionSM_last_state;
			stateConfVectorPosition = 1;
			/* Exit action for state 'endOfCharging'. */
			timer->unsetTimer(this, (sc_eventid)(&timeEvents[2]));
			/* Exit action for state 'charging'. */
			timer->unsetTimer(this, (sc_eventid)(&timeEvents[1]));
			break;
		}
		case charging_running :
		{
			/* Default exit sequence for state charging running */
			stateConfVector[1] = AccuChargerExtensionSM_last_state;
			stateConfVectorPosition = 1;
			/* Exit action for state 'charging'. */
			timer->unsetTimer(this, (sc_eventid)(&timeEvents[1]));
			break;
		}
		default: break;
	}
}

sc_boolean StateMachine::isActive()
{
	return stateConfVector[0] != AccuChargerExtensionSM_last_state||stateConfVector[1] != AccuChargerExtensionSM_last_state;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean StateMachine::isFinal()
{
   return false;}

void StateMachine::runCycle()
{
	
	clearOutEvents();
	
	for (stateConfVectorPosition = 0;
		stateConfVectorPosition < maxOrthogonalStates;
		stateConfVectorPosition++)
		{
			
		switch (stateConfVector[stateConfVectorPosition])
		{
		case filter :
		{
			react_filter();
			break;
		}
		case off :
		{
			react_off();
			break;
		}
		case endOfCharging :
		{
			react_endOfCharging();
			break;
		}
		case charging_running :
		{
			react_charging_running();
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents();
}

void StateMachine::clearInEvents()
{
	timeEvents[0] = false; 
	timeEvents[1] = false; 
	timeEvents[2] = false; 
}

void StateMachine::clearOutEvents()
{
	iface.chargingPortEnable_raised = false;
	iface.chargingPortDisable_raised = false;
	iface.accuStateUpdate_raised = false;
}


void StateMachine::setTimer(TimerInterface* timer)
{
	this->timer = timer;
}

TimerInterface* StateMachine::getTimer()
{
	return timer;
}

void StateMachine::raiseTimeEvent(sc_eventid evid)
{
	if ((evid >= (sc_eventid)timeEvents) && (evid < (sc_eventid)(&timeEvents[timeEventsCount])))
	{
		*(sc_boolean*)evid = true;
	}				
}

sc_boolean StateMachine::isStateActive(AccuChargerExtensionSMStates state)
{
	switch (state)
	{
		case filter : 
			return (sc_boolean) (stateConfVector[0] == filter
			);
		case off : 
			return (sc_boolean) (stateConfVector[1] == off
			);
		case charging : 
			return (sc_boolean) (stateConfVector[1] >= charging
				&& stateConfVector[1] <= charging_running);
		case endOfCharging : 
			return (sc_boolean) (stateConfVector[1] == endOfCharging
			);
		case charging_running : 
			return (sc_boolean) (stateConfVector[1] == charging_running
			);
		default: return false;
	}
}

StateMachine::DefaultSCI* StateMachine::getDefaultSCI()
{
	return &iface;
}

sc_boolean StateMachine::DefaultSCI::isRaised_chargingPortEnable()
{
	return chargingPortEnable_raised;
}

sc_boolean StateMachine::isRaised_chargingPortEnable()
{
	return iface.isRaised_chargingPortEnable();
}

sc_integer StateMachine::DefaultSCI::get_chargingPortEnable_value()
{
	return chargingPortEnable_value;
}

sc_integer StateMachine::get_chargingPortEnable_value()
{
	return iface.get_chargingPortEnable_value();
}

sc_boolean StateMachine::DefaultSCI::isRaised_chargingPortDisable()
{
	return chargingPortDisable_raised;
}

sc_boolean StateMachine::isRaised_chargingPortDisable()
{
	return iface.isRaised_chargingPortDisable();
}

sc_integer StateMachine::DefaultSCI::get_chargingPortDisable_value()
{
	return chargingPortDisable_value;
}

sc_integer StateMachine::get_chargingPortDisable_value()
{
	return iface.get_chargingPortDisable_value();
}

sc_boolean StateMachine::DefaultSCI::isRaised_accuStateUpdate()
{
	return accuStateUpdate_raised;
}

sc_boolean StateMachine::isRaised_accuStateUpdate()
{
	return iface.isRaised_accuStateUpdate();
}

sc_integer StateMachine::DefaultSCI::get_accuStateUpdate_value()
{
	return accuStateUpdate_value;
}

sc_integer StateMachine::get_accuStateUpdate_value()
{
	return iface.get_accuStateUpdate_value();
}


const sc_integer StateMachine::DefaultSCI::get_nrOfChargingPorts()
{
	return NrOfChargingPorts;
}

const sc_integer StateMachine::get_nrOfChargingPorts()
{
	return StateMachine::DefaultSCI::NrOfChargingPorts;
}

const sc_integer StateMachine::DefaultSCI::get_samplingTime_ms()
{
	return SamplingTime_ms;
}

const sc_integer StateMachine::get_samplingTime_ms()
{
	return StateMachine::DefaultSCI::SamplingTime_ms;
}

const sc_integer StateMachine::DefaultSCI::get_accuStateUndef()
{
	return AccuStateUndef;
}

const sc_integer StateMachine::get_accuStateUndef()
{
	return StateMachine::DefaultSCI::AccuStateUndef;
}

const sc_integer StateMachine::DefaultSCI::get_accuStateFull()
{
	return AccuStateFull;
}

const sc_integer StateMachine::get_accuStateFull()
{
	return StateMachine::DefaultSCI::AccuStateFull;
}

const sc_integer StateMachine::DefaultSCI::get_accuStateRun()
{
	return AccuStateRun;
}

const sc_integer StateMachine::get_accuStateRun()
{
	return StateMachine::DefaultSCI::AccuStateRun;
}

sc_integer StateMachine::DefaultSCI::get_chargingCurrent_raw()
{
	return chargingCurrent_raw;
}

sc_integer StateMachine::get_chargingCurrent_raw()
{
	return iface.chargingCurrent_raw;
}

void StateMachine::DefaultSCI::set_chargingCurrent_raw(sc_integer value)
{
	chargingCurrent_raw = value;
}

void StateMachine::set_chargingCurrent_raw(sc_integer value)
{
	iface.chargingCurrent_raw = value;
}

sc_integer StateMachine::DefaultSCI::get_accuState()
{
	return accuState;
}

sc_integer StateMachine::get_accuState()
{
	return iface.accuState;
}


sc_integer StateMachine::InternalSCI::get_actualChargingPort()
{
	return actualChargingPort;
}

void StateMachine::InternalSCI::set_actualChargingPort(sc_integer value)
{
	actualChargingPort = value;
}

sc_real StateMachine::InternalSCI::get_chargingCurrent_filtered_raw()
{
	return chargingCurrent_filtered_raw;
}

void StateMachine::InternalSCI::set_chargingCurrent_filtered_raw(sc_real value)
{
	chargingCurrent_filtered_raw = value;
}

sc_real StateMachine::InternalSCI::get_a()
{
	return a;
}

void StateMachine::InternalSCI::set_a(sc_real value)
{
	a = value;
}

sc_integer StateMachine::InternalSCI::get_chargingTime_s()
{
	return chargingTime_s;
}

void StateMachine::InternalSCI::set_chargingTime_s(sc_integer value)
{
	chargingTime_s = value;
}


// implementations of all internal functions

/* The reactions of state filter. */
void StateMachine::react_filter()
{
	/* The reactions of state filter. */
	/* Default exit sequence for state filter */
	stateConfVector[0] = AccuChargerExtensionSM_last_state;
	stateConfVectorPosition = 0;
	ifaceInternalSCI.a = InternalSCI_OCB::exponential(-StateMachine::DefaultSCI::SamplingTime_ms / 16.6);
	ifaceInternalSCI.chargingCurrent_filtered_raw = iface.chargingCurrent_raw * (1 - ifaceInternalSCI.a) + ifaceInternalSCI.chargingCurrent_filtered_raw * ifaceInternalSCI.a;
	/* 'default' enter sequence for state filter */
	stateConfVector[0] = filter;
	stateConfVectorPosition = 0;
}

/* The reactions of state off. */
void StateMachine::react_off()
{
	/* The reactions of state off. */
	if (timeEvents[0])
	{ 
		/* Default exit sequence for state off */
		stateConfVector[1] = AccuChargerExtensionSM_last_state;
		stateConfVectorPosition = 1;
		/* Exit action for state 'off'. */
		timer->unsetTimer(this, (sc_eventid)(&timeEvents[0]));
		iface.chargingPortEnable_value = ifaceInternalSCI.actualChargingPort;
		iface.chargingPortEnable_raised = true;
		ifaceInternalSCI.chargingTime_s = 0;
		/* Entry action for state 'charging'. */
		timer->setTimer(this, (sc_eventid)(&timeEvents[1]), 1 * 1000, true);
		/* 'default' enter sequence for state charging running */
		stateConfVector[1] = charging_running;
		stateConfVectorPosition = 1;
	} 
}

/* The reactions of state endOfCharging. */
void StateMachine::react_endOfCharging()
{
	/* The reactions of state endOfCharging. */
	if (timeEvents[1])
	{ 
		ifaceInternalSCI.chargingTime_s += 1;
	} 
	if (timeEvents[2])
	{ 
		/* Default exit sequence for state charging */
		/* Default exit sequence for region r1 */
		/* Handle exit of all possible states (of accuChargerExtensionSM._2__charge_batteries.charging.r1) at position 1... */
		switch(stateConfVector[ 1 ])
		{
			case endOfCharging :
			{
				/* Default exit sequence for state endOfCharging */
				stateConfVector[1] = AccuChargerExtensionSM_last_state;
				stateConfVectorPosition = 1;
				/* Exit action for state 'endOfCharging'. */
				timer->unsetTimer(this, (sc_eventid)(&timeEvents[2]));
				break;
			}
			case charging_running :
			{
				/* Default exit sequence for state charging running */
				stateConfVector[1] = AccuChargerExtensionSM_last_state;
				stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Exit action for state 'charging'. */
		timer->unsetTimer(this, (sc_eventid)(&timeEvents[1]));
		iface.chargingPortDisable_value = -1;
		iface.chargingPortDisable_raised = true;
		react__choice1_();
	}  else
	{
		if (ifaceInternalSCI.chargingCurrent_filtered_raw > 514.5)
		{ 
			/* Default exit sequence for state endOfCharging */
			stateConfVector[1] = AccuChargerExtensionSM_last_state;
			stateConfVectorPosition = 1;
			/* Exit action for state 'endOfCharging'. */
			timer->unsetTimer(this, (sc_eventid)(&timeEvents[2]));
			/* 'default' enter sequence for state charging running */
			stateConfVector[1] = charging_running;
			stateConfVectorPosition = 1;
		} 
	}
}

/* The reactions of state charging running. */
void StateMachine::react_charging_running()
{
	/* The reactions of state charging running. */
	if (timeEvents[1])
	{ 
		ifaceInternalSCI.chargingTime_s += 1;
	} 
	if (ifaceInternalSCI.chargingCurrent_filtered_raw < 514.5)
	{ 
		/* Default exit sequence for state charging running */
		stateConfVector[1] = AccuChargerExtensionSM_last_state;
		stateConfVectorPosition = 1;
		/* 'default' enter sequence for state endOfCharging */
		/* Entry action for state 'endOfCharging'. */
		timer->setTimer(this, (sc_eventid)(&timeEvents[2]), 5 * 1000, false);
		stateConfVector[1] = endOfCharging;
		stateConfVectorPosition = 1;
	} 
}

/* The reactions of state null. */
void StateMachine::react__choice0_()
{
	/* The reactions of state null. */
	ifaceInternalSCI.chargingCurrent_filtered_raw = 0;
	/* 'default' enter sequence for state filter */
	stateConfVector[0] = filter;
	stateConfVectorPosition = 0;
}

/* The reactions of state null. */
void StateMachine::react_react__choice0_()
{
	/* The reactions of state null. */
	iface.chargingPortDisable_value = -1;
	iface.chargingPortDisable_raised = true;
	ifaceInternalSCI.actualChargingPort = 0;
	/* 'default' enter sequence for state off */
	/* Entry action for state 'off'. */
	timer->setTimer(this, (sc_eventid)(&timeEvents[0]), 100, false);
	stateConfVector[1] = off;
	stateConfVectorPosition = 1;
}

/* The reactions of state null. */
void StateMachine::react__choice1_()
{
	/* The reactions of state null. */
	if (ifaceInternalSCI.chargingTime_s > 1800)
	{ 
		iface.accuState = StateMachine::DefaultSCI::AccuStateRun;
		react__choice2_();
	}  else
	{
		if (ifaceInternalSCI.chargingTime_s < 60)
		{ 
			iface.accuState = StateMachine::DefaultSCI::AccuStateUndef;
			react__choice2_();
		}  else
		{
			iface.accuState = StateMachine::DefaultSCI::AccuStateFull;
			react__choice2_();
		}
	}
}

/* The reactions of state null. */
void StateMachine::react__choice2_()
{
	/* The reactions of state null. */
	iface.accuStateUpdate_value = ifaceInternalSCI.actualChargingPort;
	iface.accuStateUpdate_raised = true;
	react__choice3_();
}

/* The reactions of state null. */
void StateMachine::react__choice3_()
{
	/* The reactions of state null. */
	ifaceInternalSCI.actualChargingPort = (ifaceInternalSCI.actualChargingPort + 1) % StateMachine::DefaultSCI::NrOfChargingPorts;
	/* 'default' enter sequence for state off */
	/* Entry action for state 'off'. */
	timer->setTimer(this, (sc_eventid)(&timeEvents[0]), 100, false);
	stateConfVector[1] = off;
	stateConfVectorPosition = 1;
}


