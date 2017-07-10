
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


void StateMachine::init()
{
	for (int i = 0; i < maxOrthogonalStates; ++i)
		stateConfVector[i] = AccuChargerExtensionSM_last_state;
	
	stateConfVectorPosition = 0;

	clearInEvents();
	clearOutEvents();
	
	/* Default init sequence for statechart accuChargerExtensionSM */
	iface.nrOfChargingPorts = 8;
	iface.samplingTime_ms = 5;
	iface.chargingCurrent_raw = 0;
	iface.preChargeVoltage_raw = 0;
	ifaceInternalSCI.actualChargingPort = 0;
	ifaceInternalSCI.chargingCurrent_filtered_raw = 0.0;
	ifaceInternalSCI.preChargingPinOn = false;
	ifaceInternalSCI.a = 0.0;
	ifaceInternalSCI.chargingTime_s = 0;
	ifaceInternalSCI.chargedPorts = 0;
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
	/* 'default' enter sequence for region (3) preCharge */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state preCharge */
	stateConfVector[2] = preCharge;
	stateConfVectorPosition = 2;
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
		case charging :
		{
			/* Default exit sequence for state charging */
			stateConfVector[1] = AccuChargerExtensionSM_last_state;
			stateConfVectorPosition = 1;
			/* Exit action for state 'charging'. */
			timer->unsetTimer(this, (sc_eventid)(&timeEvents[1]));
			break;
		}
		case endOfCharging :
		{
			/* Default exit sequence for state endOfCharging */
			stateConfVector[1] = AccuChargerExtensionSM_last_state;
			stateConfVectorPosition = 1;
			/* Exit action for state 'endOfCharging'. */
			timer->unsetTimer(this, (sc_eventid)(&timeEvents[2]));
			break;
		}
		default: break;
	}
	/* Default exit sequence for region (3) preCharge */
	/* Handle exit of all possible states (of accuChargerExtensionSM._3__preCharge) at position 2... */
	switch(stateConfVector[ 2 ])
	{
		case preCharge :
		{
			/* Default exit sequence for state preCharge */
			stateConfVector[2] = AccuChargerExtensionSM_last_state;
			stateConfVectorPosition = 2;
			break;
		}
		default: break;
	}
}

