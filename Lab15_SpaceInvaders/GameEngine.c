//Talal Javed Qadri
// August 11, 2014

#include "GameEngine.h"
#include "Nokia5110.h"
#include "Sprites.h"
#include "ADC.h"
#include "Sound.h"
#include "SwitchLed.h"
#include "Random.h"

#define MAX_REG_MISSILES 4
#define MAX_SPEC_MISSILES 2
#define MAX_LASERS 5

unsigned long FrameCount=0; //to anmate enemies while they move
unsigned long Distance; // units 0.001 cm
unsigned long ADCdata;  // 12-bit 0 to 4095 sample
unsigned long Score; //game score
unsigned char RegMissileCount; //number of active RMTyp on screen
unsigned char SpecMissileCount; //number of active SLTyp on screen
unsigned char SpecMissileDecrementCheck; //if 1 decrement the SpecMissileCount as long as other conditions also satisfied
unsigned char LaserCount; //number of active ELTyp on screen
unsigned char KilledEnemyCount;//number of alive ETyp (enemies on screen)
unsigned char LaserDelay; //used to create a delay between firing of successive lasers


struct GameObject {
  unsigned long x;      // x coordinate
  unsigned long y;      // y coordinate
  unsigned char life;  // 0=dead, greater than 0 = alive
};         
typedef struct GameObject GTyp;

struct PlayerSprite {
	GTyp GObj;
	const unsigned char *image; // a single pointer to image
																		//no animation used
	unsigned char explode; //explode if > 0
};
typedef struct PlayerSprite PTyp;

struct EnemySprite {
	GTyp GObj;
	const unsigned char *image[2]; // two pointers to images
	unsigned char explode; //1 = draw explosion
	unsigned long hitBonus;																	
};
typedef struct EnemySprite ETyp;

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
	unsigned long hitBonus;
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

//Return a random number, the upper limit is 1 less than the current number of alive enemies on screen with a maximum enemy count of 12
//This method is used to access an enemy at random and launch a laser from its position
//It is also used to create a random value for LaserDelay
unsigned long RandomGenerator(unsigned long enemies){
  return ((Random()>>22)%enemies);  
}

void Game_Init(void){ 
	unsigned char i, j, k, l, m;
	Score = 0;
	RegMissileCount = 0;
  SpecMissileCount = 0; 
  SpecMissileDecrementCheck = 0;
  LaserCount = 0; 
  KilledEnemyCount = 0;
	LaserDelay = RandomGenerator(4);
	Player.GObj.x = 32;
	Player.GObj.y = 47;
	Player.image = PlayerShip0;
	Player.explode = 0;
	Player.GObj.life = 1;
	
	for(j=0;j<2;j++){
		Bunkers[j].GObj.x = (83-BUNKERW)*j;
    Bunkers[j].GObj.y = 47 - PLAYERH;
    Bunkers[j].image[0] = Bunker3;
    Bunkers[j].image[1] = Bunker2;
		Bunkers[j].image[2] = Bunker1;
		Bunkers[j].image[3] = Bunker0;
    Bunkers[j].GObj.life = 3;
	}
	
  for(i=0;i<12;i++){
		Enemy[i].GObj.life = 1;
		Enemy[i].explode = 0;
		if(i < 4){
			Enemy[i].GObj.x = 16*i;
			Enemy[i].GObj.y = ENEMY10H;
			Enemy[i].image[0] = SmallEnemy30PointA;
			Enemy[i].image[1] = SmallEnemy30PointB;
			Enemy[i].hitBonus = 30;
		}
		if((i < 8) && (i > 3)){
			Enemy[i].GObj.x = 16*(i-4);
			Enemy[i].GObj.y = 2*ENEMY10H;
			Enemy[i].image[0] = SmallEnemy20PointA;
			Enemy[i].image[1] = SmallEnemy20PointB;
			Enemy[i].hitBonus = 20;
		}
		if(i > 7){
			Enemy[i].GObj.x = 16*(i-8);
			Enemy[i].GObj.y = 3*ENEMY10H;
			Enemy[i].image[0] = SmallEnemy10PointA;
			Enemy[i].image[1] = SmallEnemy10PointB;
			Enemy[i].hitBonus = 10;
		}
   }
	
	 for(k = 0; k < MAX_LASERS; k++){
		 Lasers[k].GObj.life = 0;
	 }
	 for(l = 0; l < MAX_REG_MISSILES; l++){
		 RegMissiles[l].GObj.life = 0;
	 }
	 for(m = 0; m < MAX_SPEC_MISSILES; m++){
		 SpecMissiles[m].GObj1.life = 0;
		 SpecMissiles[m].GObj2.life = 0;
	 }
}

