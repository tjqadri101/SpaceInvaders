//Talal Javed Qadri
// August 11, 2014

#include "GameEngine.h"
#include "Nokia5110.h"
#include "Sprites.h"
#include "ADC.h"

#define MAX_REG_MISSILES 4
#define MAX_SPEC_MISSILES 2
#define MAX_LASERS 5

unsigned long FrameCount=0;
unsigned long Distance; // units 0.001 cm
unsigned long ADCdata;  // 12-bit 0 to 4095 sample
unsigned char RegMissileCount = 0;
unsigned char SpecMissileCount = 0;
unsigned char SpecMissileDecrementCheck = 0;
unsigned char LaserCount = 0;

struct GameObject {
  unsigned long x;      // x coordinate
  unsigned long y;      // y coordinate
  unsigned char colid; // 0 = not collided, 1= collided
  unsigned char life;  // 0=dead, greater than 0 = alive
};         
typedef struct GameObject GTyp;

struct PlayerSprite {
	GTyp GObj;
	const unsigned char *image; // a single pointer to image
																		//no animation used
};
typedef struct PlayerSprite PTyp;

struct EnemySprite {
	GTyp GObj;
	const unsigned char *image[2]; // two pointers to images
																		
};
typedef struct BunkerSprite ETyp;
struct BunkerSprite {
	GTyp GObj;
	const unsigned char *image[4]; //4 pointers to images
																		
};
typedef struct BunkerSprite BTyp;

//Lasers fired by enemy invaders
//A maximum of 5 lasers will be allowed on the screen at the same time using counters
//The speed would be set to make the lasers travel vertically downwards
struct EnemyLaserSprite {
	GTyp GObj;
	const unsigned char *image; // a single pointer to image
	unsigned long yspeed;																	
};
typedef struct EnemyLaserSprite ELTyp; 

//Regular missiles fired by the player ship
//A maximum of four missiles will be allowed on the screen at a time
//The speed would be set to make the lasers travel vertically upwards
struct RegMissileSprite {
	GTyp GObj;
	const unsigned char *image; // a single pointer to image
	unsigned long yspeed;																	
};
typedef struct RegMissileSprite RMTyp; 

//Special missiles fired by the player ship
//A maximum of 2 special missile objects will be allowed on the screen at a time
//Each missile object fires two missiles
//The x and y speeds will be set to make the missiles travel diagonally
//The speed would be set to make the lasers travel vertically upwards
struct SpecMissileSprite {
	GTyp GObj1;
	GTyp GObj2;
	const unsigned char *image[2]; // 2 pointers to 2 images
	unsigned long xspeed;	
	unsigned long yspeed;																	
};
typedef struct SpecMissileSprite SMTyp; 

PTyp Player;
RMTyp RegMissiles[MAX_REG_MISSILES];
SMTyp SpecMissiles[MAX_SPEC_MISSILES];
BTyp Bunkers[2];
ETyp Enemy[12];
ELTyp Lasers[MAX_LASERS];



// Convert a 12-bit binary ADC sample into a 32-bit unsigned
// fixed-point distance (resolution 0.001 cm).  Calibration
// data is gathered using known distances and reading the
// ADC value measured on PE1.  
// Input: sample  12-bit ADC sample
// Output: 32-bit distance (resolution 0.001cm)
unsigned long ConvertToDistance(unsigned long sample){
   // A is slope and B is intercept of calibration graph
	//For 3 cm long potentiometer with 10 kohm resistance:
	return ((750*sample) >> 10) + (429 >> 10); //A was found to be 0.7326, right shift by 10 is division by 1024, 750/1024 is approximately 0.7326
																	 //B was found to be 0.4188, right shift by 10 is division by 1024, 429/1024 is approximately 0.4188
																	//For 3cm long 10 kohm potentiometer, method returns a number betweem 0 and 3000
}

