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
#include <libfakecatnap/culpeo.h>
#include <libfakecatnap/events.h>
#include <libfakecatnap/tasks.h>
#include <libfakecatnap/hw.h>

// App specifcis
#include <libdsp/DSPLib.h>
#include "fft.h"
#include "mic.h"

#define BIT_FLIP(port,bit) \
	P##port##OUT |= BIT##bit; \
	P##port##DIR |= BIT##bit; \
	P##port##OUT &= ~BIT##bit; \

#define PKT_LEN 32

//TODO put these functions somewhere else

#define EXTRA_SLOTS 12

void starter();
void radio();
void run_fft();
void read_mic_event();

STARTER_EVT(starter);

#define BLE_PERIOD 55000
#define SENSING_PERIOD 30000

DEC_EVT(radio,radio,BLE_PERIOD,SPORADIC); //Deschedules itself
//TODO remove PRE-DECS
#if 0
__nv evt_t EVT_radio =
  { .evt = &radio,
    .period = BLE_PERIOD,
    .periodic = SPORADIC,
    .burst_num = 1, /*TODO switch back to 1*/\
    .time_rdy = BLE_PERIOD,
    .valid = WAITING,
    .no_profile = 0,
    .V_safe = 2118,
    .V_min = 2.052,
    .V_final = 2.301
  };
#endif
//DEC_EVT(encrypt,encrypt,BLE_PERIOD,SPORADIC);//Deschedules itself
DEC_TSK(run_fft,run_fft);
//DEC_EVT(read_mic_event,read_mic_event,SENSING_PERIOD,PERIODIC);
// Can't profile because it also uses the adc
__nv evt_t EVT_read_mic_event =
  { .evt = &read_mic_event,
    .period = SENSING_PERIOD,
    .periodic = PERIODIC,
    .burst_num = 1, /*TODO switch back to 1*/\
    .time_rdy = SENSING_PERIOD,
    .valid = WAITING,
    .no_profile = 1,
#if 0
    .V_safe = 1617,
    .V_min = 2.389,
    .V_final = 2.423
#else
    .V_safe = 0,
    .V_min = 0,
    .V_final = 0 
#endif
  };

uint16_t min_reading = 0xFFFF;

#define SAMPLES 256

__nv uint16_t mic_out_buff1[SAMPLES];
__nv uint16_t mic_out_buff2[SAMPLES];
__nv uint16_t *mic_out_ptr = mic_out_buff1;
__nv uint8_t fft_out[128];
__nv uint8_t fft_working[128];
__nv uint8_t fft_done = 0;
__nv uint8_t radio_buffer[256]; // Stores 2 outputs of the fft

void app_hw_init(void) {
	P3SEL0 |= BIT2;
	P3SEL1 |= BIT2;
	//P2DIR |= BIT2;
	//P2OUT |= BIT2;
#ifdef RUN_INT
  // Init interrupt on P1.5
  P1DIR &= ~BIT5; //input
  P1REN |= BIT5; //pulldown
  P1IFG = 0;
  P1IES &= ~BIT5; //low to high
  P1IE |= BIT5; //enable int
#endif
}

void starter() {
  PRINTF("off to the races!\r\n");
  add_event(&EVENT(read_mic_event));
  add_event(&EVENT(radio));
  // Schedules sensing;
  push_task(&TASK(run_fft));
  dec_event(&EVT_FCN_STARTER);
  EVENT_RETURN();
}