//Fire laser from an enemy if the number of lasers on screen is less than 5
//Enemies chosen at random from the existing alive enemies
//This method called within LaserMove() method which is called every SysTick interrupt via Move_ActiveObjects() method
//If LaserDelay is not 0, it decrements LaserDelay and returns
//hence, a delay equal to period of 1 Systick Interrupt (1/30Hz) times initialized value of LaserDelay is created between successive laser fires
//LaserDelay is always initialized as a random number between 1 and 5
void EnemyLaserFire(void){unsigned char i, generate;
	if(LaserDelay){
		LaserDelay--;
		return;
	}
	LaserDelay = RandomGenerator(4); 
	for(i = 0; i < 12; i++){
		generate = RandomGenerator(2); //Random number which is either 0 or 1
		if(Enemy[i].GObj.life && (LaserCount < MAX_LASERS) && generate){
			//Lasers are 2 pixels wide
			//Enemies are 16 pixels wide
			Lasers[LaserCount].GObj.x = Enemy[i].GObj.x + 6; //set bottom left of laser at the center pixel along x axis of the enemy
			Lasers[LaserCount].GObj.y = Enemy[i].GObj.y;
			Lasers[LaserCount].GObj.life = 1;
			Lasers[LaserCount].image = Laser0;
			Lasers[LaserCount].yspeed = 2;
			LaserCount++;
			Sound_InvaderShoot();
			return;
		}
	}

}

void RegMissile_Fire(void){
	if(RegMissileCount < MAX_REG_MISSILES){
			RegMissiles[RegMissileCount].GObj.x = Player.GObj.x + 7; //Player 18 pixels, laser 2 pixels wide
																																//adding 7 makes laser image start at 8th pixel of player, making laser appear from player's center
			RegMissiles[RegMissileCount].GObj.y = 47 - PLAYERH;
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
			SpecMissiles[SpecMissileCount].GObj1.life = 1;
			SpecMissiles[SpecMissileCount].GObj2.x = Player.GObj.x + 6; 
			SpecMissiles[SpecMissileCount].GObj2.y = 47 - PLAYERH;
			SpecMissiles[SpecMissileCount].GObj2.life = 1;
			//Missile0 will rise upwards and also move to the right
			//Missile1 will rise upwards and also move to the left
			SpecMissiles[SpecMissileCount].image[0] =Missile0;
			SpecMissiles[SpecMissileCount].image[1] =Missile1;
			SpecMissiles[SpecMissileCount].hitBonus = 10;
			SpecMissiles[SpecMissileCount].xspeed = 2;
			SpecMissiles[SpecMissileCount].yspeed = 2;
			SpecMissileCount++;
	}
}

void CheckEnemyRegMissileCollisions(void){unsigned char i, j;
	for(i = 0; i < 12; i++){		
		if(Enemy[i].GObj.life){
			for(j = 0; j < MAX_REG_MISSILES; j++){
					if((RegMissiles[j].GObj.life) && 
						!(((RegMissiles[j].GObj.x+LASERW) < Enemy[i].GObj.x) || (RegMissiles[j].GObj.x > (Enemy[i].GObj.x + ENEMY10W))) &&
						!((RegMissiles[j].GObj.y < (Enemy[i].GObj.y - ENEMY10H)) || ((RegMissiles[j].GObj.y - LASERH) > Enemy[i].GObj.y))){
								
							Score += Enemy[i].hitBonus;
							Enemy[i].GObj.life = 0;
							RegMissiles[j].GObj.life = 0;
							Enemy[i].explode = 1;
							Success_LedOn(1000); // 1000 Timer2A periods approximately equal 0.9s
							Sound_Killed();
							RegMissileCount--;
							KilledEnemyCount++;
							break;
					}
			}
		}
	}
}

