//#include <Arduino.h>/////////////////debug
#include <stdlib.h>

#include "YakinduManualStatechartTimer.h"


YakinduManualStatechartTimer::YakinduManualStatechartTimer(){}

YakinduManualStatechartTimer::YakinduManualStatechartTimer(sc_integer samplingtime_ms){
	//samplingtime: with this time period the timer has to be called
	this->samplingtime_ms = samplingtime_ms;
}

		
YakinduManualStatechartTimer::~YakinduManualStatechartTimer(){
	this->cancel();
}

/*
 * Starts the timing for a time event.
 */ 
void YakinduManualStatechartTimer::setTimer(TimedStatemachineInterface* statemachine, sc_eventid event, sc_integer time, sc_boolean isPeriodic){
	if (firstTimeEvent == null){
		
		//firstTimeEvent = (TimeEvent*) malloc(sizeof(TimeEvent));
		//*firstTimeEvent = {statemachine, event, time, isPeriodic, time, true, null};
		
		firstTimeEvent = new TimeEvent(
			statemachine,
			event,
			time,
			isPeriodic,
			time,
			true,
			null
		);
		
	}else{	
		TimeEvent* actualTimeEvent = firstTimeEvent;
		sc_boolean timerSet = false;
		while(!timerSet){
			if(!actualTimeEvent->isActive){
				//actualTimeEvent = {statemachine, event, time, isPeriodic, time, true, actualTimerEvent.nextTimeEvent};
				
				// actualTimeEvent->statemachine = statemachine;
				// actualTimeEvent->eventId = event;
				// actualTimeEvent->time_ms = time;
				// actualTimeEvent->isPeriodic = isPeriodic;
				// actualTimeEvent->remainingTime_ms = time;
				// actualTimeEvent->isActive = true;
				// actualTimeEvent->nextTimeEvent = null;
				
				actualTimeEvent->setValues(
					statemachine,
					event,
					time,
					isPeriodic,
					time,
					true,
					actualTimeEvent->nextTimeEvent
				);
		
				timerSet = true;
			}else if(actualTimeEvent->nextTimeEvent == null){
				
				//old actualTimeEvent->nextTimeEvent = (TimeEvent*) malloc(sizeof(TimeEvent));
				//old *(actualTimeEvent->nextTimeEvent) = {statemachine, event, time, isPeriodic, time, true, null};
				actualTimeEvent->nextTimeEvent = new TimeEvent(
					statemachine,
					event,
					time,
					isPeriodic,
					time,
					true,
					null
				);
				
				timerSet = true;
			}else{
				actualTimeEvent = actualTimeEvent->nextTimeEvent;
			}
		}
	}
}

/*
 * Unsets the given time event.
 */
void YakinduManualStatechartTimer::unsetTimer(TimedStatemachineInterface* statemachine, sc_eventid event){
  (void)statemachine; //Dummy access to suppress "unused parameter" warning.
  
	TimeEvent* actualTimeEvent = firstTimeEvent;
	sc_boolean timerUnset = false;
	sc_boolean endReached = false;
	
	while(!timerUnset && !endReached ){
		if(actualTimeEvent->eventId == event){
      
			actualTimeEvent->isActive = false;
			timerUnset = true;
		}else if(actualTimeEvent->nextTimeEvent == null){
			endReached = true;
		}else{
			actualTimeEvent = actualTimeEvent->nextTimeEvent;
		}
	}
	
	//note: if endReached is true, then probably an error happened!
}


/*
 * Cancel timer service. Use this to end possible timing threads and free
 * memory resources.
 */
void YakinduManualStatechartTimer::cancel(){
	TimeEvent* actualTimeEvent = firstTimeEvent;
	TimeEvent* nextTimeEvent = null;
	sc_boolean endReached = false;
	while(!endReached){
		if(actualTimeEvent != null){
			nextTimeEvent = actualTimeEvent->nextTimeEvent;
			free(actualTimeEvent);
			actualTimeEvent = nextTimeEvent;
		}else{
			endReached = true;
		}
	}	
}


void YakinduManualStatechartTimer::runCycle(){
	if(firstTimeEvent != null){
		
		TimeEvent* actualTimeEvent = firstTimeEvent;
		sc_boolean endReached = false;
		while(!endReached){
      
			if(actualTimeEvent->isActive){
				actualTimeEvent->remainingTime_ms -= this->samplingtime_ms;
				if(actualTimeEvent->remainingTime_ms <= 0){
					actualTimeEvent->statemachine->raiseTimeEvent(actualTimeEvent->eventId);
					if(actualTimeEvent->isPeriodic){
						actualTimeEvent->remainingTime_ms = actualTimeEvent->time_ms;
					}else{
						actualTimeEvent->isActive = false;
					}
				}
			}
			
			if(actualTimeEvent->nextTimeEvent == null){
				endReached = true;
			}else{
				actualTimeEvent = actualTimeEvent->nextTimeEvent;
			}
		}
	}
}
