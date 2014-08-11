// Talal Javed Qadri
//August 11, 2014

// Port E bits 1-0 are inputs
// fire button connected to PE0
// special weapon fire button connected to PE1

//Port E bits 5-4 are outputs
// LED on PB4
// LED on PB5

#include "SwitchLed.h"
#include "tm4c123gh6pm.h"

unsigned long PrevRegFire = 0;
unsigned long PrevSpecFire = 0;

// **************SwitchLed_Init*********************
// Initialize piano key inputs
// Input: none
// Output: none
void SwitchLed_Init(void){ 
  volatile unsigned long  delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;     // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start	
	GPIO_PORTE_AMSEL_R &= ~0x03; // 3) disable analog function on PE1-0
  GPIO_PORTE_PCTL_R &= ~0x000000FF; // 4) enable regular GPIO on PE1-0
  GPIO_PORTE_DIR_R &= ~0x03;   // 5) inputs on PE3-0
  GPIO_PORTE_AFSEL_R &= ~0x03; // 6) regular function on PE3-0
  GPIO_PORTE_DEN_R |= 0x03;    // 7) enable digital on PE3-0
	
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // activate port B
  delay = SYSCTL_RCGC2_R;    // allow time to finish activating
  GPIO_PORTB_AMSEL_R &= ~0x30;      // no analog function for PB5-4
  GPIO_PORTB_PCTL_R &= ~0x00FF0000; // regular function for PB5-4
  GPIO_PORTB_DIR_R &= ~0x30;      // make PB5-4 in
  GPIO_PORTB_AFSEL_R &= ~0x30;   // disable alt funct on PB5-4
  GPIO_PORTB_DEN_R |= 0x30;      // enable digital I/O on PB5-4
}
// **************Switch_Fire*********************
// Input from fire button (PE0)
// Input: none 
// Output: 0 or 1 depending on whether button was just pressed (positive logic)
unsigned char Switch_Fire(void){
	 unsigned char FireBool;
   if((GPIO_PORTE_DATA_R&0x01) && (PrevRegFire == 0)){ // just pressed
		 FireBool = 1;
	 }
	 else{
		 FireBool = 0;
	 }
	 PrevRegFire = GPIO_PORTE_DATA_R&0x01;
	 return FireBool;
}

// **************Switch_SpecialFire*********************
// Input from special weapons button (PE1)
// Input: none 
// Output: 0 or 1 depending on whether button was just pressed (positive logic)
unsigned char Switch_SpecialFire(void){
	unsigned char SpecFireBool;
   if((GPIO_PORTE_DATA_R&0x02) && (PrevSpecFire == 0)){ // just pressed
		 SpecFireBool = 1;
	 }
	 else{
		 SpecFireBool = 0;
	 }
	 PrevSpecFire = GPIO_PORTE_DATA_R&0x02;
	 return SpecFireBool;
}
