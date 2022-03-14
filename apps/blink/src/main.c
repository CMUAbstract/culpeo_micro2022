#include <stdlib.h>
#include <stdint.h>
//#include <string.h>
#include <stdio.h>

// Built-in libraries for the msp430
#include <msp430.h>
#include <libmsp/clock.h>
#include <libmsp/watchdog.h>
#include <libmsp/gpio.h>
#include <libmsp/mem.h>
#include <libio/console.h>
// Functions for the capybara board
#include <libcapybara/board.h>

// Catnap stand in
#include <libfakecatnap/catnap.h>
#include <libfakecatnap/events.h>
#include <libfakecatnap/tasks.h>

#define BIT_FLIP(port,bit) \
	P##port##OUT |= BIT##bit; \
	P##port##DIR |= BIT##bit; \
	P##port##OUT &= ~BIT##bit; \


void starter();
void blink();
void blink2();

STARTER_EVT(starter);

DEC_EVT(blink,blink,30000); //TODO translation from seconds to ticks
DEC_EVT(blink2,blink2,7000); //TODO translation from seconds to ticks


void starter() {
  BIT_FLIP(1,1);
  PRINTF("off to the races!\r\n");
  add_event(&EVENT(blink));
  add_event(&EVENT(blink2));
  dec_event(&EVT_FCN_STARTER);
  EVENT_RETURN();
}

void blink() {
  PRINTF("Blinking!\r\n");
  EVENT_RETURN();
}

void blink2() {
  PRINTF("Still blinking!\r\n");
  EVENT_RETURN();
}

