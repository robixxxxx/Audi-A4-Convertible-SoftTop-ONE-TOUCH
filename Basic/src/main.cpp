#include <Arduino.h>
#include <avr/wdt.h>

//definition of delays
const unsigned long roofOpenTime = 30000;
const unsigned long delayBetweenClick = 800;

unsigned long debounceDelay = 50;

//definition of input/output pins
const int inputPinOpen = 2;
const int inputPinClose = 3;
const int outputPinOpen = 4; 
const int outputPinClose = 5;
volatile bool openButtonPressed = false;
volatile bool closeButtonPressed = false; 

//buttons sequence state
unsigned int clicksOpen = 0;
unsigned int clicksClose = 0;
unsigned long lastOpenPressed = 0;
unsigned long lastClosePressed = 0;

//roof state
bool roofMoves = false;
unsigned long runTime = 0;

void chceckRoofVars();
void initRoof();
void closeButtonISR();
void openButtonISR();
void checkButtonsState();
void checkButtonSequenceTime();
void roofCloseSequence();
void roofOpenSequence();
void checkMoveTime();

void setup() {
  wdt_enable(WDTO_15MS);
  initRoof();  
}

void loop() {
  wdt_reset();
  chceckRoofVars();
}

void checkMoveTime(){
    if((millis()-runTime>roofOpenTime)&&roofMoves){ //if roof was opening for assigned roofOpenTime stop the roof and reset click sequences
    digitalWrite(outputPinOpen, LOW);
    clicksOpen = 0;
    digitalWrite(outputPinClose, LOW);
    clicksClose = 0;
    roofMoves = false;
    }
}

void roofOpenSequence(){
      clicksOpen++; //increase open sequence
    clicksClose = 0; //reset close sequence
    digitalWrite(outputPinClose, LOW); //stops roof if it's closing
    if(clicksOpen==2){ //if clicks sequence == 2
      digitalWrite(outputPinOpen, HIGH); //open roof
      runTime = millis();
      roofMoves = true;
    }
    if(clicksOpen>2){ //if clicks sequence is >2 stop roof and reset clicks sequence
      clicksOpen=0;
      digitalWrite(outputPinOpen, LOW); // Włączenie pierwszej diody LED
      roofMoves = false;
    }
    lastOpenPressed = millis(); //remember last open button click time
    openButtonPressed = false;
}

void roofCloseSequence(){
  clicksClose++;
    clicksOpen = 0;
    digitalWrite(outputPinOpen, LOW);
    if (clicksClose==2) { 
      digitalWrite(outputPinClose, HIGH); 
      runTime = millis();
      roofMoves = true;
    }
    if (clicksClose>2) {
      clicksClose=0;
      
      digitalWrite(outputPinClose, LOW);
      roofMoves = false;
    }
    closeButtonPressed = false;
    lastClosePressed = millis();
}

void checkButtonSequenceTime(){
    if((millis()-lastOpenPressed>delayBetweenClick)&&!roofMoves)// if open button wasn't pressed for assigned time reset click squence
    clicksOpen=0;
    if((millis()-lastClosePressed>delayBetweenClick)&&!roofMoves)
    clicksClose=0;
}

void checkButtonsState(){
    if(openButtonPressed) {
        roofOpenSequence();
    }
    if (closeButtonPressed) {
        roofCloseSequence();
    }
}

void openButtonISR() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if(interrupt_time - last_interrupt_time > debounceDelay)
    openButtonPressed = !digitalRead(inputPinOpen);
  last_interrupt_time = interrupt_time;
}

void closeButtonISR() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if(interrupt_time - last_interrupt_time > debounceDelay){
    closeButtonPressed = !digitalRead(inputPinClose);      
  }                                                                                                                                                                                 
  last_interrupt_time = interrupt_time;
}

void initRoof(){
  pinMode(inputPinOpen, INPUT_PULLUP);
  pinMode(inputPinClose, INPUT_PULLUP); 
  pinMode(outputPinOpen, OUTPUT); 
  pinMode(outputPinClose, OUTPUT);
  digitalWrite(outputPinOpen, LOW);
  digitalWrite(outputPinClose, LOW);
  attachInterrupt(digitalPinToInterrupt(inputPinOpen), openButtonISR, CHANGE); // Assinging interrupt for open button
  attachInterrupt(digitalPinToInterrupt(inputPinClose), closeButtonISR, CHANGE); // Assinging interrupt for close button
}

void chceckRoofVars(){
    checkButtonsState();
    checkMoveTime();
    checkButtonSequenceTime();
}