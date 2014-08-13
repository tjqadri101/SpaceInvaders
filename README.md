SpaceInvaders
=============

For this project, I designed and built an embedded system for the 80's arcade game, called Space Invaders. This was the final project (optional) of Massive open online course (MOOC) titled "UT.6.01x Embedded Systems - Shape the World". This MOOC was offered by University of Texas at Austin on edx.org and was taught by Dr. Jonathan Valvano and Dr. Ramesh Yerraballi. The course was completeled on May 22nd, 2014; however, all the course material, including lecture videos, quizzes, and labs, were archived on the edx website and I used the archived material to go through the entore course. This project had a requirements documemt (provided by the course professors) and I have inculded that document in the file titled [RequriementsDocument.md](https://raw.githubusercontent.com/tjqadri101/SpaceInvaders/master/RequriementsDocument.md). The professors provided the starter code for this project and also suggested a [data flow graph](https://raw.githubusercontent.com/tjqadri101/SpaceInvaders/master/Lab15DataFlowGraph.png) and a [flow chart](https://raw.githubusercontent.com/tjqadri101/SpaceInvaders/master/Lab15FlowChart.png) for the project. I adhered to their suggested flow methodolgy by outputting to the 4 bit DAC for sound in the Timer2A interrupt service routine (ISR). The Timer2A ISR ran at 11.025 kHz. The time to turn on the LEDS representing a success (e.g. killing an enemy) and a failure (e.g. bunker being destroyed) was also determined, via counters, through the Timer2A ISR. The rest of the game engine ran through the SysTick ISR. The SysTick ISR had a frequency between 30 Hz - 120 Hz. This frequency depended on the number of killed enemies. As the number of killed enemies increase, the SysTick ISR frequency also increased; thereby causing the remaining enemies to move faster. This mechanism allowed the creation of varying difficult as the game progressed. The starter code had the support routines for the LCD graphics. The rest of the code came from the work I finished in the previous labs of the course as well as new coded added when needed.

This project had the following objectives (as stated by the instructions on the course website): 1) learn modular design by putting all the components of the class into one system; 2) the development of a second interrupt service routine (ISR) using one of the timers (the first being the Systick ISR which was already developed previously in the course), creating three threads; 3) learn how to represent and maniputate images as two-dimensional arrays; 4) design, test, and debug a large C program; and 5) review I/O interfacing techniques used in this class. 

The general description of the game can be found in the RequirementsDocument.md file. The following were the specific system requirements: 1) The game must run on a real TM4C123 LaunchPad microcontroller, or run in the simulator with LaunchPadDLL version 1.0.0.6 (or higher). 2)The game must compile in Keil IDE. 3) The project may use the Keil simulator, the real Nokia5110 LCD, or the virtual Nokia (UART+TExaSdisplay) interface. 4) The compilation must occur with the 32k-limit free version of Keil. 5)The slide pot must be attached to PE2/AIN1. 6)Two buttons must be attached to PE1 and PE0. 7)Two LEDs must be attached to PB5 and PB4. 8)The 4-bit DAC must be attached to PB3-0. 9)If the real Nokia5110 is used, it must be interfaced to PA7, PA6, PA5, PA3, and PA2. 10) If the virtual Nokia5110 is used, then TExaSdisplay version 1.08 (or higher) must be running. I used the real Nokia5110 LCD in my project.

For reference, I have included the [TM4C123 Launchpad manual](https://github.com/tjqadri101/SpaceInvaders/raw/master/TM4C123_LaunchPadUsersManual.pdf) and the [TM4C123 Launchpad datasheet](https://github.com/tjqadri101/SpaceInvaders/raw/master/TM4C123_Datasheet.pdf).

In this project, object refers to either a function or a data element. A public object is one that is shared by multiple modules. This means a public object can be accessed by other modules. Following convention and course instructions, the most general functions of a module are made public, so the functions can be called from other modules. For a module performing I/O, typical public functions include initialization, input, and output. A private object is one that is not shared. I.e., a private object can be accessed by only one module. Following convention and course instructions, the internal workings of a module were madeprivate, to hide how a private function works from user of the module. The software in this course uses the naming convention of using the module name followed by an underline to identify the public functions of a module. For example if the module is ADC, then ADC_Init and ADC_Input are public functions. Functions without the underline in its name are private. In this manner one can easily identify whether a function or data object as public or private.

Before assembling the final hardware, I debugged the system in simulation on the Keil IDE. I began by debugging each module separately. The GameEngine.c had the major functions for the actual game engine which ran in SysTick interrupt service routine. After adding a major function, such as movement, or collision detection, I debugged in simulation. The next link shows a simulation run of the final software version. 
[Simulation run of final software version](http://youtu.be/NSUf1PLbaTU)

