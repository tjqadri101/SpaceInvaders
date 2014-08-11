#Requirements Document

##1. Overview

  1.1. Objectives: Why are we doing this project? What is the purpose? The overall objective of this project is to integrate the individual components taught in this class into a single system. More specifically, the objectives of this project are: 1) design, test, and debug a large C program; 2) to review I/O interfacing techniques used in this class; and 3) to design a system that performs a useful task.  In particular we will design an 80’s-style shoot-em up game like Space Invaders.

  1.2. Process: How will the project be developed? Similar to the labs, this project will have a starter project, Lab15_SpaceInvaders, which will include some art and sounds to get you started.

  1.3. Roles and Responsibilities: Who will do what?  Who are the clients? Students may develop their games individually or in teams. An effective team size for this project ranges from 1 to 3 members. There is no upper limit to team size, but above 3 members will present difficulty in communication and decision making. The clients for this project will be other classmates and your professors of the UT.6.01x course.

 
  1.4. Interactions with Existing Systems: How will it fit in? The game must be developed in C on the Keil IDE and run on a Stellaris/Tiva LaunchPad. We expect you to combine your solutions to Lab 8 (switches, LED), Lab 12 (interrupts), Lab 13 (DAC and sounds), and Lab 14 (slide pot and ADC) into one system. We expect everyone to use the slide pot, two switches, two LEDs, one 4-bit DAC, and the Nokia5110 LCD screen. If you do not own a Nokia5110 there will be a mechanism to pass the Nokia graphic commands to the PC and the application TExaSdisplay will show the images in real time as your game software runs on your real LaunchPad.

  1.5. Terminology: Define terms used in the document. BMP is a simple file format to store graphical images. A sprite is a virtual entity that is created, moves around the screen, and might disappear. A public function is one that can be called by another module. For example, since the main program is allowed to call Sound_Play, Sound_Play is a public function.

  1.6. Security: How will intellectual property be managed? Since this project does not contribute to the final grade in UT.6.01x you do not need to upload your solution. If you do upload your source code, then other students who have uploaded will be able to see and download your source code. To reduce the chance of spreading viruses, we will restrict the upload to a single text-formatted source code file. More specifically, the upload must be a SpaceInvaders.c file, and this file must compile within a project like the Lab15_SpaceInvaders starter project within the 32k-limit of the free version of the Keil IDE.

##2. Function Description

  2.1. Functionality: What will the system do precisely? You will design, implement and debug an 80’s or 90’s-style video game. You are free to simplify the rules but your game should be recognizable as one of these six simple games: Space Invaders, Asteroids, Missile Command, Centipede, Snood, or Defender. Buttons, and the slide pot are inputs. The Nokia5110 LCD, LEDs, and sound (Lab 13) are the outputs. The slide pot is a simple yet effective means to move your ship. Interrupts must be appropriately used control the input/output, and will make a profound impact on how the user interacts with the game. You could use an edge-triggered interrupt to execute software whenever a button is pressed. You could create two periodic interrupts. Use one fixed-frequency periodic interrupt to output sounds with the DAC. You could decide to move a sprite using a second periodic interrupt, although the actual LCD output should always be performed in the main program.

  2.2. Scope: List the phases and what will be delivered in each phase. The first phase is forming a team and defining the exact rules of game play. You next will specify the modules: e.g., the main game engine, a module to input from switches, a module to output to LEDs, a module to draw images on the Nokia5110, and a module that inputs from the slide pot. Next you will design the prototypes for the public functions. At this phase of the project, individual team members can develop and test modules concurrently. The last phase of the project is to combine the modules to create the overall system.

  2.3. Prototypes: How will intermediate progress be demonstrated? In a system such as this each module must be individually tested. Your system will have four or more modules. Each module has a separate header and code file. For each module create a header file, a code file and a separate main program to test that particular module. After the game is completed, you will create a single C file that compiles within the starter project to be uploaded for others to play.

  2.4. Performance: Define the measures and describe how they will be determined. The game should be easy to learn, and fun to play. You will have the option to share your game with other students in the class.

  2.5. Usability: Describe the interfaces. Be quantitative if possible. In order to allow other students to download, compile and run your game you must follow strict requirements for the pins used for input and output. These requirements are detailed at the top of the SpaceInvaders.c file within the starter project.

  2.6. Safety: Explain any safety requirements and how they will be measured. To reduce the chance of spreading viruses we will only allow SpaceInvaders.o files with TM4C123 object code to be uploaded. The usual rules about respect and tolerance as defined for the forums apply as well to the output of the video games.

##3. Deliverables

  3.1. Reports: How will the system be described? You will create and upload a 60-sec YouTube video describing how the game works. Additional comments can be added to the description section of the YouTube upload explaining the purpose and functionality of your game.

  3.2. Audits: How will the clients evaluate progress? There will be a discussion forum that will allow you to evaluate the performance (easy to learn, fun to play) of the other games.

  3.3. Outcomes: What are the deliverables? How do we know when it is done? There are three options for Lab 15. First, Lab 15 is completely optional and you are not required to develop or share your game.
Second, if you wish to show your game to others, you will create a YouTube video, and upload a YouTube link to a Goggle form hosted on the class Google Drive. We will create a YouTube channel of all the videos submitted. This will allow others to watch your video and upvote the ones they like.
Third, if you wish to allow others to play your game, you must follow strict hardware specifications listed in the stater project. Next, you must store your SpaceInvaders.o  somewhere on the web and provide a URL to the SpaceInvaders.o file. Other students can download your object file and play your game on their hardware. We will disable uploads and downloads on the last class day.

