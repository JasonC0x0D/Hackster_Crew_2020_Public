// Jordan rocks //

//---------- Pins --------------------------------//
//#define 0                 // Reserved Serial TX
//#define 1                 // Reserved Serial RX
#define butSprayPin 2          // PWM
#define butFillPin 3           // PWM
#define butWifiPin 4
#define ledCleanPin 5          // PWM
#define ledDirtyPin 6          // PWM
#define ledSensePin 7        
#define ledLevel1Pin 8     
#define ledLevel2Pin 9         // PWM
#define ledLevel3Pin 10        // PWM
#define ledLevel4Pin 11
#define pinEcho 12
#define pinTrig 13         // Note: Also Conneced to onboard LED
//#define 14                // A0
//#define 15                // A1
//#define 16                // A2
//#define 17                // A3
//#define 18                // A4
//#define 19                // A5
#define ledDirtyNozzlePin 20   // A6
#define pumpPin 21          // A7

//---------- Finite State Machine Definitions ---//
enum mainStates{
  caseName,         // Place holder for template state
  monitoring,
  verify,
  spray,
  sanitize,
  buttonState,
  timingConfig,
  distanceConfig,
  wifiConfig,
  senseConfig  
};


enum subStates{
  subSpray,
  subFill,
  subWifi
};


int maxSonarMissed = 5;
int missedDetections = 0;
unsigned long minSonarTime = 500;
unsigned long delayBeforeSpray = 2000;
unsigned long lastSonarTime = 0;
unsigned long startSonarTime = millis(); 
unsigned long startSanitizeTime = 0;
unsigned long requiredSanitizeTime = 2;  // Should be 30 seconds
 

// For debugging (10 rows, 25 charaters max per row)
char charMainStates [10][25] = {
  "caseName",
  "monitoring",
  "verify",
  "spray",
  "sanitize",
  "buttonState",
  "timingConfig",
  "distanceConfig",
  "wifiConfig",
  "senseConfig"
};


//---------- Global Variables -------------------//
int handCount; // number of cylces hand is in range
unsigned long last = 0; // used to track cycle ty

unsigned long handRange = 10; // distance of sensor detection range
unsigned long handCycles = 250; // number of loop cycles hand need to be in handRange
int sonarThreshold = 100;  // 100mm - threshold to detect an object
int mainState = monitoring;
int subState = 0;
int detectCount = 0;     // 
int sensorDetectsTheshold = 10;  // Sonar Sensor must detect 10 times before it will register / pad dirty
int spray_count; // "spray_count" indicates how many times the device has been used.
bool buttonPressed;     // If a button has been pressed (acts like a latch)
int myIndex = 0;        // Used to find index of values in arrays (-1 means not found)

//---------- Classes ----------------------------//
#include "Button.h"
#include "Led.h"
#include "Pump.h"
#include "Sonar.h"
#include "LevelDisplay.h"

//---------- Libraries -------------------------//
//#include <WiFiNINA.h>


// --------------- End Classes ------------ //

// ------ Objects -------------------//
  Sonar sonarSensor(pinTrig,pinEcho,sonarThreshold);
  Pump  pump(pumpPin);
  
  Led ledSense(ledSensePin);
  Led ledDirty(ledDirtyPin);
  Led ledDirtyNozzle(ledDirtyNozzlePin, true);  // Inverted behavior
  Led ledClean(ledCleanPin);
  
  //Led ledLevel1(ledLevel1Pin);
  //Led ledLevel2(ledLevel2Pin);
  //Led ledLevel3(ledLevel3Pin);
  //Led ledLevel4(ledLevel4Pin);
  LevelDisplay ledLevels(ledLevel1Pin, 
                      ledLevel2Pin,
                      ledLevel3Pin, 
                      ledLevel4Pin);
  
  Button butSpray(butSprayPin);
  Button butFill(butFillPin);
  Button butWifi(butWifiPin);


void setup() 
{    
  Serial.begin(9600);

  verifyBoard();

}

