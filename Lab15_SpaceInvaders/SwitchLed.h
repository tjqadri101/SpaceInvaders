// Talal Javed Qadri
//August 11, 2014

// Port E bits 1-0 are inputs
// fire button connected to PE0
// special weapon fire button connected to PE1

//Port E bits 5-4 are outputs
// LED on PB4
// LED on PB5

//Prototype for functions dealing with switch and led interface

// Initialize piano key inputs
// Input: none
// Output: none
void SwitchLed_Init(void);

// Input from fire button (PE0)
// Input: none 
// Output: 0 or 1 depending on whether button was just pressed (positive logic)
unsigned char Switch_Fire(void);

// Input from special weapons button (PE1)
// Input: none 
// Output: 0 or 1 depending on whether button was just pressed (positive logic)
unsigned char Switch_SpecialFire(void);

//Turns on Led connected to PB4 when something positive happens e.g. enemy hit, game won etc.
//Initializes a value for the SuccessLedCount variable which can be used to determine how many interrupt periods (like Timer2A perids the LED  should stay on
void Success_LedOn(unsigned long count);

//Turns on Led connected to PB5 when something negative happens e.g. player hit, bunker hit, game lost etc.
//Initializes a value for the FailureLedCount variable which can be used to determine how many interrupt periods (like Timer2A perids the LED  should stay on
void Failure_LedOn(unsigned long count);

//Decrements the value of the SuccessLedCount variable and then returns this value
unsigned long Success_LedCount(void);

//Decrements the value of the FailureLedCount variable and then returns this value
unsigned long Failure_LedCount(void);

//Turns off Led connected to PB4 
void Success_LedOff(void);

//Turns off Led connected to PB5 
void Failure_LedOff(void);
