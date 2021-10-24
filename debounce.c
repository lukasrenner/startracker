#include "debounce.h"
#include <avr/interrupt.h>

uint8_t checkIndex = 0;
uint8_t state[NUM_CHECKS] = {0};
/* state of the input port after debouncing */
volatile uint8_t debouncedState = 0;
/* holds the previous state of the input port
   a 1 indicated that is was pressed */
uint8_t previousState = 0;
/* a 1 in this byte indicates that this button has been pressed.
   updated everytime debouncePort is called
   has to be cleared everytiem the application checks for presses */
volatile uint8_t buttonPresses = 0;

/* similar to above, 1 indicates that button is down (switch closed) */
volatile uint8_t buttonDown = 0;

/* similar to above, 1 indicates that button is up (switch open) */
volatile uint8_t buttonUp = 0;

/* array of pointers to functions that will be called
   when the corresponding button is pushed */
buttonFunc_t buttonFuncs[8] = {0};

/* array of pointers to functions that will be called
   when the corresponding button is up (switch open) */
buttonFunc_t buttonUpFuncs[8] = {0}; 

/* array of pointers to functions that will be called
   when the corresponding button is down (switch closed) */
buttonFunc_t buttonDownFuncs[8] = {0}; 

void setupDebounce(){

  if(TIMERNUM == 1){
    REG_TCCRB = (1<<WGM12) | (1<<CS12) | (1<<CS10);
  }else if(TIMERNUM == 0){
    REG_TCCRA = (1<<WGM01);
    REG_TCCRB = (1<<CS02) | (1<<CS00);
  }else{
    REG_TCCRA = (1<<WGM21);
    REG_TCCRB = (1<<CS22) | (1<<CS21) | (1<<CS20);
  }
  OCRXA =  TIMERTOP;

  TIMSKX |= (1<<OCIEXA);
  return;
}

static void debouncePort(){
  uint8_t i;
  uint8_t tmpState = 0xFF;
  uint8_t invTmpState = 0xFF;
  
  /* save last debounced state */
  previousState = debouncedState;

  /* get input */
  state[checkIndex] = INPORT;
  checkIndex++;

  /* perform debouncing and save state */
  for(i = 0; i < NUM_CHECKS; i++){
    tmpState = tmpState & state[i];
    invTmpState = invTmpState & ~(state[i]);
  }
  debouncedState = tmpState;
  buttonUp = invTmpState;
  buttonDown = tmpState;

  /* check for buttonpresses, i.e. transition from 
     pressed to not-pressed and save.
     This variable is reset below. */
  buttonPresses |= (previousState ^ debouncedState) & previousState;
  
  /* update index with wraparound */
  if(checkIndex >= NUM_CHECKS){
    checkIndex = 0;
  }
  return;
}

uint8_t isButtonPressed(uint8_t buttonNum){
  uint8_t tmp = buttonPresses & (1 << buttonNum);
  buttonPresses &= ~(1 << buttonNum);
  return tmp;
}

uint8_t getButtonPresses(){
  uint8_t tmp = buttonPresses;
  buttonPresses = 0;
  return tmp;
}

ISR(TIM_COMPA_ISR){
  debouncePort();
  return;
}

void registerFuncButtonPress(buttonFunc_t pFunc, uint8_t buttonNum){
  buttonFuncs[buttonNum] = pFunc;
  return;
}

void registerFuncButtonDown(buttonFunc_t pFunc, uint8_t buttonNum){
  buttonDownFuncs[buttonNum] = pFunc;
  return;
}

void registerFuncButtonUp(buttonFunc_t pFunc, uint8_t buttonNum){
  buttonUpFuncs[buttonNum] = pFunc;
  return;
}

void handleButtonPresses(){


  for(uint8_t i=0; i < sizeof(uint8_t) * 8; i++){

    // handle button presses
    if((buttonFuncs[i] != 0) && ((buttonPresses & (1 << i)) != 0)){
      buttonFuncs[i]();
    }

    // handle functions to be executed when a button is down
    if((buttonDownFuncs[i] != 0) && ((buttonDown & (1 << i)) != 0)){
      buttonDownFuncs[i]();
    }

    // handle functions to be executed when a button is up
    if((buttonUpFuncs[i] != 0) && ((buttonUp & (1 << i)) != 0)){
      buttonUpFuncs[i]();
    }

  }
  //  if(buttonPresses) buttonFuncs[1]();
  buttonPresses = 0;

  return;
}

