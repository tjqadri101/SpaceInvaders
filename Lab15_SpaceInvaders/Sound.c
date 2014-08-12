// Sound.c
// Runs on any computer
// Sound assets based off the original Space Invaders and basic
// functions to play them.  Copy these functions and constants
// into your SpaceInvaders.c for ease of sharing your game!
// Jonathan Valvano
// November 19, 2012

//Modified by Talal Javed Qadri
//August 12, 2014

#include "DAC.h"
#include "Sound.h"
#include "SoundWaves.h"
#include "tm4c123gh6pm.h"

unsigned long Index = 0;
const unsigned char *Wave;
unsigned long SoundCount = 0;

//Periodic function passed to Timer2A 
//It determines whether a sound should play or not
void Sound_Play(void){
  if(SoundCount){
    DAC_Out(Wave[Index]>>4);
    Index = Index + 1;
    SoundCount = SoundCount - 1;
  }else{
		DAC_Out(0);
  }
}
void Sound_Init(void){
  DAC_Init();               // initialize simple 4-bit DAC
  Index = 0;
  SoundCount = 0;
}
void SoundOut(const unsigned char *pt, unsigned long count){
  Wave = pt;
  Index = 0;
  SoundCount = count;
}
void Sound_Shoot(void){
  SoundOut(shoot,4080);
}
void Sound_Killed(void){
  SoundOut(invaderkilled,3377);
}
void Sound_Explosion(void){
  SoundOut(explosion,2000);
}
void Sound_InvaderShoot(void){
  SoundOut(invadershoot,1802);
}
