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
void Success_LedOn(void);

//Turns on Led connected to PB5 when something negative happens e.g. player hit, bunker hit, game lost etc.
void Failure_LedOn(void);

//Turns off Led connected to PB4 
void Success_LedOff(void);

//Turns off Led connected to PB5 
void Failure_LedOff(void);