void CheckEnemySpecMissileCollisions(void){unsigned char i, j;
	for(i = 0; i < 12; i++){		
		if(Enemy[i].GObj.life){
			for(j = 0; j < MAX_SPEC_MISSILES; j++){
					if((SpecMissiles[j].GObj1.life) && 
						!(((SpecMissiles[j].GObj1.x+MISSILEW) < Enemy[i].GObj.x) || (SpecMissiles[j].GObj1.x > (Enemy[i].GObj.x + ENEMY10W))) &&
						!((SpecMissiles[j].GObj1.y < (Enemy[i].GObj.y - ENEMY10H)) || ((SpecMissiles[j].GObj1.y - MISSILEH) > Enemy[i].GObj.y))){
								
							Score += Enemy[i].hitBonus + SpecMissiles[j].hitBonus;
							Enemy[i].GObj.life = 0;
							SpecMissiles[j].GObj1.life = 0;
							Enemy[i].explode = 1;
							Success_LedOn(1000); // 1000 Timer2A periods approximately equal 0.9s
							Sound_Killed();
							SpecMissileDecrementCheck = 1;
							KilledEnemyCount++;
							break;
					}
						
					if((SpecMissiles[j].GObj2.life) && 
						!(((SpecMissiles[j].GObj2.x+MISSILEW) < Enemy[i].GObj.x) || (SpecMissiles[j].GObj2.x > (Enemy[i].GObj.x + ENEMY10W))) &&
						!((SpecMissiles[j].GObj2.y < (Enemy[i].GObj.y - ENEMY10H)) || ((SpecMissiles[j].GObj2.y - MISSILEH) > Enemy[i].GObj.y))){
								
							Score += Enemy[i].hitBonus + SpecMissiles[j].hitBonus;
							Enemy[i].GObj.life = 0;
							SpecMissiles[j].GObj2.life = 0;
							Enemy[i].explode = 1;
							Success_LedOn(1000); // 1000 Timer2A periods approximately equal 0.9s
							Sound_Killed();
							SpecMissileDecrementCheck = 1;
							KilledEnemyCount++;
							break;
					}
			}
		}
	}
}

void CheckBumperLaserCollision(void){unsigned char i, j;
		for(i = 0; i < 2; i++){		
		if(Bunkers[i].GObj.life){
			for(j = 0; j < MAX_LASERS; j++){
					if((Lasers[j].GObj.life) && 
						!(((Lasers[j].GObj.x+LASERW) < Bunkers[i].GObj.x) || (Lasers[j].GObj.x > (Bunkers[i].GObj.x + BUNKERW))) &&
						!((Lasers[j].GObj.y < (Bunkers[i].GObj.y - BUNKERH)) || ((Lasers[j].GObj.y - LASERH) > Bunkers[i].GObj.y))){
					
							Bunkers[i].GObj.life--;
							Lasers[j].GObj.life = 0;
							Failure_LedOn(1000); // 1000 Timer2A periods approximately equal 0.9s
							LaserCount--;
							if(Bunkers[i].GObj.life == 0){
								Sound_Explosion();
								break;
							}
					}
			}
		}
	}
}

void CheckPlayerLaserCollision(void){ unsigned char j;
		if(Player.GObj.life){
			for(j = 0; j < MAX_LASERS; j++){
					if((Lasers[j].GObj.life) && 
						!(((Lasers[j].GObj.x+LASERW) < Player.GObj.x) || (Lasers[j].GObj.x > (Player.GObj.x + PLAYERW))) &&
						!((Lasers[j].GObj.y < (Player.GObj.y - PLAYERH)) || ((Lasers[j].GObj.y - LASERH) > Player.GObj.y))){
					
							Player.GObj.life = 0;
							Player.explode = 1;
							Lasers[j].GObj.life = 0;
							Failure_LedOn(1000); // 1000 Timer2A periods approximately equal 0.9s
							LaserCount--;
							Sound_Explosion();
							break;
					}
			}
		}
}

