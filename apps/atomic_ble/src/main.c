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
#include <liblsm/gyro.h>
#include <libfxl/fxl6408.h>
#include <libapds/proximity.h>

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

#define EXTRA_SLOTS 12

void starter();
void radio();
void encrypt();
void sense();
void extra_sense();

STARTER_EVT(starter);

#define BLE_PERIOD 24000

DEC_EVT(radio,radio,BLE_PERIOD,SPORADIC); //Deschedules itself
DEC_EVT(encrypt,encrypt,BLE_PERIOD,SPORADIC);//Deschedules itself
DEC_EVT(sense,sense,BLE_PERIOD,PERIODIC);
DEC_TSK(extra_sense,extra_sense);

uint16_t min_reading = 0xFFFF;

void app_hw_init(void) {
  enable_photoresistor();
}

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
  // Init lsm6ds1
  fxl_clear(BIT_SENSE_SW);
  __delay_cycles(640000);
  fxl_set(BIT_SENSE_SW);
  __delay_cycles(640000);
  gyro_init_data_rate_hm(0x80,1);
  PRINTF("Sense!\r\n");
  LCFN_INTERRUPTS_ENABLE;
  uint16_t ax,ay,az,gx,gy,gz;
  for (int i = 0; i < 16; i++) {
    LCFN_INTERRUPTS_DISABLE;
    // Grab IMU vals
    read_raw_gyro(&gx,&gy,&gz);
    read_raw_accel(&ax,&ay,&az);
    PRINTF("%u ",i);
    LCFN_INTERRUPTS_ENABLE;
    // Write to pkt
    // Fill in last 4 bytes!!
    PLAINTEXT[(i << 4) + 0] = gx;
    PLAINTEXT[(i << 4) + 2] = gy;
    PLAINTEXT[(i << 4) + 4] = gz;
    PLAINTEXT[(i << 4) + 6] = ax;
    PLAINTEXT[(i << 4) + 8] = ay;
    PLAINTEXT[(i << 4) + 10] = az;
  }
  LCFN_INTERRUPTS_DISABLE;
  lsm_disable(); //off
  LCFN_INTERRUPTS_ENABLE;
  add_event(&EVENT(encrypt));//TODO add back in
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
        radio_buff[i] =*(((uint8_t *)&PLAINTEXT) + (j*PKT_LEN)); }
    radio_send_one_off();
  }
  BIT_FLIP(1,1);
  dec_event(&EVENT(radio));
  EVENT_RETURN();
}

void extra_sense() {
  while(1) { // Run perpetually
    uint16_t light, light1;
    for (int i = 0; i < 16; i++) {
      LCFN_INTERRUPTS_DISABLE;
      //PRINTF("\t\tStart task\r\n");
      //enable_photoresistor();
      light = read_photoresistor();
      //Instead of delay?
      //enable_photoresistor();
      light1 = read_photoresistor();
      LCFN_INTERRUPTS_ENABLE;
      //TODO add an averaging step instead, write avg & stddev
      for( int j = 0; j < 0xff; j++) {
        for(int i = 0; i < 0xff; i++) {
          volatile uint16_t temp = i;
        }
      }
      LCFN_INTERRUPTS_DISABLE;
      PRINTF("Tasking %u!\r\n",i);
      __delay_cycles(1000);
      LCFN_INTERRUPTS_ENABLE;
      PLAINTEXT[(i<<4) + 12] = light;
      PLAINTEXT[(i<<4) + 14] = light1;
    }
  }
}


