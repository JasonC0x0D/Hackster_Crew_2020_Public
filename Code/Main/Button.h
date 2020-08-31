// Button class //

#ifndef _Button_h
#define _Button_h

class Button {
  int pin;
  int state;        // 1 = pressed / 0 = not pressed
  int lastState;
  
  unsigned long debounceDelay;
  unsigned long lastDebounceTime;
  unsigned long timeSinceChange;


public:
  Button(int pin) {
    this->pin = pin;
  }

  // (optional parameter) howLong[seconds] - returns true if button has been pressed as long or longer than howLong, else false
  bool Pressed(int howLong = 0){
    State();
    howLong = 1000*(unsigned long)howLong; // convert to milliseconds 
    /*
    Serial.print("How Long: ");
    Serial.print(howLong);
    Serial.print(" --- Current Millis: ");
    Serial.print(millis());
    Serial.print(" --- Last Debounce Time: ");
    Serial.print(lastDebounceTime);
    Serial.print(" --- ");
    */
    if( (state == 1) && ((millis() - lastDebounceTime) >= howLong) ){
      //Serial.println(" --- Returned True");
      return true;
    } else {
      //Serial.println(" --- Returned False");
      return false;
    }
  }

  // (optional parameter) howLong[seconds] - returns true if button has been unpressed as long or longer than howLong, else false
  bool Unpressed(int howLong = 0){
    State();
    howLong = 1000*(unsigned long)howLong; // convert to milliseconds 
    if(state == 0){
      if((millis() - lastDebounceTime) >= howLong){
      return true;
     }
    } else {
      return false;
    }
  }
  
  void State(){
    int reading = digitalRead(pin);

    if(reading != lastState){
      //reset debounce timer
      lastDebounceTime = millis(); 
    }
    if((millis() - lastDebounceTime) > debounceDelay){
      // whatever the reading is at, it's been there for longer than the debounce
      // delay, so take it as the actual current state:
  
      // if the button state has changed:
      if (reading != state) {
        state = reading;
      }
    }
    lastState = reading;
  }

  
};

#endif // _Button_h
