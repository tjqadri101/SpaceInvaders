// Sound.c
// Runs on any computer
// Sound assets based off the original Space Invaders and basic
// functions to play them.  Copy these functions and constants
// into your SpaceInvaders.c for ease of sharing your game!
// Jonathan Valvano
// November 19, 2012

#include "DAC.h"
#include "Timer0.h"
#include "Sound.h"
#include "SoundWaves.h"
#include "tm4c123gh6pm.h"

unsigned long Index = 0;
const unsigned char *Wave;
unsigned long SoundCount = 0;
void Play(void){
  if(SoundCount){
    DAC_Out(Wave[Index]>>4);
    Index = Index + 1;
    SoundCount = SoundCount - 1;
  }else{
  NVIC_DIS0_R = 1<<19;           // disable IRQ 19 in NVIC
  }
}
void Sound_Init(void){
  DAC_Init();               // initialize simple 4-bit DAC
//  Timer0B_Init(&Play, 20000); // 4 kHz
  Timer0_Init(&Play, 80000000/11025);     // 11.025 kHz
  Index = 0;
  SoundCount = 0;
//   while(1){
//     DAC_Out(2048);
//   }
}
void Sound_Play(const unsigned char *pt, unsigned long count){
  Wave = pt;
  Index = 0;
  SoundCount = count;
  NVIC_EN0_R = 1<<19;           // 9) enable IRQ 19 in NVIC
  TIMER0_CTL_R = 0x00000001;    // 10) enable TIMER0A
}
void Sound_Shoot(void){
  Sound_Play(shoot,4080);
}
void Sound_Killed(void){
  Sound_Play(invaderkilled,3377);
}
void Sound_Explosion(void){
  Sound_Play(explosion,2000);
}
void Sound_Fastinvader1(void){
  Sound_Play(fastinvader1,982);
}
void Sound_Fastinvader2(void){
  Sound_Play(fastinvader2,1042);
}
void Sound_Fastinvader3(void){
  Sound_Play(fastinvader3,1054);
}
void Sound_Fastinvader4(void){
  Sound_Play(fastinvader4,1098);
}
void Sound_Highpitch(void){
  Sound_Play(highpitch,1802);
}
