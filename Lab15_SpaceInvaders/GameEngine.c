//Talal Javed Qadri
// August 11, 2014

#include "GameEngine.h"
#include "Nokia5110.h"
#include "Sprites.h"
#include "ADC.h"

unsigned long FrameCount=0;
unsigned long Distance; // units 0.001 cm
unsigned long ADCdata;  // 12-bit 0 to 4095 sample

struct GameObject {
  unsigned long x;      // x coordinate
  unsigned long y;      // y coordinate
  unsigned char colid; // collision id to help determine collision behavior
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
	const unsigned char *image[2]; // a single pointer to image
																		//no animation used
};
typedef struct BunkerSprite ETyp;
struct BunkerSprite {
	GTyp GObj;
	const unsigned char *image[4]; // a single pointer to image
																		//no animation used
};
typedef struct BunkerSprite BTyp;
PTyp Player;
BTyp Bunkers[2];
ETyp Enemy[12];


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
		Enemy[i].GObj.colid = 1;
   }
}

void Player_Move(void){
	//The x co-ordinate of the bottom left corner of the player ship can move from 0 to 64 on the x axis (player ship is 18 pixels wide and screen is 84 pixels)
	//The ConvertToDistance method returns a distance between 0 and 3 cm in units of 0.001 cm (i.e. a number between 0 and 3000
	//This distance was matched with pixel location on x axis, for the bottom left corner of player ship, using raw data from the simulation debugging
	// The relationship was found to be BottomLeftX = Distance*0.0229 - 1.8 via a linear fit to data
	//0.0229 was approximated as 22/1024 (right shift by 10 is division by 1024) to get a pixel location accurate to 1 pixel
	//This pixel location varied from 0 to 64
	Player.GObj.x = (ConvertToDistance(ADC0_In())*22) >> 10; 
}

void Enemy_Move(void){ int i;
  for(i=0;i<12;i++){
    if(Enemy[i].GObj.x < 72){
      Enemy[i].GObj.x += 2; // move to right
    }else{
      Enemy[i].GObj.x = 0; //reached end, start from left most end again
    }
  }
}
void Draw_Frame(void){ int i, j;
  Nokia5110_ClearBuffer();
	Nokia5110_PrintBMP(Player.GObj.x, Player.GObj.y, Player.image, 0); // player ship middle bottom
	for(j=0;j<2;j++){
		Nokia5110_PrintBMP(Bunkers[j].GObj.x, Bunkers[j].GObj.y, Bunkers[j].image[0], 0); 
	}
  for(i=0;i<12;i++){
    if(Enemy[i].GObj.life > 0){
     Nokia5110_PrintBMP(Enemy[i].GObj.x, Enemy[i].GObj.y, Enemy[i].image[FrameCount], 0);
		}
  }
	
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
