# Hackster_Crew_2020

This will be the code repository for the competition.

ToDo List:

Important:
1) Build Housing / Assembly
	- Button Covers
2) Determine Parameters
	- Spray Time (Durration / Duty Cycle / Frequency)
	- Delay after hand detected
	- Ultrasonic sensor ignore time
3) DONE!!!
4) DONE!!!
5) Button class needs built in debouncing
	- Might have started that
6) Config for Spray Button	
	- Determine sensor distance or off
	- 4 distance levels + 1 off levels
	- Use the Level LEDs for indication
7) Config Wifi
	- Goes into WiFi host mode
	- User connects and inputs SSID and password
	- Also inputs variables used to connect to Email Service
	- Saves values (non-volitle)
	- On exit attempts to connect to input WiFi
8) Send Email
	- When it needs to be filled
9) Documentation for Hackster
	- Need to complete project write-up
	- Wiring Diagrams
	- Code Diagrams
	- Bill of Materials	
	- 3D Part Drawings / Files
	- Story
	- Video Demo
10) Test Assembly / Trial
	-
Completed:
3) Code in delay after hand detected	
	- After Y time then 
4) Allow misses in ultrasonic sensor
	- Allow for X number of missed signals in a row before turning off



Strech Goals:
1) Online Dashboard
	- Statistics on device usage
	- Estimated battery / fluid levels
	- Ability for tracking multiple devices from one page
2) Online Configuration
	- Modify the device configuration via online portal 
	- This might a bad idea / security risk
3) Debug Power Button
	- Currently needed to power on device, but will only reset not power off
	- Could not debouncing the switch be causing problems?
	- Tested on atleast 2 boards
4) Debug Capacitive Touch
	- Seems to draw too much power as arduino power light is very dim
	- Arduino doesn't run
	- Only tested on one board
5) Blink function in Leds
	- Just a nice feature
	- Inputs (Time / Frequency)
6) Dim function Leds
	- Might need pins to be on PWM which they are not all on
	- Nano Every - All digital are PWM
	- Nano IOT - Some pins are PWM
7) WiFi Functionality
	- Next PCB revision might be nice to have a WiFi connected Led
	- Could use Level LEDs to display signal strength
8) Send Usage Email
	- Midnight, if powered on
	- User configurable if they want it
	- Tracks device usage / spray / etc
9) Biological Testing with E-coli
	- Verify this device actually kills stuff
10) Battery Life Testing
	- How long device will last on a 1 or 2 cell powerbank
11) Allow the spray delay to be configurable
	- Use the "fill button" configuration mode
	- 4 levels (0 2.5 5 7.5 10)
	- Maybe 4 other levels for "doors" that might need more time to close
Completed:
	