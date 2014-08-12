// DAC.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Implementation of the 4-bit digital to analog converter
// Talal Javed Qadri
// August 5, 2014
// Port B bits 3-0 have the 4-bit DAC

#include "DAC.h"
#include "tm4c123gh6pm.h"

#define PB3_0 (*((volatile unsigned long *)0x4000503C)) //access only Port B bits 3-0

// Initialize 4-bit DAC 
// Input: none
// Output: none
void DAC_Init(void){
	/*unsigned long volatile delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // activate port B
  delay = SYSCTL_RCGC2_R;    // allow time to finish activating*/
	
	//This function must be called after SwitchLed_Init function from SwitchLed.c
	//The clock for PortB is activated in SwitchLed_Init; hence it does not need to be activated here
	
  GPIO_PORTB_AMSEL_R &= ~0x0F;      // no analog function for PB3-0
  GPIO_PORTB_PCTL_R &= ~0x0000FFFF; // regular function for PB3-0
  GPIO_PORTB_DIR_R |= 0x0F;      // make PB3-0 out
	GPIO_PORTB_DR8R_R |= 0x0F;    // can drive up to 8mA out
  GPIO_PORTB_AFSEL_R &= ~0x0F;   // disable alt funct on PB3-0
  GPIO_PORTB_DEN_R |= 0x0F;      // enable digital I/O on PB3-0
}



// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(unsigned long data){
  //GPIO_PORTB_DATA_R = data;
  PB3_0 = data;
}