void Game_Init(void){ 
	int i, j;
	ADC0_Init();
	Player.GObj.x = 32;
	Player.GObj.y = 47;
	Player.image = PlayerShip0;
	Player.GObj.life = 1;
  Player.GObj.colid = 0;
	
	for(j=0;j<2;j++){
		Bunkers[j].GObj.x = (83-BUNKERW)*j;
    Bunkers[j].GObj.y = 47 - PLAYERH;
    Bunkers[j].image[0] = Bunker0;
    Bunkers[j].image[1] = Bunker1;
		Bunkers[i].image[2] = Bunker2;
		Bunkers[i].image[3] = Bunker3;
    Bunkers[j].GObj.life = 3;
		Bunkers[j].GObj.colid = 0;
	}
	
  for(i=0;i<12;i++){
		if(i < 4){
			Enemy[i].GObj.x = 16*i;
			Enemy[i].GObj.y = ENEMY10H - 1;
			Enemy[i].image[0] = SmallEnemy30PointA;
			Enemy[i].image[1] = SmallEnemy30PointB;
			Enemy[i].GObj.life = 1;
		}
		if((i < 8) && (i > 3)){
			Enemy[i].GObj.x = 16*(i-4);
			Enemy[i].GObj.y = 2*ENEMY10H - 1;
			Enemy[i].image[0] = SmallEnemy20PointA;
			Enemy[i].image[1] = SmallEnemy20PointB;
			Enemy[i].GObj.life = 1;
		}
		if(i > 7){
			Enemy[i].GObj.x = 16*(i-8);
			Enemy[i].GObj.y = 3*ENEMY10H - 1;
			Enemy[i].image[0] = SmallEnemy10PointA;
			Enemy[i].image[1] = SmallEnemy10PointB;
			Enemy[i].GObj.life = 1;
		}
		Enemy[i].GObj.colid = 0;
   }
}
void RegMissile_Fire(void){
	if(RegMissileCount < MAX_REG_MISSILES){
			RegMissiles[RegMissileCount].GObj.x = Player.GObj.x + 7; //Player 18 pixels, laser 2 pixels wide
																																//adding 7 makes laser image start at 8th pixel of player, making laser appear from player's center
			RegMissiles[RegMissileCount].GObj.y = 47 - PLAYERH;
			RegMissiles[RegMissileCount].GObj.colid = 0;
			RegMissiles[RegMissileCount].GObj.life = 1;
			RegMissiles[RegMissileCount].image = Laser0;
			RegMissiles[RegMissileCount].yspeed = 2;
			RegMissileCount++;
	}
}

void SpecMissile_Fire(void){
	if(SpecMissileCount < MAX_SPEC_MISSILES){
			SpecMissiles[SpecMissileCount].GObj1.x = Player.GObj.x + 6; //Player 18 pixels, missile is 4 pixels wide
																																//adding 6 makes missile image start at 8th pixel of player
																																// making missile appear from player's center
			SpecMissiles[SpecMissileCount].GObj1.y = 47 - PLAYERH;
			SpecMissiles[SpecMissileCount].GObj1.colid = 0;
			SpecMissiles[SpecMissileCount].GObj1.life = 1;
			SpecMissiles[SpecMissileCount].GObj2.x = Player.GObj.x + 6; 
			SpecMissiles[SpecMissileCount].GObj2.y = 47 - PLAYERH;
			SpecMissiles[SpecMissileCount].GObj2.colid = 0;
			SpecMissiles[SpecMissileCount].GObj2.life = 1;
			//Missile0 will rise upwards and also move to the right
			//Missile1 will rise upwards and also move to the left
			SpecMissiles[SpecMissileCount].image[0] =Missile0;
			SpecMissiles[SpecMissileCount].image[1] =Missile1;
			SpecMissiles[SpecMissileCount].xspeed = 2;
			SpecMissiles[SpecMissileCount].yspeed = 2;
			SpecMissileCount++;
	}
}

//Move player horizontally across the screen
//The x co-ordinate of the bottom left corner of the player ship can move from 0 to 64 on the x axis (player ship is 18 pixels wide and screen is 84 pixels)
//The ConvertToDistance method in GameEngine.c returns a distance between 0 and 3 cm in units of 0.001 cm (i.e. a number between 0 and 3000
//This distance was matched with pixel location on x axis, for the bottom left corner of player ship, using raw data from the simulation debugging
//The relationship was found to be BottomLeftX = Distance*0.0229 - 1.8 via a linear fit to data
//0.0229 was approximated as 22/1024 (right shift by 10 is division by 1024) to get a pixel location accurate to 1 pixel
//This pixel location varied from 0 to 64
void PlayerMove(void){
	Player.GObj.x = (ConvertToDistance(ADC0_In())*22) >> 10; 
}

//Move all living enemies 2 pixels to the right
// if enemy reaches edge of screen then move it to the left edge while maintaining the y-coordinate
void EnemyMove(void){ unsigned char i;
  for(i=0;i<12;i++){
    if(Enemy[i].GObj.x < 72){
      Enemy[i].GObj.x += 2; // move to right
    }else{
      Enemy[i].GObj.x = 0; //reached end, start from left most end again
    }
  }
}

