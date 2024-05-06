#include <Arduino.h>
#include <avr/wdt.h>

//definition of roof open time and maximum delay between button clicks
const unsigned long roofOpenTime = 15000;
const unsigned long delayBetweenClick = 2000;

//definition of input/output pins
const int openButtonPin = 2;
const int closeButtonPin = 3;
const int openOutputPin = 10; 
const int closeOutputPin = 11;

//buttons flags
volatile bool openButtonPressed = false;
volatile bool closeButtonPressed = false;

//roof state flags
bool roofMoves = false;
unsigned long runTime = 0;

//buttons sequence state
unsigned int clicksOpen = 0;
unsigned int clicksClose = 0;
unsigned long lastOpenPressed = 0;
unsigned long lastClosePressed = 0;

//debounce time
unsigned long debounceDelay = 200;

//interrupt functions definitions
void openButtonISR();
void closeButtonISR();

void setup() {
  wdt_enable(WDTO_15MS); //enabling watchdog
  pinMode(openButtonPin, INPUT);
  pinMode(closeButtonPin, INPUT); 
  pinMode(openOutputPin, OUTPUT); 
  pinMode(closeOutputPin, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(openButtonPin), openButtonISR, CHANGE); // Assinging interrupt for open button
  attachInterrupt(digitalPinToInterrupt(closeButtonPin), closeButtonISR, CHANGE); // Assinging interrupt for close button
}

void loop() {
  wdt_reset(); //reset watchdog
  if(openButtonPressed) {
    clicksOpen++; //increase open sequence
    clicksClose = 0; //reset close sequence
    digitalWrite(closeOutputPin, LOW); //stops roof if it's closing
    if(clicksOpen==2){ //if clicks sequence == 2
      digitalWrite(openOutputPin, HIGH); //open roof
      runTime = millis();
      roofMoves = true;
    }
    if(clicksOpen>2){ //if clicks sequence is >2 stop roof and reset clicks sequence
      clicksOpen=0;
      digitalWrite(openOutputPin, LOW); // Włączenie pierwszej diody LED
      roofMoves = false;
    }
    lastOpenPressed = millis(); //remember last open button click time
    openButtonPressed = false;
  }
  if (closeButtonPressed) {
    clicksClose++;
    clicksOpen = 0;
    digitalWrite(openOutputPin, LOW);
    if (clicksClose==2) { 
      digitalWrite(closeOutputPin, HIGH); 
      runTime = millis();
      roofMoves = true;
    }
    if (clicksClose>2) {
      clicksClose=0;
      digitalWrite(closeOutputPin, LOW);
      roofMoves = false;
    }
    closeButtonPressed = false;
    lastClosePressed = millis();
  }
    
  if((millis()-runTime>roofOpenTime)&&roofMoves){ //if roof was opening for assigned roofOpenTime stop the roof and reset click sequences
    digitalWrite(openOutputPin, LOW);
    clicksOpen = 0;
    digitalWrite(closeOutputPin, LOW);
    clicksClose = 0;
    roofMoves = false;
  }
  if((millis()-lastOpenPressed>delayBetweenClick)&&!roofMoves)// if open button wasn't pressed for assigned time reset click squence
    clicksOpen=0;
  if((millis()-lastClosePressed>delayBetweenClick)&&!roofMoves)
    clicksClose=0;
}

void openButtonISR() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if(interrupt_time - last_interrupt_time > debounceDelay)
    openButtonPressed = digitalRead(openButtonPin);
  last_interrupt_time = interrupt_time;
}

void closeButtonISR() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if(interrupt_time - last_interrupt_time > debounceDelay)
    closeButtonPressed = digitalRead(closeButtonPin);
  last_interrupt_time = interrupt_time;
}