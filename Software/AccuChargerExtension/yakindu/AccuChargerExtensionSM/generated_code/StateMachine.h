
#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "sc_types.h"
#include "StatemachineInterface.h"
#include "TimedStatemachineInterface.h"

/*! \file Header of the state machine 'accuChargerExtensionSM'.
*/

class StateMachine : public TimedStatemachineInterface, public StatemachineInterface
{
	
	public:
		
		StateMachine();
		
		~StateMachine();
		
		/*! Enumeration of all states */ 
		typedef enum
		{
			filter,
			off,
			charging,
			endOfCharging,
			charging_running,
			AccuChargerExtensionSM_last_state
		} AccuChargerExtensionSMStates;
		
		//! Inner class for default interface scope.
		class DefaultSCI
		{
			
			public:
				/*! Gets the value of the variable 'NrOfChargingPorts' that is defined in the default interface scope. */
				const sc_integer get_nrOfChargingPorts();
				
				/*! Gets the value of the variable 'SamplingTime_ms' that is defined in the default interface scope. */
				const sc_integer get_samplingTime_ms();
				
				/*! Gets the value of the variable 'AccuStateUndef' that is defined in the default interface scope. */
				const sc_integer get_accuStateUndef();
				
				/*! Gets the value of the variable 'AccuStateFull' that is defined in the default interface scope. */
				const sc_integer get_accuStateFull();
				
				/*! Gets the value of the variable 'AccuStateRun' that is defined in the default interface scope. */
				const sc_integer get_accuStateRun();
				
				/*! Gets the value of the variable 'AccuStateInit' that is defined in the default interface scope. */
				const sc_integer get_accuStateInit();
				
				/*! Checks if the out event 'chargingPortEnable' that is defined in the default interface scope has been raised. */
				sc_boolean isRaised_chargingPortEnable();
				
				/*! Gets the value of the out event 'chargingPortEnable' that is defined in the default interface scope. */
				sc_integer get_chargingPortEnable_value();
				
				/*! Checks if the out event 'chargingPortDisable' that is defined in the default interface scope has been raised. */
				sc_boolean isRaised_chargingPortDisable();
				
				/*! Gets the value of the out event 'chargingPortDisable' that is defined in the default interface scope. */
				sc_integer get_chargingPortDisable_value();
				
				/*! Checks if the out event 'accuStateUpdate' that is defined in the default interface scope has been raised. */
				sc_boolean isRaised_accuStateUpdate();
				
				/*! Gets the value of the out event 'accuStateUpdate' that is defined in the default interface scope. */
				sc_integer get_accuStateUpdate_value();
				
				/*! Gets the value of the variable 'chargingCurrent_raw' that is defined in the default interface scope. */
				sc_integer get_chargingCurrent_raw();
				
				/*! Sets the value of the variable 'chargingCurrent_raw' that is defined in the default interface scope. */
				void set_chargingCurrent_raw(sc_integer value);
				
				/*! Gets the value of the variable 'accuState' that is defined in the default interface scope. */
				sc_integer get_accuState();
				
				
			private:
				friend class StateMachine;
				static const sc_integer NrOfChargingPorts;
				static const sc_integer SamplingTime_ms;
				static const sc_integer AccuStateUndef;
				static const sc_integer AccuStateFull;
				static const sc_integer AccuStateRun;
				static const sc_integer AccuStateInit;
				sc_boolean chargingPortEnable_raised;
				sc_integer chargingPortEnable_value;
				sc_boolean chargingPortDisable_raised;
				sc_integer chargingPortDisable_value;
				sc_boolean accuStateUpdate_raised;
				sc_integer accuStateUpdate_value;
				sc_integer chargingCurrent_raw;
				sc_integer accuState;
		};
				
		
		/*! Returns an instance of the interface class 'DefaultSCI'. */
		DefaultSCI* getDefaultSCI();
		
		/*! Gets the value of the variable 'NrOfChargingPorts' that is defined in the default interface scope. */
		const sc_integer get_nrOfChargingPorts();
		
		/*! Gets the value of the variable 'SamplingTime_ms' that is defined in the default interface scope. */
		const sc_integer get_samplingTime_ms();
		
		/*! Gets the value of the variable 'AccuStateUndef' that is defined in the default interface scope. */
		const sc_integer get_accuStateUndef();
		
		/*! Gets the value of the variable 'AccuStateFull' that is defined in the default interface scope. */
		const sc_integer get_accuStateFull();
		
		/*! Gets the value of the variable 'AccuStateRun' that is defined in the default interface scope. */
		const sc_integer get_accuStateRun();
		
		/*! Gets the value of the variable 'AccuStateInit' that is defined in the default interface scope. */
		const sc_integer get_accuStateInit();
		
