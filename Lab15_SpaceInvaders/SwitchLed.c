//Talal Javed Qadri
//August 11, 2014

// Port E bits 1-0 are inputs
// fire button connected to PE0
// special weapon fire button connected to PE1

//Port E bits 5-4 are outputs
// LED on PB4
// LED on PB5

#include "tm4c123gh6pm.h"
#include "SwitchLed.h"

unsigned long PrevRegFire = 0;
unsigned long PrevSpecFire = 0;
unsigned long SuccessLedCount = 0;
unsigned long FailureLedCount = 0;

// Initialize switch inputs and LED outputs
// Input: none
// Output: none
void SwitchLed_Init(void){ 
  volatile unsigned long  delay;
	//Clock for Port E already activated in ADC_Init which is called before this function in main
  /*SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;     // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start	*/
	GPIO_PORTE_AMSEL_R &= ~0x03; // 3) disable analog function on PE1-0
  GPIO_PORTE_PCTL_R &= ~0x000000FF; // 4) enable regular GPIO on PE1-0
  GPIO_PORTE_DIR_R &= ~0x03;   // 5) inputs on PE1-0
  GPIO_PORTE_AFSEL_R &= ~0x03; // 6) regular function on PE1-0
  GPIO_PORTE_DEN_R |= 0x03;    // 7) enable digital on PE1-0
	
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // activate port B
  delay = SYSCTL_RCGC2_R;    // allow time to finish activating
  GPIO_PORTB_AMSEL_R &= ~0x30;      // no analog function for PB5-4
  GPIO_PORTB_PCTL_R &= ~0x00FF0000; // regular function for PB5-4
  GPIO_PORTB_DIR_R |= 0x30;      // make PB5-4 in
  GPIO_PORTB_AFSEL_R &= ~0x30;   // disable alt funct on PB5-4
  GPIO_PORTB_DEN_R |= 0x30;      // enable digital I/O on PB5-4
}

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


//Turns on Led connected to PB4 when something positive happens e.g. enemy hit, game won etc.
//Initializes a value for the SuccessLedCount variable which can be used to determine how many interrupt periods (like Timer2A perids the LED  should stay on
void Success_LedOn(unsigned long count){
	GPIO_PORTB_DATA_R |= 0x10;
	SuccessLedCount = count;
}

//Turns on Led connected to PB5 when something negative happens e.g. player hit, bunker hit, game lost etc.
//Initializes a value for the FailureLedCount variable which can be used to determine how many interrupt periods (like Timer2A perids the LED  should stay on
void Failure_LedOn(unsigned long count){
	GPIO_PORTB_DATA_R |= 0x20;
	FailureLedCount = count;
}

//Decrements the value of the SuccessLedCount variable and then returns this value
unsigned long Success_LedCount(void){
	if(SuccessLedCount)
			SuccessLedCount--;
	
	return SuccessLedCount;
}

//Decrements the value of the FailureLedCount variable and then returns this value
unsigned long Failure_LedCount(void){
	if(FailureLedCount)
			FailureLedCount--;
	
	return FailureLedCount;
}

//Turns off Led connected to PB4 
void Success_LedOff(void){
	GPIO_PORTB_DATA_R &= ~0x10;
}

//Turns off Led connected to PB5 
void Failure_LedOff(void){
	GPIO_PORTB_DATA_R &= ~0x20;
	
}
