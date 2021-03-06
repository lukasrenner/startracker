#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <avr/io.h>

/* how often to check the state of the input port */
#define NUM_CHECKS (5u)

/* input port */
#define INPORT (PINB)

/* delay between each check */
#define TIME_BETWEEN_CHECKS_MS (10)

/* which timer to use, indicated by its number (as in the datasheet) */
#define TIMERNUM 0

/* Timer prescaler */
#define PRESCALER_TIMER 1024

/* Frequency of the timer */
#define FREQ_TIMER (16000000 / PRESCALER_TIMER)

/* TOP of the timer, stored in OCRXA
   corresponds to the the delay between checks */
#define TIMERTOP (TIME_BETWEEN_CHECKS_MS * FREQ_TIMER / 1000)

#define CONCAT2_REG_OCRXA(a) (OCR ## a ## A)
#define CONCAT1_REG_OCRXA(a) CONCAT2_REG_OCRXA(a)
#define OCRXA CONCAT1_REG_OCRXA(TIMERNUM)

#define CONCAT2_REG_TIMSKX(a) (TIMSK ## a)
#define CONCAT1_REG_TIMSKX(a) CONCAT2_REG_TIMSKX(a)
#define TIMSKX CONCAT1_REG_TIMSKX(TIMERNUM)

#define CONCAT2_REG_OCIEXA(a) (OCIE ## a ## A)
#define CONCAT1_REG_OCIEXA(a) CONCAT2_REG_OCIEXA(a)
#define OCIEXA CONCAT1_REG_OCIEXA(TIMERNUM)

#define CONCAT2_REG_TCCRA(a) (TCCR ## a ## A)
#define CONCAT1_REG_TCCRA(a) CONCAT2_REG_TCCRA(a)
#define REG_TCCRA CONCAT1_REG_TCCRA(TIMERNUM)

#define CONCAT2_REG_TCCRB(a) (TCCR ## a ## B)
#define CONCAT1_REG_TCCRB(a) CONCAT2_REG_TCCRB(a)
#define REG_TCCRB CONCAT1_REG_TCCRB(TIMERNUM)

#define CONCAT2_REG_TCNT(a) (TCNT ## a)
#define CONCAT1_REG_TCNT(a) CONCAT2_REG_TCNT(a)
#define REG_TCNT CONCAT1_REG_TCNT(TIMERNUM)

#define CONCAT2_REG_TIFR(a) (TIFR ## a)
#define CONCAT1_REG_TIFR(a) CONCAT2_REG_TIFR(a)
#define REG_TIFR CONCAT1_REG_TIFR(TIMERNUM)

#define CONCAT2_REG_OCRA(a) (OCR ## a ## A)
#define CONCAT1_REG_OCRA(a) CONCAT2_REG_OCRA(a)
#define REG_OCRA CONCAT1_REG_OCRA(TIMERNUM)

#define CONCAT2_REG_OCRB(a) (OCR ## a ## B)
#define CONCAT1_REG_OCRB(a) CONCAT2_REG_OCRB(a)
#define REG_OCRB CONCAT1_REG_OCRB(TIMERNUM)

#define CONCAT2_ISR_TIM_OVF(a) (TIMER ## a ## _OVF_vect)
#define CONCAT1_ISR_TIM_OVF(a) CONCAT2_ISR_TIM_OVF(a)
#define TIM_OVF_ISR CONCAT1_ISR_TIM_OVF(TIMERNUM)

#define CONCAT2_ISR_TIM_COMPA(a) (TIMER ## a ## _COMPA_vect)
#define CONCAT1_ISR_TIM_COMPA(a) CONCAT2_ISR_TIM_COMPA(a)
#define TIM_COMPA_ISR CONCAT1_ISR_TIM_COMPA(TIMERNUM)

typedef void (*buttonFunc_t)();

void setupDebounce();
uint8_t isButtonPressed(uint8_t buttonNum);
uint8_t getButtonPresses();
void registerFuncButtonPress(buttonFunc_t pFunc, uint8_t buttonNum);
void registerFuncButtonDown(buttonFunc_t pFunc, uint8_t buttonNum);
void registerFuncButtonUp(buttonFunc_t pFunc, uint8_t buttonNum);
void handleButtonPresses();
#endif