void loop() {

// /*
  // For debugging purposes
  Serial.print("Main State: ");
  Serial.print(charMainStates[mainState]);
  Serial.print(" --- Sub State: ");
  Serial.print(subState);
  Serial.print(" --- Sonar Measure: ");
  Serial.print(sonarSensor.Measure());
  Serial.print(" --- Sonar Detected: ");
  Serial.println(sonarSensor.Detected());  
  
   
  // Check if button has been pressed
  if(buttonPressed == false){
    if(butSpray.Pressed() || butFill.Pressed() || butWifi.Pressed()){
      buttonPressed = true;  
      mainState = buttonState;
    }
  }
  


// ---------- Finite State Machine (F.S.M.) ----------//

  switch(mainState)
  {
    case monitoring:
      // Actions:
      //    1) Sense Person
      // Exits:      
      //    1) Person Sensed
      //    2) Button Pressed (Buttons should always change the main state to like a button state)
      
      if(sonarSensor.Detected()){
        mainState = verify;
        ledSense.On();            // Blue LED On
      }
      break;

    case verify:
      // Actions:
      //    1) Verify sensing was human
      // Exits:
      //    1) False Alarm -> monitoring
      //    2) True Alarm -> turn dirty led on & spray 

      switch(subState){
        case 0: // get start time  
          startSonarTime = millis(); 
          missedDetections = 0;
          subState = 1;
          break;

        case 1: // verify atleast minimum Sonar time that sensor detects
                // (to short could be something just passing though)
                // allow for max number of missed signals before missing
          if(sonarSensor.Detected()){
            if(millis() - startSonarTime > minSonarTime){
              subState = 2;
              lastSonarTime = millis();
            }  
          } else {
            missedDetections++;
            if(missedDetections > maxSonarMissed){
              // Too many missed signals, not "real" person
              mainState = monitoring;
              subState = 0;
              missedDetections = 0;
            }
          }
          break;

        case 2:  // Wait for Y time to pass w/o detection
          if(!sonarSensor.Detected()){
            if(millis() - lastSonarTime > delayBeforeSpray){
              mainState = spray;
              ledDirty.On();
              ledClean.Off();
              subState = 0;
            }
          } else {
            lastSonarTime = millis();
          }
          break;
      }
      break;

    case spray:
      // Actions:
      //    1) Spray pin pad / handle 
      //       Might be like 3 little mist cycles or something
      // Exits:
      //    1) Done Spraying -> Sanitize

      if(sonarSensor.Detected()){
        ledSense.On();
        // Stop?
      } else {
        ledSense.Off();
      }

      ledSense.Off(); // Without this it can get stuck on if one removes hand while pumping

      pump.On(2);
      mainState = sanitize;
      break;      

    case sanitize:
      // Actions:
      //    1) Wait required amount of time (30 seconds)
      // Exits:
      //    1) Time > 30 seconds -> Monitoring (turn off red light / turn on green light)
      switch(subState){
        case 0:
          startSanitizeTime = millis();
          subState = 1;
          break;
        case 1:
          if(millis() - startSanitizeTime > (requiredSanitizeTime * 1000)){
            mainState = monitoring;
            ledDirty.Off();
            ledDirtyNozzle.Off();
            ledClean.On(); 
          }
          break;
      }
      break;

    case buttonState:
      // Actions:
      //    1) Check which button to go to
      if(butSpray.Pressed()){
        subState = subSpray;
      } else if(butFill.Pressed()){
        subState = subFill;
      } else if(butWifi.Pressed()){
        subState = subWifi;
      }

      switch(subState){ // button subState
        
        case subSpray:
          if (butSpray.Pressed(3)){
            // Go to Timing Config
            mainState = timingConfig;
            subState = 0;
          } else if (butSpray.Unpressed()){
            // Go to Spray
            mainState = spray;
            subState = 0;
            buttonPressed = false;
          } else {
            // Wait for user to let go of button
            // or hold for longer than 3 seconds.
          }
          break;

        case subFill:
          if (butFill.Pressed(3)){
            // Go to Distance Config
            mainState = distanceConfig;
            subState = 0;
          } else if (butFill.Unpressed()){
            // Reset fill level and back to monitoring state
            // TODO Reset Fill Lvl
            mainState = monitoring;
            subState = 0;
            buttonPressed = false;
          } else {
            // Wait for user to let go of button
            // or hold for longer than 3 seconds.
          }
          break;

        case subWifi:
          if (butWifi.Pressed(3)){
            // Go to Wifi Config
            mainState = wifiConfig;
            subState = 0;
            buttonPressed = false;
          } else if (butWifi.Unpressed()){
            // Ultrasonic Sensor Config
            mainState = senseConfig;
            subState = 0;
            buttonPressed = false;
          } else {
            // Wait for user to let go of button
            // or hold for longer than 3 seconds.
          }
         break;
      } // End button SubState
      break;

    case timingConfig:
      // Actions:
      //    1) Choose Delay Time
            // Switch light using spray button taps
            // 0 sec - 0 lights
            // 2.5 sec - 1 light
            // 5 sec - 2 lights
            // 10 sec - 3 lights
            // 20 sec - 4 lights
      // Exits:
      //    1) Hold spray button for 2 seconds

      switch(subState){
        case 0:
          // Wait for user to release button
          // Turn lights on to show in config state
          ledSense.On();
          ledDirty.On();
          ledClean.On();
          
          if(butSpray.Pressed() == false){
            subState = 1; 
            Serial.print("Getting delay time index: ");
            myIndex = sonarSensor.GetDelayTimeIndex();
            Serial.println(myIndex);
              if(myIndex == -1){
                myIndex = 0;
              }
          }
          break;
        case 1:
          // Light up level LEDs to last delayTime level
          // If not found (custom time delay) function returns -1
          // Start user off at level 0

          ledLevels.On(myIndex);
          sonarSensor.SetDelayTimeIndex(myIndex);
            
          if(butSpray.Pressed()){
              subState = 2;
          }
          
          break;
        case 2:
          if(butSpray.Unpressed()){
            myIndex++;
            if (myIndex > 4){
              myIndex = 0;
            }
            subState = 1;
          } else if (butSpray.Pressed(2)){
            subState = 3; 
            ledSense.Off();
            ledDirty.Off();
            ledClean.Off();
            ledLevels.Off();
          }
          break;
        case 3:
          if(butSpray.Unpressed()){
            mainState = monitoring;
            subState = 0;
            buttonPressed = false;
          }
          break;
      }
      break; // End 

    case distanceConfig:
      // Actions:
      //    1) Choose Sensor Threshold Distance
            // Switch light using spray button taps
            // 0 - 0 lights - Sensor Off
            // 50 mm  - 1 light
            // 100 mm - 2 lights
            // 150 mm - 3 lights
            // 200 mm - 4 lights
      // Exits:
      //    1) Hold fill button for 2 seconds

          // Enable or disable the sonar -> manual spray mode or sensor spray mode option
          //if(ledDirty.Status() == 1){
          //  sonarSensor.Enable();
          //} else {
          //  sonarSensor.Disable();
          //}   
          
      switch(subState){
        case 0:
          // Wait for user to release button
          // Turn lights on to show in config state
          ledSense.On();
          ledDirty.On();
          ledClean.On();
          
          if(butFill.Pressed() == false){
            subState = 1; 
            myIndex = sonarSensor.GetThresholdIndex();
            if(myIndex == -1){
                myIndex = 0;
            }
          }
        break;
        case 1:
          // Light up level LEDs to last sensor threshold distance level
          ledLevels.On(myIndex);
          sonarSensor.SetThresholdIndex(myIndex);
            
          if(butFill.Pressed()){
              subState = 2;
          }
        break;
        case 2:
          if(butFill.Unpressed()){
            myIndex++;
            if (myIndex > 4){
              myIndex = 0;
            }
            subState = 1;
          } else if (butFill.Pressed(2)){
            subState = 3; 
            ledSense.Off();
            ledDirty.Off();
            ledClean.Off();
            ledLevels.Off();
          }
          break;
        case 3:
          if(butFill.Unpressed()){
            mainState = monitoring;
            subState = 0;
            buttonPressed = false;
          }
          break;
      };
      break; // End Distance Config
          
      
    case wifiConfig:
      // Actions:
      //    1)
      // Exits:
      //    1)
      

      
      switch(subState){
        case 0:
          // Config Wifi

          if(true){
            // Change true placeholder to value that is true when wifi config is done
            subState = 1;
          }
        break;

        case 1:   // Done Wifi Config      
          mainState = monitoring;
          buttonPressed = false;
          subState = 0;  
        break;
      break;
      }


    case senseConfig:
      // Actions:
      //    1)
      // Exits:
      //    1)
      
      mainState = monitoring;
      buttonPressed = false;        
      break;

    case caseName:
      // Actions:
      //    1)
      // Exits:
      //    1)
      break;

    default:
      Serial.println("Default");
      break;
    

  }
// ---------- End F.S.M. ---------------------------- //
//*/  
  

}
