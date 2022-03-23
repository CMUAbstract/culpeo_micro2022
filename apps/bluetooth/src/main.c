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
#include <libradio/radio.h>

#define BIT_FLIP(port,bit) \
	P##port##OUT |= BIT##bit; \
	P##port##DIR |= BIT##bit; \
	P##port##OUT &= ~BIT##bit; \

#define PKT_LEN 64

//TODO put these functions somewhere else
#ifdef RUN_ADC

#define ADC_ENABLE \
do { \
  P1SEL1 |= BIT2; \
  P1SEL0 |= BIT2;\
  ADC12CTL0 &= ~ADC12ENC; \
  ADC12CTL0 = ADC12SHT0_2 | ADC12ON; \
  ADC12CTL1 = ADC12SHP;\
  ADC12CTL2 = ADC12RES_2; \
  ADC12MCTL0 = ADC12INCH_2;\
  ADC12IER0 &= ~ADC12IE0;\
  }while(0);


#define ADC_DISABLE \
		ADC12CTL0 &= ~(ADC12ENC | ADC12ON);

uint16_t read_adc(void) {
  // ======== Configure ADC ========
  // Take single sample when timer triggers and compare with threshold
  ADC12IFGR0 &= ~ADC12IFG0;
  // Use ADC12SC to trigger and single-channel
  ADC12CTL1 |= ADC12SHP | ADC12SHS_0 | ADC12CONSEQ_0 ;
  ADC12CTL0 |= (ADC12ON + ADC12ENC + ADC12SC); 			// Trigger ADC conversion

  while(!(ADC12IFGR0 & ADC12IFG0)); 			// Wait till conversion over
  uint16_t adc_reading = ADC12MEM0; 					// Read ADC value
  return adc_reading;
}

#else

#define ADC_ENABLE

#define ADC_DISABLE

uint16_t read_adc(void) {
  return 0xff;
}

#endif


uint16_t min_reading = 0xFFFF;

int main(void) {
  capybara_init();
  char pkt[64] =  "hello world! we're going to the moon! we're going to the moon!  ";
  char pkt1[64] =  "hello world! we're going to the moon! we're going to mars!     ";
  char pkt2[64] =  "hello world! we're going to the moon! we're going to the pool! ";
  char pkt3[64] =  "hello world! we're going to the moon! we're going to sleep!    ";
  // Wait for trigger
  while(1) {
    if (P1IN & BIT4){
      break;
    }
  }
  ADC_ENABLE;
  // Run timer
  __delay_cycles(1000);
  uint16_t val = read_adc();
  PRINTF("Starting: %u\r\n",val);
	TA1CCR0 = 8000;
	TA1CTL = MC__UP | TASSEL__SMCLK;
  TA1CCTL0 = CCIE;
  // done
  for (unsigned i = 0; i < PKT_LEN; ++i) {
      radio_buff[i] =*(((uint8_t *)&pkt) + i); }
  for (unsigned i = 0; i < PKT_LEN; ++i) {
      radio_buff[i+64] =*(((uint8_t *)&pkt1) + i); 
  }
  for (unsigned i = 0; i < PKT_LEN; ++i) {
      radio_buff[i+128] =*(((uint8_t *)&pkt2) + i); 
  }
  for (unsigned i = 0; i < PKT_LEN; ++i) {
      radio_buff[i+172] =*(((uint8_t *)&pkt3) + i); 
  }
  while(1) {
    for(int j = 0; j < 8; j++) {
      BIT_FLIP(1,0);
      //PRINTF("Here\r\n");
      radio_send_one_off();
    }
    BIT_FLIP(1,1);
    ADC_DISABLE;
    TA1CCTL0 &= ~CCIE;
    __delay_cycles(8000);
    PRINTF("Min val: %u\r\n",min_reading);
    while(1);
  }
  return 0;
}


void __attribute__((interrupt(TIMER1_A0_VECTOR)))
timerA1ISRHandler(void) {
	  //TA1CCTL0 &= ~(CCIFG | CCIE); // Clear flag and stop int
    //BIT_FLIP(1,0);
    uint16_t val;
    //BIT_FLIP(1,1);
    TA1R = 0;
    val = read_adc();
    if (min_reading > val) {
      min_reading = val;
    }
   //BIT_FLIP(1,1);
   //BIT_FLIP(1,1);
    //TA1CCTL0 |= CCIE; // Re-enable timer int.
}