void RegMissileMove(void){unsigned char i;
	for(i = 0; i < MAX_REG_MISSILES; i++){
		if(RegMissiles[i].GObj.life){
			if(RegMissiles[i].GObj.y <= LASERH ){
				RegMissiles[i].GObj.life = 0;
				RegMissileCount--;	
			}
			else{
				RegMissiles[i].GObj.y -= RegMissiles[i].yspeed; 
			}
		}
	}
}

void SpecMissileMove(void){
	unsigned char i;
	for(i = 0; i < MAX_SPEC_MISSILES; i++){
		if(SpecMissiles[i].GObj1.life){
			//Move Missile0 image right
			if((SpecMissiles[i].GObj1.y > LASERH) && (SpecMissiles[i].GObj1.x < MAX_X-1-MISSILEW)){
				SpecMissiles[i].GObj1.x += SpecMissiles[i].xspeed; 
				SpecMissiles[i].GObj1.y -= SpecMissiles[i].yspeed; 
			}
			else{
				SpecMissileDecrementCheck = 1;
				SpecMissiles[i].GObj1.life = 0;
			}
		}
		
		if(SpecMissiles[i].GObj2.life){
			//Move Missile1 image left
			if((SpecMissiles[i].GObj2.y > LASERH) && (SpecMissiles[i].GObj2.x > MISSILEW)){
				SpecMissiles[i].GObj2.x -= SpecMissiles[i].xspeed; 
				SpecMissiles[i].GObj2.y -= SpecMissiles[i].yspeed; 
			}
			else{
				SpecMissileDecrementCheck = 1;
				SpecMissiles[i].GObj2.life = 0;
			}
	  }
		if(!SpecMissiles[i].GObj1.life && !SpecMissiles[i].GObj2.life && SpecMissileDecrementCheck){
			SpecMissileCount--;
		}
		 SpecMissileDecrementCheck = 0;
	}
}

void Move_ActiveObjects(void){
	PlayerMove();
	EnemyMove();
	RegMissileMove();
	SpecMissileMove();
}

void DrawPlayer(void){
	Nokia5110_PrintBMP(Player.GObj.x, Player.GObj.y, Player.image, 0); // player ship middle bottom
}

void DrawBunkers(void){unsigned char j;
	for(j=0;j<2;j++){
		Nokia5110_PrintBMP(Bunkers[j].GObj.x, Bunkers[j].GObj.y, Bunkers[j].image[0], 0); 
	}
}

void DrawEnemies(void){unsigned char i;
	 for(i=0;i<12;i++){
    if(Enemy[i].GObj.life > 0){
     Nokia5110_PrintBMP(Enemy[i].GObj.x, Enemy[i].GObj.y, Enemy[i].image[FrameCount], 0);
		}
  }
}

void DrawRegMissiles(void){unsigned char i;
	for(i=0;i<MAX_REG_MISSILES;i++){
    if(RegMissiles[i].GObj.life > 0){
     Nokia5110_PrintBMP(RegMissiles[i].GObj.x, RegMissiles[i].GObj.y, RegMissiles[i].image, 0);
		}
  }
}

void DrawSpecMissiles(void){unsigned char i;
	for(i=0;i<MAX_SPEC_MISSILES;i++){
    if(SpecMissiles[i].GObj1.life > 0){
     Nokia5110_PrintBMP(SpecMissiles[i].GObj1.x, SpecMissiles[i].GObj1.y, SpecMissiles[i].image[0], 0);
		}
		
		if(SpecMissiles[i].GObj2.life > 0){
     Nokia5110_PrintBMP(SpecMissiles[i].GObj2.x, SpecMissiles[i].GObj2.y, SpecMissiles[i].image[1], 0);
		}
  }
	
}

void Draw_Frame(void){
  Nokia5110_ClearBuffer();
	DrawPlayer();
	DrawBunkers();
  DrawEnemies();
	DrawRegMissiles();
	DrawSpecMissiles();
  Nokia5110_DisplayBuffer();      // draw buffer
  FrameCount = (FrameCount+1)&0x01; // 0,1,0,1,...
}

  /*Delay100ms(50);              // delay 5 sec at 80 MHz
  Nokia5110_Clear();
  Nokia5110_SetCursor(1, 1);
  Nokia5110_OutString("GAME OVER");
  Nokia5110_SetCursor(1, 2);
  Nokia5110_OutString("Nice try,");
  Nokia5110_SetCursor(1, 3);
  Nokia5110_OutString("Earthling!");
  Nokia5110_SetCursor(2, 4);
  Nokia5110_OutUDec(1234);*/
