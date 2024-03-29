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
#include <libfakecatnap/hw.h>

#define BIT_FLIP(port,bit) \
	P##port##OUT |= BIT##bit; \
	P##port##DIR |= BIT##bit; \
	P##port##OUT &= ~BIT##bit; \

void starter();
void blink();
void blink2();
void burn();

STARTER_EVT(starter);

DEC_EVT(blink,blink,8000,PERIODIC);
DEC_EVT(blink2,blink2,7000,PERIODIC);
DEC_TSK(burn,burn);

void app_hw_init() {
  return;
}

void starter() {
  BIT_FLIP(1,1);
  PRINTF("off to the races!\r\n");
  add_event(&EVENT(blink));
  //add_event(&EVENT(blink2));
  push_task(&TASK(burn));
  dec_event(&EVT_FCN_STARTER);
  EVENT_RETURN();
}

void blink() { //16000
  LCFN_INTERRUPTS_DISABLE;
  PRINTF("Blinking!\r\n");
  LCFN_INTERRUPTS_ENABLE;
  EVENT_RETURN();
}

void blink2() { //4000
  LCFN_INTERRUPTS_DISABLE;
  PRINTF("Still blinking!\r\n");
  LCFN_INTERRUPTS_ENABLE;
  EVENT_RETURN();
}


void burn() {
  while(1) {
    for (int k = 1; k < 0xff; k++) {
      for( int j = 0; j < 0xff; j++) {
        for(int i = 0; i < 0xff; i++) {
          volatile uint16_t temp = i;
        }
      }
      LCFN_INTERRUPTS_DISABLE;
      PRINTF("\t\tTasking %u\r\n",k);
      LCFN_INTERRUPTS_ENABLE;
    }
  }
  return;
}

