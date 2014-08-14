// SpaceInvaders.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the edX Lab 15
// In order for other students to play your game
// 1) You must leave the hardware configuration as defined
// 2) You must not add/remove any files from the project
// 3) You must add your code only this this C file
// I.e., if you wish to use code from sprite.c or sound.c, move that code in this file
// 4) It must compile with the 32k limit of the free Keil
// April 10, 2014
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
"Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013
"Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013
Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
You may use, edit, run or distribute this file
as long as the above copyright notice remains
THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED
OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
For more information about my classes, my research, and my books, see
http://users.ece.utexas.edu/~valvano/
*/
//Modified by Talal Javed Qadri
//August 11, 2014
//Requirements 2 and 3 on lines 7 and 8 above were not followed as this project was not shared with
// the rest of the class as I went through the course after it was completed
//Hence additional files were added to this project
// ******* Required Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5
// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal (Nokia 5110) LaunchPad pin
// 3.3V (VCC, pin 1) power
// Ground (GND, pin 2) ground
// SSI0Fss (SCE, pin 3) connected to PA3
// Reset (RST, pin 4) connected to PA7
// Data/Command (D/C, pin 5) connected to PA6
// SSI0Tx (DN, pin 6) connected to PA5
// SSI0Clk (SCLK, pin 7) connected to PA2
// back light (LED, pin 8) not connected, consists of 4 white LEDs which draw ~80mA total

// ***** 1. Pre-processor Directives Section *****
#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "SwitchLed.h"
#include "Sound.h"
#include "Random.h"
#include "ADC.h"
#include "GameEngine.h"
#include "TExaS.h"


//***** 2. Global Declarations Section *****

//Global variables
unsigned char GameOverFlag;
unsigned char Semaphore = 0;

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Timer2_Init(void(*task)(void), unsigned long period);
void Delay100ms(unsigned long count); // time delay in 0.1 seconds
void PF1Init(void); //Initialize PF1 (PF1) for debugging the SysTick interrupts
//void SwitchLed_Init(void);// Initialize switch inputs and LED outputs
void SysTick_Init(unsigned long period); // Initialize SysTick interrupts
void (*PeriodicTask)(void);   // user function for Timer2A (comes from Sound.c and outputs to 4 bit DAC)

// ***** 3. Subroutines Section *****

int main(void){
	DisableInterrupts();
  TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
	Random_Init(1);
  Nokia5110_Init();
	PF1Init();
  //SysTick_Init(2666666); //Initialize SysTick with 30 Hz interrupts
	SysTick_Init(2666666*4); //Increased period by 4 for actual hardware to make the game run at a playable speed
  Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();      // draw buffer
	ADC0_Init();
	Game_Init();
	SwitchLed_Init();
	Sound_Init();
	Timer2_Init(&Sound_Play,7256); //11.025 kHz. 80,000,000/11,025 cycles, which is about 7256
	GameOverFlag = 0;
	EnableInterrupts();
	
  while(1){
		while(Semaphore==0){};
    Semaphore = 0;
		if(GameOverFlag){
			State_GameOver();
		}
		else{
			Draw_GameFrame(); // update the LCD
		}	
		if((GameOverFlag == 0) && (Check_GameOver())){ //just detected game over
			Delay100ms(2);//Delay 200ms
			GameOverFlag = Check_GameOver();
			//SysTick_Init(2666666);//Re-initialize with 30 Hz interrupt
			SysTick_Init(2666666*4); //Increased period by 4 for actual hardware to make the game run at a playable speed
		}
	}
}

void PF1Init(void){
	volatile unsigned long  delay;
	SYSCTL_RCGC2_R |= 0x00000020;     // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
  GPIO_PORTF_AMSEL_R &= ~0x02;        // 3) disable analog on PF1
  GPIO_PORTF_PCTL_R &= ~0x000000F0;   // 4) PCTL GPIO on PF1
  GPIO_PORTF_DIR_R |= 0x02;          // 5) PF1 out
  GPIO_PORTF_AFSEL_R &= ~0x02;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_DEN_R |= 0x02;          // 7) enable digital I/O on PF1
}

void SysTick_Init(unsigned long period){
	NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 1      
  NVIC_ST_CTRL_R = 0x0007;  // enable SysTick with core clock and interrupts
}
void SysTick_Handler(void){  // runs at frequency of SysTick interrupts
	GPIO_PORTF_DATA_R ^= 0x02;     // toggle PF1, debugging
	//Game Engigine methods below
	if(GameOverFlag){
		if(Switch_Fire() || Switch_SpecialFire()){
			GameOverFlag = 0;
			Game_Init();
		}
	}
	else{
		Check_Collisions();
		Move_ActiveObjects();  
		if(Switch_Fire()){
			RegMissile_Fire();
			Sound_Shoot();
		}
		if(Switch_SpecialFire()){
			SpecMissile_Fire();
			Sound_Shoot();
		}
		SysTick_Init(Set_Difficulty());
	}
  Semaphore = 1;
}


// ***************** Timer2_Init **************** (This method was written by Jonathan Valvano)
// Activate Timer2 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer2_Init(void(*task)(void), unsigned long period){
  unsigned long volatile delay;
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  delay = SYSCTL_RCGCTIMER_R;
  PeriodicTask = task;          // user function
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}

void Timer2A_Handler(void){
	unsigned long checkSuccessLed = Success_LedCount();
	unsigned long checkFailureLed = Failure_LedCount();
   TIMER2_ICR_R = 0x00000001;   // acknowledge timer2A timeout
	if(!checkSuccessLed) //SucccessLedCount = 0
		Success_LedOff();
	
	if(!checkFailureLed)//FailureLedCount = 0
		Failure_LedOff();
	
  (*PeriodicTask)();                // execute user task
}

void Delay100ms(unsigned long count){unsigned long volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}
