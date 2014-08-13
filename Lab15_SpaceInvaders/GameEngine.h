//Talal Javed Qadri
//August 11, 2014


//Prototype for functions dealing with game mechanics such as movement, collision etc.

 //Initialize, player, enemies, and bunkers
void Game_Init(void);

//Fire a regular missile from the player's ship if the number of missiles on the screen are less than 4
//Missiles can pass through bunkers
//Missiles are launched from the center of the player ship
void RegMissile_Fire(void);

//Fire a special missile object from the player's ship if the number of specialmissiles on the screen are less than 2
//Each special missile object launches two missiles that move diagonal upwards and in opposite direction of each other
//Missiles can pass through bunkers
//Missiles are launched from the center of the player ship
void SpecMissile_Fire(void);

//Move all the alive objects in the game according to each objects behavior
void Move_ActiveObjects(void);

//Detect all the collisions for the current fram and respond appropriately by, for example, turning on LEDS, printing explosions etc
void Check_Collisions(void);

//Draw the current game state 
void Draw_GameFrame(void); 

//Returns a number for SysTick period that is used to initialize SysTick interrupts with a frequency between 30-120Hz
//Number is returned such that frequency increases as number of killed enemies increases
//As more enemies are killed, game speed increases, making the game more difficult
unsigned long Set_Difficulty(void);

//returns 1 if game over; 0 otherwise
unsigned char Check_GameOver(void);

//Output the frame for the Game Over State
void State_GameOver(void);