		/*! Checks if the out event 'chargingPortEnable' that is defined in the default interface scope has been raised. */
		sc_boolean isRaised_chargingPortEnable();
		
		/*! Gets the value of the out event 'chargingPortEnable' that is defined in the default interface scope. */
		sc_integer get_chargingPortEnable_value();
		
		/*! Checks if the out event 'chargingPortDisable' that is defined in the default interface scope has been raised. */
		sc_boolean isRaised_chargingPortDisable();
		
		/*! Gets the value of the out event 'chargingPortDisable' that is defined in the default interface scope. */
		sc_integer get_chargingPortDisable_value();
		
		/*! Checks if the out event 'accuStateUpdate' that is defined in the default interface scope has been raised. */
		sc_boolean isRaised_accuStateUpdate();
		
		/*! Gets the value of the out event 'accuStateUpdate' that is defined in the default interface scope. */
		sc_integer get_accuStateUpdate_value();
		
		/*! Gets the value of the variable 'chargingCurrent_raw' that is defined in the default interface scope. */
		sc_integer get_chargingCurrent_raw();
		
		/*! Sets the value of the variable 'chargingCurrent_raw' that is defined in the default interface scope. */
		void set_chargingCurrent_raw(sc_integer value);
		
		/*! Gets the value of the variable 'accuState' that is defined in the default interface scope. */
		sc_integer get_accuState();
		
		
		//! Inner class for internal interface scope operation callbacks.
		class InternalSCI_OCB
		{
			public:
				static sc_real exponential(sc_real value);
		};
		
		void init();
		
		void enter();
		
		void exit();
		
		void runCycle();
		
		/*!
		* Checks if the state machine is active (until 2.4.1 this method was used for states).
		* A state machine is active if it has been entered. It is inactive if it has not been entered at all or if it has been exited.
		*/
		sc_boolean isActive();
		
		
		/*!
		* Checks if all active states are final. 
		* If there are no active states then the state machine is considered being inactive. In this case this method returns false.
		*/
		sc_boolean isFinal();
		
		void setTimer(TimerInterface* timer);
		
		TimerInterface* getTimer();
		
		void raiseTimeEvent(sc_eventid event);
		
		/*! Checks if the specified state is active (until 2.4.1 the used method for states was calles isActive()). */
		sc_boolean isStateActive(AccuChargerExtensionSMStates state);
	
	private:
	
		//! Inner class for internal interface scope.
		class InternalSCI
		{
			
			public:
				/*! Gets the value of the variable 'actualChargingPort' that is defined in the internal scope. */
				sc_integer get_actualChargingPort();
				
				/*! Sets the value of the variable 'actualChargingPort' that is defined in the internal scope. */
				void set_actualChargingPort(sc_integer value);
				
				/*! Gets the value of the variable 'chargingCurrent_filtered_raw' that is defined in the internal scope. */
				sc_real get_chargingCurrent_filtered_raw();
				
				/*! Sets the value of the variable 'chargingCurrent_filtered_raw' that is defined in the internal scope. */
				void set_chargingCurrent_filtered_raw(sc_real value);
				
				/*! Gets the value of the variable 'a' that is defined in the internal scope. */
				sc_real get_a();
				
				/*! Sets the value of the variable 'a' that is defined in the internal scope. */
				void set_a(sc_real value);
				
				/*! Gets the value of the variable 'chargingTime_s' that is defined in the internal scope. */
				sc_integer get_chargingTime_s();
				
				/*! Sets the value of the variable 'chargingTime_s' that is defined in the internal scope. */
				void set_chargingTime_s(sc_integer value);
				
				
			private:
				friend class StateMachine;
				sc_integer actualChargingPort;
				sc_real chargingCurrent_filtered_raw;
				sc_real a;
				sc_integer chargingTime_s;
		};
	
		//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
		static const sc_integer maxOrthogonalStates = 2;
		
		//! number of time events used by the state machine.
		static const sc_integer timeEventsCount = 3;
		TimerInterface* timer;
		sc_boolean timeEvents[timeEventsCount];
		
		AccuChargerExtensionSMStates stateConfVector[maxOrthogonalStates];
		
		sc_ushort stateConfVectorPosition;
		
		DefaultSCI iface;
		InternalSCI ifaceInternalSCI;
		
		// prototypes of all internal functions
		
		void react_filter();
		void react_off();
		void react_endOfCharging();
		void react_charging_running();
		void react__choice0_();
		void react_react__choice0_();
		void react__choice1_();
		void react__choice2_();
		void react__choice3_();
		void clearInEvents();
		void clearOutEvents();
		
};
#endif /* STATEMACHINE_H_ */
