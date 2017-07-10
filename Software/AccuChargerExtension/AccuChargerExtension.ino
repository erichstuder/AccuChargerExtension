//#define int long

#include "yakindu/AccuChargerExtensionSM/generated_code/StateMachine.cpp"
#include "YakinduManualStatechartTimer.h"

//#include "MemoryFree.h" 


/*
TODOs
- abklären, wie das mit NULL und null genau läuft. Wert? NULL oder null?
  - Timer_Counter1.cpp prüfung auf Null-Pointer bei den Callbacks
- Tasking so machen, das es exakt z.B. 5ms beträgt.
- nach ca. 1h soll der nächste Akku geladen werden. Datatype grösse 16bit lässt nur etwa 30s zu. Was hat leonardo?

*/

//sc_integer samplingtime_ms = 5;
#define RELAY_ENABLE LOW
#define RELAY_DISABLE HIGH
const int relayPinNumbers[] = {9,2,3,4,5,6,7,8};//pin 1 is used by serial communication //9 kommt als erstes weil ich keine Lust hatte zum Umstecken auf dem Board nachdem pin 1 wegfiel
const int chargingCurrentPin = A0;
const int preChargeVoltagePin = A1;
const int preChargeSwitchPin = 10; 

StateMachine* stateMachine;
StateMachine::DefaultSCI* sciInterface;
YakinduManualStatechartTimer* timer;
sc_integer chargedPorts;

void setup() {
  // setup serial
//  Serial1.begin(9600);
  //while (!Serial) {
    //; // wait for serial port to connect. Needed for native USB
  //}


  //
  chargedPorts = 0;
  stateMachine = new StateMachine();
  stateMachine->init(); //initializes variables (e.g. samplingTime)
  timer = new YakinduManualStatechartTimer(stateMachine->get_samplingTime_ms());
  stateMachine->setTimer(timer);
  stateMachine->enter();

  for(int n=0; n<stateMachine->get_nrOfChargingPorts(); n++){ 
    pinMode(relayPinNumbers[n], OUTPUT);
    digitalWrite(relayPinNumbers[n], RELAY_DISABLE);
  }

  setupTimer();
}

void loop() {
  if(Serial){
    Serial.println("-");
    Serial.println(String(chargedPorts));
  }
  sc_integer temp = chargedPorts;
  for(int n=0; n<16; n++){
    if((temp & 0x01) && Serial){
      Serial.print("port ");
      Serial.print(String(n));
      Serial.println(" : full");
    }
    temp >>= 1;
  } 
  delay(1000);//1s
}

void setupTimer(){
  TCCR1A = 0; //for any reason, this must be done!!
  TCCR1B = _BV(WGM12) | _BV(CS11) | _BV(CS10); //match on value of OCR1A and divide clock by 64
  OCR1A = 1250; //5ms
  TIMSK1 = _BV(OCIE1A); //enable interrupt
}

ISR(TIMER1_COMPA_vect){
  timer->runCycle();
  stateMachine->set_chargingCurrent_raw(analogRead(chargingCurrentPin));
  stateMachine->set_preChargeVoltage_raw(analogRead(preChargeVoltagePin));
  stateMachine->runCycle();
  
  sc_integer value;
  
  if( stateMachine->isRaised_chargingPortDisable()){
    value = stateMachine->get_chargingPortDisable_value();
    if(value >= 0 && value <=7){
      digitalWrite(relayPinNumbers[value], RELAY_DISABLE);
    }else if(value == -1){
      for(int n=0; n<stateMachine->get_nrOfChargingPorts(); n++){
        digitalWrite(relayPinNumbers[n], RELAY_DISABLE);  
      } 
    }
  }
  
  if( stateMachine->isRaised_chargingPortEnable()){
    value = stateMachine->get_chargingPortEnable_value();
    if(value >= 0 && value <=7){
      //digitalWrite(13, 1);
      digitalWrite(relayPinNumbers[value], RELAY_ENABLE);
    }
  }
  
  if( stateMachine->isRaised_chargedPortsUpdate()){
    chargedPorts = stateMachine->get_chargedPortsUpdate_value();
  }
  
  if( stateMachine->isRaised_preChargeUpdate()){
    if( stateMachine->get_preChargeUpdate_value() == true){
      digitalWrite(preChargeSwitchPin, 1);
    }else{
      digitalWrite(preChargeSwitchPin, 0);
    }
  }
  

  
  //TODO: make it dependant of the samplingTime.
  //delay(stateMachine->get_samplingTime_ms());
}