void CheckLaserRegMissileCollision(void){unsigned char i, j;
	for(i = 0; i < MAX_LASERS; i++){		
		if(Lasers[i].GObj.life){
			for(j = 0; j < MAX_REG_MISSILES; j++){
					if((RegMissiles[j].GObj.life) && 
						!(((RegMissiles[j].GObj.x+LASERW) < Lasers[i].GObj.x) || (RegMissiles[j].GObj.x > (Lasers[i].GObj.x + LASERW))) &&
						!((RegMissiles[j].GObj.y < (Lasers[i].GObj.y - LASERH)) || ((RegMissiles[j].GObj.y - LASERH) > Lasers[i].GObj.y))){
					
							Score += 1;
							Lasers[i].GObj.life = 0;
							RegMissiles[j].GObj.life = 0;
							RegMissileCount--;
							LaserCount--;
							Success_LedOn(100); // 100 Timer2A periods approximately equal 0.09s
							break;
					}
			}
		}
	}
}

void CheckLaserSpecMissileCollision(void){unsigned char i, j;
	for(i = 0; i < MAX_LASERS; i++){		
		if(Lasers[i].GObj.life){
			for(j = 0; j < MAX_SPEC_MISSILES; j++){
					if((SpecMissiles[j].GObj1.life) && 
						!(((SpecMissiles[j].GObj1.x+MISSILEW) < Lasers[i].GObj.x) || (SpecMissiles[j].GObj1.x > (Lasers[i].GObj.x + LASERW))) &&
						!((SpecMissiles[j].GObj1.y < (Lasers[i].GObj.y - LASERH)) || ((SpecMissiles[j].GObj1.y - MISSILEH) > Lasers[i].GObj.y))){
					
							Score += 2;
							Lasers[i].GObj.life = 0;
							SpecMissiles[j].GObj1.life = 0;
							SpecMissileDecrementCheck = 1;
							LaserCount--;
							Success_LedOn(100); // 100 Timer2A periods approximately equal 0.09s
							break;
					}
						
						if((SpecMissiles[j].GObj2.life) && 
						!(((SpecMissiles[j].GObj2.x+MISSILEW) < Lasers[i].GObj.x) || (SpecMissiles[j].GObj2.x > (Lasers[i].GObj.x + LASERW))) &&
						!((SpecMissiles[j].GObj2.y < (Lasers[i].GObj.y - LASERH)) || ((SpecMissiles[j].GObj2.y - MISSILEH) > Lasers[i].GObj.y))){
					
							Score += 2;
							Lasers[i].GObj.life = 0;
							SpecMissiles[j].GObj2.life = 0;
							SpecMissileDecrementCheck = 1;
							LaserCount--;
							Success_LedOn(100); // 100 Timer2A periods approximately equal 0.09s
							break;
					}
			}
		}
	}
}

//Detect all the collisions for the current fram and respond appropriately by, for example, turning on LEDS, printing explosions etc
//This method has to be called before the Move_ActiveObjects method
void Check_Collisions(void){
	CheckEnemyRegMissileCollisions();
	CheckEnemySpecMissileCollisions();
	CheckBumperLaserCollision();
	CheckPlayerLaserCollision();
	CheckLaserRegMissileCollision();
	CheckLaserSpecMissileCollision();	
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
		if((SpecMissiles[i].GObj1.life == 0) && (SpecMissiles[i].GObj2.life==0) && SpecMissileDecrementCheck){
			SpecMissileCount--;
		}
		 SpecMissileDecrementCheck = 0;
	}
}

void LaserMove(void){unsigned char i;
	for(i = 0; i < MAX_LASERS; i++){
		if(Lasers[i].GObj.life){
			if(Lasers[i].GObj.y >= (47)){
				Lasers[i].GObj.life = 0;
				LaserCount--;	
			}
			else{
				Lasers[i].GObj.y += Lasers[i].yspeed; 
			}
		}
	}
	EnemyLaserFire(); //create new laser if possible
}

