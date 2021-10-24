
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#include "startracker.h"
#include "debounce.h"


// current state of the startracker
state_t trackerState;

//  total number of steps performed. Steps in the closing direction are subtracted
uint32_t currentSteps;

// distance that the rod has travelled (i.e. the length) in mm (this is our x in f(x) = ...)
float currentLength;

void updateStepFreq() {
  // update length
  currentLength = currentSteps * INC_STEP;
  // calc radius correction
  float corr = D_CORR * currentLength/(sqrt(4*(uint16_t)R_0*R_0 - currentLength*currentLength));
  float corrR = R_0 - corr;
  // calc step frequency
  float fStep = C_F * corrR * (1 - (currentLength*currentLength/(4*corrR*corrR)));
  // write to register ISR1
  ICR1 = FREQ_TIMER1 / fStep;
  // set register OCR1A accordingly
  OCR1A = ICR1 / 2;
}

void turnOnOff(){
  TCCR1B ^= BIT_CS10 | BIT_CS12;
}

void turnOn(){
  TCCR1B |= BIT_CS10 | BIT_CS12;
}

void turnOff(){
  TCCR1B &= ~(BIT_CS10 | BIT_CS12);
}


void stopMoving() {
  trackerState = IDLE;
  turnOff();
}

void changeDir(){
  PORTC ^= BIT_OUT_DIR;
}

void toggleLed(){
  PORTB ^= BIT_OUT_LED;
}

void setDir(rotDir_t dir) {

  if (dir == OPEN) {
    PORTC |= BIT_OUT_DIR;
  } else {
    PORTC &= ~BIT_OUT_DIR;
  }
  return;
}

void startTracking() {
  
  if (trackerState != TRACKING) {
    // debugging
    TCNT1 = 0;
    updateStepFreq();
    trackerState = TRACKING;
    setDir(OPEN);
    turnOn();
  }
  return;
}

void rewind(){
  
  if (trackerState != REWIND) {
    // change the speed when rewinding
    TCNT1 = 0;
    ICR1 = ICR1_DEBUG;
    OCR1A = ICR1 / 2;
    trackerState = REWIND;
    setDir(CLOSE);
    turnOn();
  }
  return;
}

void setup(){

  trackerState = IDLE;
  currentSteps = 0u;

  // turn on interrupts
  sei();
  // Configure timer 1
  TCCR1A = BIT_WGM11 | BIT_COM1A0 | BIT_COM1A1;
  TCCR1B = BIT_WGM12 | BIT_WGM13;
  // enable interrupt on compare match OCR1A
  TIMSK1 |= BIT_OCIE1A;
  // configure interrupt
  


  // outputs
  DDRC = BIT_DIR_DIR;
  DDRB |= BIT_DIR_STEP;
  DDRB |= BIT_DIR_LED;
  
  DDRB &= ~BIT_DIR_BUTTON1;
  PORTB &= ~BIT_PIN_BUTTON1;
  
  // set up input and debouncing
  setupDebounce();

  registerFuncButtonDown(&startTracking, 4u);
  registerFuncButtonUp(&rewind, 4u);
}


ISR(TIMER1_COMPA_vect, ISR_BLOCK) {

  if (trackerState == TRACKING) {
    if (currentSteps < MAX_STEPS) {   
      if (currentSteps % 100 == 0u)
        updateStepFreq();
      currentSteps++;
    } else
      stopMoving();
  } else if (trackerState == REWIND) {
    if (currentSteps > 0)
      currentSteps--;
     else
       stopMoving();
  }
}


int main(){

  setup();
  
  while (1) {
    handleButtonPresses();
 
  };

  return 0;
}
