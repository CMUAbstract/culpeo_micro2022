#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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
#include <libradio/radio.h>

// Catnap stand in
#include <libfakecatnap/catnap.h>
#include <libfakecatnap/events.h>
#include <libfakecatnap/tasks.h>
#include <libfakecatnap/hw.h>

// App specifcis
#include "rsa.h"

#define BIT_FLIP(port,bit) \
	P##port##OUT |= BIT##bit; \
	P##port##DIR |= BIT##bit; \
	P##port##OUT &= ~BIT##bit; \

#define PKT_LEN 32

//TODO put these functions somewhere else


void starter();
void radio();
void encrypt();
void sense();
void extra_sense();

STARTER_EVT(starter);

DEC_EVT(radio,radio,0); //Deschedules itself
DEC_EVT(encrypt,encrypt,0);//Deschedules itself
DEC_EVT(sense,sense,8000);
DEC_TSK(extra_sense,extra_sense);

uint16_t min_reading = 0xFFFF;

char __nv pkt[256] =  "hello world! we're going to the moon! we're going to the \
moon!  hello world! we're going to the moon! we're going to mars!     hello \
world! we're going to the moon! we're going to the pool! hello world! we're \
going to the moon! we're going to sleep!    ";

void starter() {
  PRINTF("off to the races!\r\n");
  add_event(&EVENT(sense));
  // Schedules sensing;
  push_task(&TASK(extra_sense));
  dec_event(&EVT_FCN_STARTER);
  EVENT_RETURN();
}

void sense() {
  LCFN_INTERRUPTS_DISABLE;
  PRINTF("Sense!\r\n");
  LCFN_INTERRUPTS_ENABLE;
  add_event(&EVENT(encrypt));
  EVENT_RETURN();
}

void encrypt() {
  // Encrypt data
  uint16_t message_length = PLAINTEXT_SIZE - 1;
  encrypt_rsa(CYPHERTEXT,&CYPHERTEXT_LEN,PLAINTEXT,message_length,&pubkey);
  // Now scheduler radio
  add_event(&EVENT(radio));
  dec_event(&EVENT(encrypt));
  LCFN_INTERRUPTS_DISABLE;
  PRINTF("Encrypt! Done\r\n");
  LCFN_INTERRUPTS_ENABLE;
  EVENT_RETURN();
}

void radio() {
  BIT_FLIP(1,1);
  BIT_FLIP(1,1);
  PRINTF("Radio!\r\n");
  for(int j = 0; j < 8; j++) {
    BIT_FLIP(1,0);
    for (unsigned i = 0; i < PKT_LEN; ++i) {
        radio_buff[i] =*(((uint8_t *)&pkt) + (j*32)); }
    radio_send_one_off();
  }
  BIT_FLIP(1,1);
  dec_event(&EVENT(radio));
  EVENT_RETURN();
}

void extra_sense() {
  while(1) { // Run perpetually
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
}


