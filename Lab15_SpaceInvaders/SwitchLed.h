// Talal Javed Qadri
//August 11, 2014

// Port E bits 1-0 are inputs
// fire button connected to PE0
// special weapon fire button connected to PE1

//Port E bits 5-4 are outputs
// LED on PB4
// LED on PB5
//Prototype for functions dealing with switch and led interface


// **************SwitchLed_Init*********************
// Initialize piano key inputs
// Input: none
// Output: none
void SwitchLed_Init(void);
// **************Switch_Fire*********************
// Input from fire button (PE0)
// Input: none 
// Output: 0 or 1 depending on whether button was just pressed (positive logic)
unsigned char Switch_Fire(void);

// **************Switch_SpecialFire*********************
// Input from special weapons button (PE1)
// Input: none 
// Output: 0 or 1 depending on whether button was just pressed (positive logic)
unsigned char Switch_SpecialFire(void);
