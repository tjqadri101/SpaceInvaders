//Talal Javed Qadri
//August 11, 2014


//Prototype for functions dealing with game mechanics such as movement, collision etc.

 //Initialize, player, enemies, and bunkers
void Game_Init(void);

//Move all living enemies 2 pixels to the right
// if enemy reaches edge of screen then move it to the left edge while maintaining the y-coordinate
void Enemy_Move(void);

void Player_Move(void);

//Draw the current game state 
void Draw_Frame(void); 

