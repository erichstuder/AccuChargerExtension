#ifndef YAKINDUMANUALSTATECHARTTIMER_H_
#define YAKINDUMANUALSTATECHARTTIMER_H_

#include "YakinduProject.h"


class YakinduManualStatechartTimer : public TimerInterface
{
	public:

		YakinduManualStatechartTimer(sc_integer samplingtime_ms);
	
		~YakinduManualStatechartTimer();
	
		/*
		 * Starts the timing for a time event.
		 */ 
		void setTimer(TimedStatemachineInterface* statemachine, sc_eventid event, sc_integer time, sc_boolean isPeriodic);
		
		/*
		 * Unsets the given time event.
		 */
		void unsetTimer(TimedStatemachineInterface* statemachine, sc_eventid event);
	
		/*
		 * Cancel timer service. Use this to end possible timing threads and free
		 * memory resources.
		 */
		void cancel();
		
		void runCycle();
		
	private:
		YakinduManualStatechartTimer();
		
		sc_integer samplingtime_ms;
		
		struct TimeEvent {
			TimedStatemachineInterface* statemachine;
			sc_eventid eventId;
			sc_integer time_ms;
			sc_boolean isPeriodic;
			sc_integer remainingTime_ms;
			sc_boolean isActive;
			struct TimeEvent* nextTimeEvent = null;
			
			TimeEvent(
				TimedStatemachineInterface* statemachine,
				sc_eventid eventId,
				sc_integer time_ms,
				sc_boolean isPeriodic,
				sc_integer remainingTime_ms,
				sc_boolean isActive,
				struct TimeEvent* nextTimeEvent){
					setValues(
						statemachine,
						eventId,
						time_ms,
						isPeriodic,
						remainingTime_ms,
						isActive,
						nextTimeEvent
					);
				}
			
			void setValues(
				TimedStatemachineInterface* statemachine,
				sc_eventid eventId,
				sc_integer time_ms,
				sc_boolean isPeriodic,
				sc_integer remainingTime_ms,
				sc_boolean isActive,
				struct TimeEvent* nextTimeEvent){
					this->statemachine = statemachine;
					this->eventId = eventId;
					this->time_ms = time_ms;
					this->isPeriodic = isPeriodic;
					this->remainingTime_ms = remainingTime_ms;
					this->isActive = isActive;
					this->nextTimeEvent = nextTimeEvent;
				}
		};
		
		TimeEvent* firstTimeEvent = null;

		
		
};
#endif /* YAKINDUMANUALSTATECHARTTIMER_H_ */