void read_mic_event() {
	// Config the ADC on the comparator pin

	// Power the mic
	P2DIR |= BIT2;
	P2OUT |= BIT2;
  BIT_FLIP(1,0);
  BIT_FLIP(1,0);
	// Wait for a bit for mic warmup
  __delay_cycles(1000000);
  __delay_cycles(1000000);
	init_adc_mic();
  PRINTF("Mic\r\n");

  // Read 64 samples
  uint16_t *mic_out_cur;
  mic_out_cur = mic_out_ptr == mic_out_buff2 ? mic_out_buff1 : mic_out_buff2;
  uint16_t temp;
	for (unsigned i = 0; i < SAMPLES; ++i) {
	// Read mic
    temp = read_mic();
		*(mic_out_cur + i)  = temp;
	}
  PRINTF("Val: %u\r\n",temp);
  fft_done = 0;
	// Power off the mic
  if (!culpeo_profiling_flag) {
    ADC12CTL0 &= ~ADC12ENC;                 // Disable conversions
    ADC12CTL0 &= ~(ADC12ON);                // Shutdown ADC12
    REFCTL0 &= ~REFON;
  }
	P2OUT &= ~BIT2;
  EVENT_RETURN();
}

void radio() {
  P1IE &= ~BIT5;//Disable extra interrupt
  PRINTF("Radio!\r\n");
  uint8_t val;
  if (fft_done) {
    for(int i = 0; i < 128; i++) {
      radio_buffer[i] = fft_working[i];
    }
  }
  else {
    for(int i = 0; i < 128; i++) {
      radio_buffer[i] = 0;
    }
  }
  for(int i = 0; i < 128; i++) {
    radio_buffer[i+128] = fft_out[i];
  }
  for(int j = 0; j < 8; j++) {
    for (unsigned i = 0; i < PKT_LEN; ++i) {
      val = *(((uint8_t *)&radio_buffer) + (j*PKT_LEN) + i);
      radio_buff[i] = val ;
    }
    radio_send_one_off();
  }
  /*Emulate low power listen*/
  __delay_cycles(8000000);
  __delay_cycles(8000000);
  BIT_FLIP(1,0);
  //dec_event(&EVENT(radio));
  dec_event(&EVENT(radio));
  P1IE |= BIT5;// Enable extra interrupt
  EVENT_RETURN();
}

#define FFT_LEN 64

void run_fft() {
  while(1) { // Run perpetually
    /*LCFN_INTERRUPTS_DISABLE;
    PRINTF("FFT start!\r\n");
    LCFN_INTERRUPTS_ENABLE;*/
    _q15 buf[SAMPLES];
    // Heuristically, we choose 0.75V as a bias.
    int bias = 0;

    // Convert to Q15
    // Undersample (without LPF..)
    unsigned step = SAMPLES / FFT_LEN;
    for (unsigned i = 0; i < FFT_LEN; ++i) {
      // Subtract 0.67V bias
		  buf[i] = _Q15((float)(*(mic_out_ptr + (i * step)) - bias) / 100);
      /*LCFN_INTERRUPTS_DISABLE;
      PRINTF("%x %x,", (unsigned)((unsigned)buf[0] >> 16),
      (unsigned)((unsigned)buf[i] & 0xFFFF));
      LCFN_INTERRUPTS_ENABLE;*/
    }
      //PRINTF("\r\n");
    // FFT
    fft(buf, (float *) fft_working, FFT_LEN);
    // Swap mic ptr
    mic_out_ptr = mic_out_ptr == mic_out_buff1 ? mic_out_buff2 : mic_out_buff1;
    fft_done = 1;
    BIT_FLIP(1,4);
    //PRINTF("\r\n");
    for (int i =0 ; i < FFT_LEN; i++) {
      fft_out[i] = fft_working[i];
      //print_float(fft_out[i]);
    }
    /*LCFN_INTERRUPTS_DISABLE;
    PRINTF("\r\nDone fft!\r\n");
    LCFN_INTERRUPTS_ENABLE;*/
    BIT_FLIP(1,4);
  }
}

#ifdef RUN_INT
void __attribute__((interrupt(PORT1_VECTOR)))
port1_handler(void) {
  P1IE &= ~BIT5;
  P1IFG = 0;
  add_event(&EVENT(radio));
  //PRINTF("Adding event int!\r\n");
  P1IE |= BIT5;
  INT_RETURN;
}
#endif
