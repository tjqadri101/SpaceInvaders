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

//Draw the current game state 
void Draw_Frame(void); 

