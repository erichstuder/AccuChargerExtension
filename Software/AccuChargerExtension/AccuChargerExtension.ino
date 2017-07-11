#include "yakindu/AccuChargerExtensionSM/generated_code/StateMachine.cpp"
#include "YakinduManualStatechartTimer.h"

#include <LiquidCrystal.h>

/*
TODOs
- abklären, wie das mit NULL und null genau läuft. Wert? NULL oder null?
  - Timer_Counter1.cpp prüfung auf Null-Pointer bei den Callbacks
- nach ca. 1h soll der nächste Akku geladen werden. Datatype grösse 16bit lässt nur etwa 30s zu. Was hat leonardo?
*/

//sc_integer samplingtime_ms = 5;
#define RELAY_ENABLE LOW
#define RELAY_DISABLE HIGH
const int relayPinNumbers[] = {2,3,4,5}; //{9,2,3,4,5,6,7,8};//pin 1 is used by serial communication //9 kommt als erstes weil ich keine Lust hatte zum Umstecken auf dem Board nachdem pin 1 wegfiel
const int chargingCurrentPin = A0;
const int preChargeVoltagePin = A1;
//const int preChargeSwitchPin = 10; 

StateMachine* stateMachine;
StateMachine::DefaultSCI* sciInterface;
YakinduManualStatechartTimer* timer;
sc_integer accuState;

LiquidCrystal lcd(12, 11, 9, 8, 7, 6);

void printLCD(int chargingPortNr, int accuState)
{
    char* stateText = "";

    if(accuState == stateMachine->get_accuStateUndef())
    {
      stateText = "undef"; 
    }
    else if(accuState == stateMachine->get_accuStateFull())
    {
      stateText = "full"; 
    }
    else if(accuState == stateMachine->get_accuStateRun())
    {
      stateText = "run"; 
    } 
    else
    {
      //should never happen
      stateText = "????"; 
    } 

    switch(chargingPortNr)
    {
      case 0:
        lcd.setCursor(0, 0);
        lcd.print("        ");
        lcd.setCursor(0, 0);
        lcd.print("0:");
        break;
      case 1:
        lcd.setCursor(8, 0);
        lcd.print("        ");
        lcd.setCursor(8, 0);
        lcd.print("1:");
        break;
      case 2:
        lcd.setCursor(0, 1);
        lcd.print("        ");
        lcd.setCursor(0, 1);
        lcd.print("2:");
        break;
      case 3:
        lcd.setCursor(8, 1);
        lcd.print("        ");
        lcd.setCursor(8, 1);
        lcd.print("3:");
        break;
      default:
        break;  
    }
    lcd.print(stateText);
}

void setup(){
  //power for the display
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  delay(1000);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  printLCD(0, stateMachine->get_accuStateUndef());
  printLCD(1, stateMachine->get_accuStateUndef());
  printLCD(2, stateMachine->get_accuStateUndef());
  printLCD(3, stateMachine->get_accuStateUndef());

  //
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

void loop(){
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
  
  if( stateMachine->isRaised_accuStateUpdate()){
    sc_integer chargingPortNr = stateMachine->get_accuStateUpdate_value();
    sc_integer accuState = stateMachine->get_accuState();//this is not very clean but i found no other possibility to hand over this information.
    
    printLCD(chargingPortNr, accuState);
//    
//    char* stateText = "";
//
//    if(accuState == stateMachine->get_accuStateUndef())
//    {
//      stateText = "undef"; 
//    }
//    else if(accuState == stateMachine->get_accuStateFull())
//    {
//      stateText = "full"; 
//    }
//    else if(accuState == stateMachine->get_accuStateRun())
//    {
//      stateText = "run"; 
//    } 
//    else
//    {
//      //should never happen
//      stateText = "????"; 
//    } 
//
//    switch(chargingPortNr)
//    {
//      case 0:
//        lcd.setCursor(0, 0);
//        lcd.print("        ");
//        lcd.setCursor(0, 0);
//        lcd.print("0:");
//        break;
//      case 1:
//        lcd.setCursor(8, 0);
//        lcd.print("        ");
//        lcd.setCursor(8, 0);
//        lcd.print("1:");
//        break;
//      case 2:
//        lcd.setCursor(0, 1);
//        lcd.print("        ");
//        lcd.setCursor(0, 1);
//        lcd.print("2:");
//        break;
//      case 3:
//        lcd.setCursor(8, 1);
//        lcd.print("        ");
//        lcd.setCursor(8, 1);
//        lcd.print("3:");
//        break;
//      default:
//        break;  
//    }
//    lcd.print(stateText);
  }
}
