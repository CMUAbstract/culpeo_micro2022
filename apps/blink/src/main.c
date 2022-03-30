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

DEC_EVT(blink,blink,8000); //TODO translation from seconds to ticks
DEC_EVT(blink2,blink2,7000); //TODO translation from seconds to ticks
DEC_TSK(burn,burn);

void starter() {
  BIT_FLIP(1,1);
  PRINTF("off to the races!\r\n");
  add_event(&EVENT(blink));
  //add_event(&EVENT(blink2));
  push_task(&TASK(burn));
  dec_event(&EVT_FCN_STARTER);
  EVENT_RETURN();
}

uint16_t val = 0;
uint8_t new_adc_val = 0;

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

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = ADC12_B_VECTOR
__interrupt void ADC12_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_B_VECTOR))) ADC12_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(ADC12IV, ADC12IV__ADC12RDYIFG))
    {
        case ADC12IV__ADC12IFG0:            // Vector 12:  ADC12MEM0 Interrupt
            if (ADC12MEM0 >= 0x7ff)         // ADC12MEM0 = A1 > 0.5AVcc?
                P1OUT |= BIT0;              // P1.0 = 1
            else
                P1OUT &= ~BIT0;             // P1.0 = 0

            break;
        default: break;
    }
}

void __attribute__((interrupt(TIMER1_A0_VECTOR)))
timerA1ISRHandler(void) {
	  //TA1CCTL0 &= ~(CCIFG | CCIE); // Clear flag and stop int
    //BIT_FLIP(1,0);
    BIT_FLIP(1,4);
    BIT_FLIP(1,5);
    TA1R = 0;
    val = read_adc();
    new_adc_val = 1;
    BIT_FLIP(1,1);
    //TA1CCTL0 |= CCIE; // Re-enable timer int.
}
