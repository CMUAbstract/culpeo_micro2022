#include <msp430.h>
#include <stdio.h>
#include <stdint.h>
#include <libio/console.h>
unsigned read_mic(void){
  
  ADC12IFGR0 &= ~ADC12IFG1;
  // Use ADC12SC to trigger and single-channel
  //ADC12CTL1 |= ADC12SHP | ADC12SHS_0 | ADC12CONSEQ_0 ;
  //ADC12CTL0 |= (ADC12ON + ADC12ENC + ADC12SC); 			// Trigger ADC conversion


  ADC12CTL0 |= ADC12SC;                   // Start conversion
	ADC12CTL0 &= ~ADC12SC;                  // We only need to toggle to start conversion
  //while(!(ADC12IFGR0 & ADC12IFG0)); 			// Wait till conversion over
  while(!(ADC12IFGR0 & ADC12IFG1)); 			// Wait till conversion over
	//while (ADC12CTL1 & ADC12BUSY) ;

	unsigned n = ADC12MEM1;
	//unsigned n = ADC12MEM0;

	// V = (n - 0.5)*3.3/4096 * 100
	// 100 is not to use float
	// ~ n * 330 / 4096

	unsigned output = (uint16_t)(((uint32_t)n * 250) / 4096);

	return output;
}

void init_adc_mic() {
	ADC12CTL0 &= ~ADC12ENC;           // Disable conversions
  while(REFCTL0 & REFGENBUSY);            // If ref generator busy, WAIT

	ADC12CTL1 = ADC12SHP | ADC12CONSEQ_1;
	ADC12MCTL1 = ADC12VRSEL_1 | ADC12INCH_14 | ADC12EOS; // Channel A14 / REFV =2.5
	//ADC12CTL1 = ADC12SHP; // SAMPCON is sourced from the sampling timer
	//ADC12MCTL0 = ADC12INCH_14 | ADC12VRSEL_1; // VR+: VREF, VR- = AVSS, // Channel A15
	ADC12CTL0 |= ADC12SHT03 | ADC12MSC | ADC12ON ;
	ADC12CTL0 |= ADC12SHT03 | ADC12ON; // sample and hold time 32 ADC12CLK

	while( REFCTL0 & REFGENBUSY );

	REFCTL0 = REFVSEL_2 | REFON;            //Set reference voltage to 2.5
	//REFCTL0 = REFVSEL_0 | REFON;            //Set reference voltage(VR+) to 1.2

	__delay_cycles(1000);                   // Delay for Ref to settle
	ADC12CTL0 |= ADC12ENC;                  // Enable conversions
}

