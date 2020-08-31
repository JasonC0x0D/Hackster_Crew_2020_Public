#ifndef _Pump_h
#define _Pump_h

class Pump {
  private:
    int pin;
    int dutyCycle;
    int frequency;

  public:

    Pump(int pin, int dutyCycle = 25, int frequency = 1){
      // duty cycle (0-100)
      // frequency hertz [cycles/second] 
      this->pin = pin;
      this->dutyCycle = dutyCycle;
      this->frequency = frequency;
      Init();
    }

    void Init() {
      pinMode(pin, OUTPUT);
      // Always try to avoid duplicate code.
      // Instead of writing digitalWrite(pin, LOW) here,
      // call the function off() which already does that
      Off();
    }

    void On(int duration){
      unsigned long endTime = millis() + 1000*duration;
      
      // TODO: make this overflow proof ---------------------------------------------------<<<<
      while(millis() <= endTime){
        Serial.print("Time: ");
        Serial.print(millis());
        Serial.print(" ---- End Time: ");
        Serial.println(endTime);
        Serial.println((2.0/frequency)*dutyCycle*1000);
        Serial.println((2.0/frequency)*(1-dutyCycle)*1000);  
        Serial.println(dutyCycle);
        Serial.println(2/frequency);      
        digitalWrite(pin, HIGH);
        delay(dutyCycle*10);
        Off();
        delay((100-dutyCycle)*10);
      }
      Off(); // Make sure pump stops in the off state  
    }

    void Off(void){
      digitalWrite(pin,LOW);
    }
};

#endif