sc_boolean StateMachine::isActive()
{
	return stateConfVector[0] != AccuChargerExtensionSM_last_state||stateConfVector[1] != AccuChargerExtensionSM_last_state||stateConfVector[2] != AccuChargerExtensionSM_last_state;
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
		case charging :
		{
			react_charging();
			break;
		}
		case endOfCharging :
		{
			react_endOfCharging();
			break;
		}
		case preCharge :
		{
			react_preCharge();
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
	iface.chargedPortsUpdate_raised = false;
	iface.preChargeUpdate_raised = false;
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
			return (sc_boolean) (stateConfVector[1] == charging
			);
		case endOfCharging : 
			return (sc_boolean) (stateConfVector[1] == endOfCharging
			);
		case preCharge : 
			return (sc_boolean) (stateConfVector[2] == preCharge
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

sc_boolean StateMachine::DefaultSCI::isRaised_chargedPortsUpdate()
{
	return chargedPortsUpdate_raised;
}

sc_boolean StateMachine::isRaised_chargedPortsUpdate()
{
	return iface.isRaised_chargedPortsUpdate();
}

sc_integer StateMachine::DefaultSCI::get_chargedPortsUpdate_value()
{
	return chargedPortsUpdate_value;
}

sc_integer StateMachine::get_chargedPortsUpdate_value()
{
	return iface.get_chargedPortsUpdate_value();
}

sc_boolean StateMachine::DefaultSCI::isRaised_preChargeUpdate()
{
	return preChargeUpdate_raised;
}

sc_boolean StateMachine::isRaised_preChargeUpdate()
{
	return iface.isRaised_preChargeUpdate();
}

sc_boolean StateMachine::DefaultSCI::get_preChargeUpdate_value()
{
	return preChargeUpdate_value;
}

sc_boolean StateMachine::get_preChargeUpdate_value()
{
	return iface.get_preChargeUpdate_value();
}


sc_integer StateMachine::DefaultSCI::get_nrOfChargingPorts()
{
	return nrOfChargingPorts;
}

sc_integer StateMachine::get_nrOfChargingPorts()
{
	return iface.nrOfChargingPorts;
}

sc_integer StateMachine::DefaultSCI::get_samplingTime_ms()
{
	return samplingTime_ms;
}

sc_integer StateMachine::get_samplingTime_ms()
{
	return iface.samplingTime_ms;
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

sc_integer StateMachine::DefaultSCI::get_preChargeVoltage_raw()
{
	return preChargeVoltage_raw;
}

sc_integer StateMachine::get_preChargeVoltage_raw()
{
	return iface.preChargeVoltage_raw;
}

void StateMachine::DefaultSCI::set_preChargeVoltage_raw(sc_integer value)
{
	preChargeVoltage_raw = value;
}

void StateMachine::set_preChargeVoltage_raw(sc_integer value)
{
	iface.preChargeVoltage_raw = value;
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

sc_boolean StateMachine::InternalSCI::get_preChargingPinOn()
{
	return preChargingPinOn;
}

void StateMachine::InternalSCI::set_preChargingPinOn(sc_boolean value)
{
	preChargingPinOn = value;
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

sc_integer StateMachine::InternalSCI::get_chargedPorts()
{
	return chargedPorts;
}

void StateMachine::InternalSCI::set_chargedPorts(sc_integer value)
{
	chargedPorts = value;
}


// implementations of all internal functions

/* The reactions of state filter. */
void StateMachine::react_filter()
{
	/* The reactions of state filter. */
	/* Default exit sequence for state filter */
	stateConfVector[0] = AccuChargerExtensionSM_last_state;
	stateConfVectorPosition = 0;
	ifaceInternalSCI.a = InternalSCI_OCB::exponential(-iface.samplingTime_ms / 16.6);
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
		/* 'default' enter sequence for state charging */
		/* Entry action for state 'charging'. */
		timer->setTimer(this, (sc_eventid)(&timeEvents[1]), 1 * 1000, false);
		stateConfVector[1] = charging;
		stateConfVectorPosition = 1;
	} 
}

/* The reactions of state charging. */
void StateMachine::react_charging()
{
	/* The reactions of state charging. */
	if (ifaceInternalSCI.chargingCurrent_filtered_raw < 514.5)
	{ 
		/* Default exit sequence for state charging */
		stateConfVector[1] = AccuChargerExtensionSM_last_state;
		stateConfVectorPosition = 1;
		/* Exit action for state 'charging'. */
		timer->unsetTimer(this, (sc_eventid)(&timeEvents[1]));
		/* 'default' enter sequence for state endOfCharging */
		/* Entry action for state 'endOfCharging'. */
		timer->setTimer(this, (sc_eventid)(&timeEvents[2]), 5 * 1000, false);
		stateConfVector[1] = endOfCharging;
		stateConfVectorPosition = 1;
	}  else
	{
		if (timeEvents[1])
		{ 
			/* Default exit sequence for state charging */
			stateConfVector[1] = AccuChargerExtensionSM_last_state;
			stateConfVectorPosition = 1;
			/* Exit action for state 'charging'. */
			timer->unsetTimer(this, (sc_eventid)(&timeEvents[1]));
			ifaceInternalSCI.chargingTime_s += 1;
			/* 'default' enter sequence for state charging */
			/* Entry action for state 'charging'. */
			timer->setTimer(this, (sc_eventid)(&timeEvents[1]), 1 * 1000, false);
			stateConfVector[1] = charging;
			stateConfVectorPosition = 1;
		} 
	}
}

/* The reactions of state endOfCharging. */
void StateMachine::react_endOfCharging()
{
	/* The reactions of state endOfCharging. */
	if (timeEvents[2])
	{ 
		/* Default exit sequence for state endOfCharging */
		stateConfVector[1] = AccuChargerExtensionSM_last_state;
		stateConfVectorPosition = 1;
		/* Exit action for state 'endOfCharging'. */
		timer->unsetTimer(this, (sc_eventid)(&timeEvents[2]));
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
			/* 'default' enter sequence for state charging */
			/* Entry action for state 'charging'. */
			timer->setTimer(this, (sc_eventid)(&timeEvents[1]), 1 * 1000, false);
			stateConfVector[1] = charging;
			stateConfVectorPosition = 1;
		} 
	}
}

/* The reactions of state preCharge. */
void StateMachine::react_preCharge()
{
	/* The reactions of state preCharge. */
	/* Default exit sequence for state preCharge */
	stateConfVector[2] = AccuChargerExtensionSM_last_state;
	stateConfVectorPosition = 2;
	iface.preChargeUpdate_value = iface.preChargeVoltage_raw < 200;
	iface.preChargeUpdate_raised = true;
	/* 'default' enter sequence for state preCharge */
	stateConfVector[2] = preCharge;
	stateConfVectorPosition = 2;
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
	ifaceInternalSCI.chargedPorts = 0;
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
	if (ifaceInternalSCI.chargingTime_s < 1800)
	{ 
		ifaceInternalSCI.chargedPorts |= 1 << ifaceInternalSCI.actualChargingPort;
		iface.chargedPortsUpdate_value = ifaceInternalSCI.chargedPorts;
		iface.chargedPortsUpdate_raised = true;
		react__choice2_();
	}  else
	{
		react__choice2_();
	}
}

/* The reactions of state null. */
void StateMachine::react__choice2_()
{
	/* The reactions of state null. */
	ifaceInternalSCI.actualChargingPort = (ifaceInternalSCI.actualChargingPort + 1) % iface.nrOfChargingPorts;
	/* 'default' enter sequence for state off */
	/* Entry action for state 'off'. */
	timer->setTimer(this, (sc_eventid)(&timeEvents[0]), 100, false);
	stateConfVector[1] = off;
	stateConfVectorPosition = 1;
}


