// Original ultransonic author for this project: Chris Born
// Edited into class: Jason Cowin
// Using on Sonar Class from : https://www.youtube.com/watch?v=NZSQcRe3NYg -- Author: NGH

// Output: sensed - boolean value wether something was in range or not 
// Action: When called, this function will send out an ultrasonic pulse and listen
//         for an echo to determine if a person / object is within range.

#ifndef _Sonar_h
#define _Sonar_h

class Sonar{
  //These wont change after being set by constructor
  int trigPin;
  int echoPin;
  int maxDistance;                            // Maximum range allowed in mm
  
  // These can change
  int thresholdDistance;
  bool enabled;
  int distanceOptions[5] = {0, 50, 100, 150, 200};  // Distance in mm
  unsigned long delayTime;                          // time in ms after sensor last sense before spraying
  unsigned long delayTimeOptions[5] = { 0, 2500, 5000, 10000, 20000};     // Default options, user selectable via button
  int currentDistance;

  public:
  Sonar(int trigPin, int echoPin, int thresholdDistance, bool enabled = true, unsigned long delayTime = 2500){
    
    this->trigPin = trigPin;
    this->echoPin = echoPin;
    this->maxDistance = 500;          // 500 mm - max distance sensor will detect      
    this->thresholdDistance = thresholdDistance;      // detection setting
    this->enabled = enabled;
    this->delayTime = delayTime;
    
    pinMode(trigPin,OUTPUT);
    pinMode(echoPin,INPUT);
    digitalWrite(trigPin,LOW);

    //Serial.print(" trig / echo / thres / enable / delay");
    //Serial.print(trigPin,echoPin
    }

  int Measure(){
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);
    int echoTime = pulseIn(echoPin,HIGH,5000);
    currentDistance = echoTime*0.1715;
    if (currentDistance == 0){
      currentDistance = maxDistance;
      }
    return currentDistance;
    }

  bool Detected(){
    if(enabled && Measure() < thresholdDistance){
      return true;
    }else{
      return false;
    }
  }

  void SetThreshold(int thresholdDistance){
    // Allows for custom threshold values to be set.
    if(thresholdDistance == 0){
      // Turn off sensor
      Disable();
    } else {
      Enable();
      this->thresholdDistance = thresholdDistance;
    }
  }

  void SetThresholdIndex(int index){
    if(0 <= index <= 5){
      thresholdDistance = distanceOptions[index];
      if(thresholdDistance == 0){
        Disable();
      } else {
        Enable();
      }
    } else {
      // Trying to reach outside of allowed values
    }
  }

  int GetThreshold(void){
    return thresholdDistance;
  }
  
  int GetThresholdIndex(void){
    for (int i=0; i<5; i++){
      if (distanceOptions[i] == thresholdDistance){
        return(i);  /* it was found */
        }
    }
    return(0);  /* if it was not found */
  }
  
  void Enable(){
    enabled = true;
  }
  
  void Disable(){
    enabled = false;
  }
  
  void SetDelayTime(unsigned long delayTime){
    this->delayTime = delayTime;
  }
  
  void SetDelayTimeIndex(int index){
    if (0 <= index <= 5) {
      delayTime = delayTimeOptions[index];
    } else {
      // Trying to reach outside of allowed values
    }
  }
  
  int GetDelayTime(void){
      return delayTime;
  };
  
  int GetDelayTimeIndex(void){
      for (int i=0; i<5; i++){
        Serial.println(delayTimeOptions[i]);
        Serial.println(delayTime);
        if (delayTimeOptions[i] == delayTime){
          return(i);  /* it was found */
          }
      }
      return(-1);  /* if it was not found */
  }


};
// End sonar Class
#endif