void Move_ActiveObjects(void){
	PlayerMove();
	EnemyMove();
	RegMissileMove();
	SpecMissileMove();
	LaserMove();
}

void DrawPlayer(void){
	if(Player.GObj.life){
		Nokia5110_PrintBMP(Player.GObj.x, Player.GObj.y, Player.image, 0); // player ship middle bottom
	}
	else if(Player.explode){
		Player.explode = 0;
		Nokia5110_PrintBMP(Player.GObj.x, Player.GObj.y,  BigExplosion0, 0); // player ship middle botto
	}
}

void DrawBunkers(void){unsigned char j;
	for(j=0;j<2;j++){
		Nokia5110_PrintBMP(Bunkers[j].GObj.x, Bunkers[j].GObj.y, Bunkers[j].image[Bunkers[j].GObj.life], 0); 
	}
}

void DrawEnemies(void){unsigned char i;
	 for(i=0;i<12;i++){
		//CheckEnemyRegMissileCollisions(i);
    if(Enemy[i].GObj.life > 0){
     Nokia5110_PrintBMP(Enemy[i].GObj.x, Enemy[i].GObj.y, Enemy[i].image[FrameCount], 0);
		}
		else{
			if(Enemy[i].explode){
					Nokia5110_PrintBMP(Enemy[i].GObj.x, Enemy[i].GObj.y, SmallExplosion0, 0);
				  Enemy[i].explode = 0;
			}
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

void DrawLasers(void){unsigned char i;
	for(i=0;i<MAX_LASERS;i++){
    if(Lasers[i].GObj.life){
     Nokia5110_PrintBMP(Lasers[i].GObj.x, Lasers[i].GObj.y, Lasers[i].image, 0);
		}
  }
}

void Draw_GameFrame(void){
  Nokia5110_ClearBuffer();
	DrawPlayer();
	DrawBunkers();
  DrawEnemies();
	DrawRegMissiles();
	DrawSpecMissiles();
	DrawLasers();
  Nokia5110_DisplayBuffer();      // draw buffer
  FrameCount = (FrameCount+1)&0x01; // 0,1,0,1,...
}

unsigned long Set_Difficulty(void){
	/*return 2666666 - KilledEnemyCount*166666; //2666666 corresponds to period of SysTick interrupt with 30 Hz frequency
																						//12 (max number of killed enemies) * 166666 approximately equals 2666666* (3/4)
																						//hence period varies from 2666666 to 2666666/4 making frequency vary from 30 Hz to 120Hz
	*/
	//Increased returned period to make game run at a more playable speed
	return 2666666*4 - KilledEnemyCount*666666; //2666666*4 corresponds to period of SysTick interrupt with 7.5 Hz frequency
																						//12 (max number of killed enemies) * 666666 approximately equals (2666666*4)* (3/4)
																						//hence frequency varies from 7.5 Hz to 30 Hz
}

//returns 1 if game over; 0 otherwise
unsigned char Check_GameOver(void){
	if((KilledEnemyCount == 12) || (Player.GObj.life == 0))
		return 1;
	return 0;
}

//Output the frame for the Game Over State
void State_GameOver(void){
	Nokia5110_Clear();
  Nokia5110_SetCursor(1, 0);
  Nokia5110_OutString("GAME OVER");
  Nokia5110_SetCursor(1, 1);
	if(KilledEnemyCount == 12){
		Nokia5110_OutString("You Won!");
		Nokia5110_SetCursor(1, 2);
		Nokia5110_OutString("Good job,");
	}
	else{
		Nokia5110_OutString("You Lost!");
		Nokia5110_SetCursor(1, 2);
		Nokia5110_OutString("Nice try,");
	}
  Nokia5110_SetCursor(1, 3);
  Nokia5110_OutString("Earthling!");
	Nokia5110_SetCursor(1, 4);
  Nokia5110_OutString("Score:");
  Nokia5110_SetCursor(7, 4);
  Nokia5110_OutUDec(Score);
}

