WSU CS 466 README
=================
Is intended to help setup the build environment for class so that we all do start from the same environment.

All these instructions are given for Linux.  If you don't have a distro you prefer I reccommend Ubuntu Mate as it has a better desktop than default Ubuntu (IMHO).  The best all around development machine is a native Linux laptop.  I run Ubuntu Mate 22.04 on my Lenovo that allows me to develop across many targets.  

Students have installed Linux on a VM with some success (Issues generally center on USB support/issues), Also some students have worked under WSL but I have never used it and thy seemed to work very hard to keep the class tools running.  It may be better now.

While there are arm-none-eabi compilers for windows I don't reccommend it and I will be very little help.. Windows for me is a platform to edit spreadsheets and maybe run teams..  As an embedded developer for bare-metal or embedded linux I only use (and understand) Linux development systems.

I locate all my development projects in a directory ```~/src``` Doing so allows me a consistant location scheme on any computer.  In linux the tilde character indicates your home directory.

It's not necessary that you use ```src``` as your parent directory but pick a directory that you treat as src of your own and install our class repo and development tools for class below it.  

If you experiance issues in lab it will help me isolate where the issue lies.  It 'should' also make all my ```make``` and ```cmake``` files work straight off without modification.

![Capture](https://user-images.githubusercontent.com/19733331/150585134-7ebbc1ad-c76b-4c19-bdd8-8922a14eacbe.JPG)

You will need git installed and possibly other applications as well (git, cmake, ) use ```sudo apt install``` for missing packages

 - The pico directory that I create below is a peer to the class CS466_S26 directory that you can install with the following commands
	- ```cd ~/src``` (or your replacement for src)
	- ```git clone https://github.com/milhead2/cs466_s26.git```
	
This file is the README.md from the class repo.

Build Environment Installation Steps
------------------------------------

 - Download SDK, examples and ARM compiler
	- ```cd ~/src``` (or your replacement for src)
	- ```mkdir pico```
	- ```cd pico```
	- ```git clone https://github.com/raspberrypi/pico-examples.git```
	- ```git clone https://github.com/raspberrypi/pico-sdk.git```
	- ```export PICO_SDK_PATH=~/src/pico/pico-sdk```  Also add this export to your .bashrc file, (I just append them at the bottom if the file)
		- ```echo "export PICO_SDK_PATH=~/src/pico/pico-sdk" >> ~/.bashrc```  Be very sure you append with ```>>`` or you can overwrite your bashrc file.
	- ```cd ~/src```

 - If you do not have the ARM GCC cross compiler installed you can get it from here..
	- Verify that you can see the installed compiler in your path `arm-none-eabi-gcc --version`
 	- You don't need the latest compiler that I document installation of below.
	- If you get a compiler verison 
		```
		$ arm-none-eabi-gcc --version
		arm-none-eabi-gcc (GNU Arm Embedded Toolchain 10.3-2021.10) 10.3.1 20210824 (release)
		Copyright (C) 2020 Free Software Foundation, Inc.
		This is free software; see the source for copying conditions.  There is NO
		warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
		```
		Then you can skop the rest of this section.  Otherwise Continue here.
		
	- ```sudo apt update```
    - ```sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi```
    - ```sudo apt install gdb-multiarch```

	- Make sure that ths binary directory of this compiler is in your path
	- Verify that you can see the installed compiler in your path
	- ``` arm-none-eabi-gcc --version```
	- Should produce the output similar to below, Your version may vary...
		```
    $ arm-none-eabi-gcc --version
    arm-none-eabi-gcc (15:13.2.rel1-2) 13.2.1 20231009
    Copyright (C) 2023 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
		```
- Build the SDK and examples dir.  This may require additional software installs...  The compiler install is below.
    - ```cd .../src/pico/pico-examples```
	- ```git submodule update --init```  
	- ```cmake .```
	- ```make```
    - ```cd ../pico-sdk```
	- ```git submodule update --init```  
	- ```cmake .```
	- ```make```
	

Build The Provided Lab One Code
---------------------------

- We will using cmake to generate a Makefile 

	- You should have you CS466_S26 repo in a peer directory to the pico tools.  For me that is ```~/src/cs466_s26``` and the pico tools in ```~/src/pico```
	- Create a build directory and make it your current dir
		- ```cd ~/src/cs466_s26/lab1```
		- ```mkdir build```
		- ```cd build```
	- Call ```cmake``` to generate a makefile
		- ```cmake ..```
	- Then use ```make``` to build the SDK and the lab1 code
		- ```make```
	- If the make is successful, the file ```lab1.uf2``` will be created in the build directory.  To execute that file reboot the pico into 
	drive-loader-mode by booting with the button pressed and copy the ```lab1.uf2``` to the faux drive that the pico creates.
		- ```cp lab1.uf2 /media/miller/RPI-RP2```
	- Note that you can easily perform a make and copy in one command line
		1. Make sure that your pico is rebooted as a USB Drive
		2. ```$ make && cp lab1.uf2 /media/miller/RPI-RP2``` <sub>(but use the path to **your** pico)</sub>
		
## FreeRTOS and the following is only used for Lab2 and beyond... (*..In yer best Buzz Lightyear voice..*)

Add FreeRTOS to directory tree
---------------------------

- In order to build lab2 and later projects you will need to add the latest freertos directory to your dev disk.  
	- Download the freertos image from https://www.freertos.org/lts-libraries.html
	- set your current directory to your parent dir.
	- run the command to extract the archive...
		- ```unzip ~/Downloads/FreeRTOSv202406.04.zip   #(You may need to adjust version)```
	- It must land in the correct place as a peer to the cs466_s26 directory.

Build the lab2 husk code
---------------------------

- You should now be able to build the lab2 code.
	- ```cd ~/src/cs466_s26/lab2```
	- ```mkdir build```
	- ```cd build```
	- ```cmake ..```
	- ```make```


Backup Information (ignore below)
---------------------------------
	
		
