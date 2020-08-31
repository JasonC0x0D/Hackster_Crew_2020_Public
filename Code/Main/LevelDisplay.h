#ifndef _Level_Display_h
#define _Level_Display_h



class LevelDisplay{
  #include "Led.h"
  private:
    Led leds[4];
    //Led ledOne;
    //Led ledTwo;
    //Led ledThree;
    //Led ledFour;

  public:
    LevelDisplay(byte levelOnePin, byte levelTwoPin, byte levelThreePin, byte levelFourPin) {
      Led ledOne(levelOnePin);
      leds[0] = ledOne;
      Led ledTwo(levelTwoPin);
      leds[1] = ledTwo;      
      Led ledThree(levelThreePin);
      leds[2] = ledThree;
      Led ledFour(levelFourPin);
      leds[3] = ledFour;
      //this->leds[1] = Led ledTwo(levelTwoPin);
      //this->leds[2] = Led ledThree(levelThreePin);
      //this->leds[3] = Led ledFour(levelFourPin);

      //Led ledOne(levelOnePin);
      
      Off();
    }

    void On (int numberOn){
      switch(numberOn){
        case 0:
            leds[0].Off();
            leds[1].Off();
            leds[2].Off();
            leds[3].Off();
        break;
        
        case 1:
            leds[0].On();
            leds[1].Off();
            leds[2].Off();
            leds[3].Off();
        break;

        case 2:
            leds[0].On();
            leds[1].On();
            leds[2].Off();
            leds[3].Off();
        break;

        case 3:
            leds[0].On();
            leds[1].On();
            leds[2].On();
            leds[3].Off();
        break;

        case 4:
            leds[0].On();
            leds[1].On();
            leds[2].On();
            leds[3].On();
        break;      
      }
    }

    void Percent(int percent){
      if(percent > 90){
        On(4);
      } else if (percent > 65){
        On(3);
      } else if (percent > 40){
        On(2);
      } else if (percent > 15){
        On(1);
      } else {
        On(0);
      }
    }

    void Off(void){
      for(int i = 0; i < 4; i ++){
        leds[i].Off();
      }
    }

    void On(){
      for(int i = 0; i < 4; i ++){
        leds[i].On();
      }
    }
     
};
#endif
