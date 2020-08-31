// Start Led Class

#ifndef _Led_h
#define _Led_h

class Led {
  private:
    byte pin;
    bool inverted;
  public:
    Led(byte pin = 0, bool inverted = false) {
      // Use 'this->' to make the difference between the
      // 'pin' attribute of the class and the 
      // local variable 'pin' created from the parameter.
      this->pin = pin;
      this->inverted = inverted;
      Init();
    }
    void Init() {
      pinMode(pin, OUTPUT);
      // Always try to avoid duplicate code.
      // Instead of writing digitalWrite(pin, LOW) here,
      // call the function off() which already does that
      Off();
    }
    void On() {
      if(inverted == false){
        digitalWrite(pin, LOW);
      } else {
        digitalWrite(pin, HIGH);
      }
    }
    void Off() {
      if(inverted == false){
        digitalWrite(pin, HIGH);
      } else { 
        digitalWrite(pin, LOW);  
      }
    }
    bool Status() {
      if(inverted == false){
        return(!digitalRead(pin));        
      } else {
        return(digitalRead(pin));        
      }
    }
    void Toggle() {
      if(Status()){
        Off();
      } else{
        On();
      }
    }
}; // don't forget the semicolon at the end of the class
// End LED class
#endif
