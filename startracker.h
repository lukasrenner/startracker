#ifndef STARTRACKER_H
#define STARTRACKER_H

// The starting radius in mm, i.e. the intersection of the threaded rod with the baseplate at phi = 0
#define R_0 (200)

// The distance of the pivot points from the baseplate in mm
#define D_CORR (80)

// The maximum allowed length in mm, i.e. the length of the threaded rod. This is used to prevent the motor from turning, when the end is reached. Can be used to maybe play a sound for notification
#define L_MAX (200)

// Conversion factor to calculate the frequency of steps for a given L and R (corrected). Unit is steps / (second * meter)
#define C_F (29.16846F)

// How much the length of the rod increases per step. Unit is  meter / step
#define INC_STEP (0.0000025)

// Frequency of TIMER1
#define FREQ_TIMER1 (16000000 / 1024)

// Time that one rotation should take (for debugging)
#define T_DEBUG (10)

// Steps per rotation (with microsteps)
#define STEPS_PER_ROT (3200)

// Debugging
#define ICR1_DEBUG ((uint16_t)((T_DEBUG * FREQ_TIMER1 / STEPS_PER_ROT)))

// Steps per second when rewinding (3200 steps = 1 rotation)
#define STEPS_PER_SEC_RW (6200)

// Time that one rotation should take (for debugging)
#define T_REWIND (1)

// Counts to be loaded into register ICR1 when rewinding
#define ISR1_REWIND ((uint16_t)((T_REWIND * FREQ_TIMER1 / STEPS_PER_ROT)))

// Max number of steps. Should not be exceeded when tracking
#define MAX_STEPS (10000)

// ----------- defines for timer mode ---------------

#define BIT_WGM10 (0<<WGM10)
#define BIT_WGM11 (1<<WGM11)
#define BIT_WGM12 (1<<WGM12)
#define BIT_WGM13 (1<<WGM13)
#define BIT_COM1A0 (0<<COM1A0)
#define BIT_COM1A1 (1<<COM1A1)
#define BIT_CS10 (1<<CS10)
#define BIT_CS11 (0<<CS11)
#define BIT_CS12 (1<<CS12)
#define BIT_OCIE1A (1<<OCIE1A)


// Output pin for step pulse
#define BIT_OUT_STEP (1 << PORTB1)
#define BIT_DIR_STEP (1 << DDB1)

// Output pin for direction
#define BIT_OUT_DIR (1 << PORTC4)
#define BIT_DIR_DIR (1 << DDC4)

// ---------- defines for buttons and stuff -----------------


// ----- old! -----
#define BIT_OUT_LED (1<< PORTB5)
#define BIT_DIR_LED (1 << DDB5)
#define BIT_PIN_BUTTON1 (1 << PINB4)
#define BIT_DIR_BUTTON1 (1 << DDB4)
#define BIT_PIN_BUTTON2 (1 << PINC1)
#define BIT_DIR_BUTTON2 (1 << DDC1)

typedef enum state_t {
		      IDLE,
		      TRACKING,
		      REWIND
} state_t;

typedef enum rotDir_t {
  OPEN = 0,
  CLOSE = 1
} rotDir_t;

#endif
