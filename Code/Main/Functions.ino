// ----------------------------------------------------------------- //
// ----- Start Functions ------------------------------------------- //
// ----------------------------------------------------------------- //



// --- Fluid Level State Function -------------- //
// --- by Jordan ------------------------------ //


//Input to the fluid level function is global variable "spray_count".
// "spray_count" indicates how many times the device has been used.
void fluidLevelSense(int spray_count) {

  //Variable used to track the fluid level. Can have values 0% - 100%.
  int fluid_level;

  //Variable used to denote if fluid level is ok or if it's empty and a notification is needed.
  String fluid_state;

  //Set number of sprays that a full liquid resevoir can handle. Example value is 300.
  int sprays_until_empty = 300;

  //Here the percentage of fluid level is calculated
  fluid_level = (spray_count/sprays_until_empty)*100;  // <-------------------Note: (from Jason 2 Jordan) fluid level is an int.
                                                       //                            if you want decimal points you will need a float
                                                       //                            otherwise arduino rounds.
                                                       //                            Also do you want: fluid_level = (1 - spray_count/sprays_until_empty)*100;

  // 1st of two fluid states. This state is for fluid level ok.
  if(fluid_level > 10){
    fluid_state = "fluid_level_ok";
  }
  //This fluid state is when the fluid is empty, then it initiates the email/text notification.
  if (fluid_level < 10){
    fluid_state = "fluid_empty";
  }
  if (fluid_state = "fluid_empty"){
    //put email notification code HERE.
  }
  //Fluid level greater than 75% turns on all 4 LED's.
  if (fluid_level > 75){
    //"turn on all 4 LEDs"
  }
  //Fluid level greater than 50% and less than 75% turns on 3 LED's.
  if ((fluid_level <= 75) and (fluid_level > 50)){
    //"turn on 3 LEDs"
  }
  //Fluid level is less than 50% and greater than 25% turns on 2 LED's.
  if ((fluid_level <= 50) and (fluid_level > 25)){
    //"turn on 2 LEDs"
  }
  //Fluid level less than 25% and greater than 0% turns on 1 LED.
  if ((fluid_level <= 25) and (fluid_level > 0)){
    //"turn on 1 LED"
  }
  // Fluid level less than or equal to 0% does not turn on any LED's.
  if (fluid_level <= 0){
    //"turn off all LEDs"
  }

}

// ---------- End fluidLevelSense ---------------------------------------------------- //

void verifyBoard(void){

  for(int i = 0; i<1; i++){
  
    ledSense.On();
    ledDirty.On();
    ledDirtyNozzle.On();
    ledClean.On();

    ledLevels.On();
    
//    ledLevel1.On();
///    ledLevel2.On();
///    ledLevel3.On();
//    ledLevel4.On();
    
    delay(500);
    
    ledSense.Off();
    ledDirty.Off();
    ledDirtyNozzle.Off();
    ledClean.Off();

    ledLevels.Off();
    
//    ledLevel1.Off();
//    ledLevel2.Off();
//    ledLevel3.Off();
//    ledLevel4.Off();
    
    delay(500);
  }

    // Level lights go up and down 3 times
    for(int i = 0; i<1; i++){
    for(int i = 0; i<100; i++){
      ledLevels.Percent(i);
      delay(5);
    }
    for(int i = 100; i>0; i--){
      ledLevels.Percent(i);
      delay(5);
    }
  }
  
}

int findIndex2(unsigned long a[], unsigned long value)
{
   // Modified from: https://phanderson.com/C/find_idx.html
   int i;

   // This FUNCTION DOES NOT WORK!! a[] is a pointer so size of a[] is 2... not size of array...
   // TODO fix!!! so much for coping others code...
   
   int num_elements = sizeof(a)/sizeof((a)[0]);
   Serial.print("Sizeof(a)");
   Serial.println(sizeof(a));
   Serial.println(sizeof((a)[0]));
   Serial.println(num_elements);
   for (i=0; i<num_elements; i++)
   {
    Serial.print("a[i]: ");
    Serial.print(a[i]);
    Serial.print(" == value: ");
    Serial.println(value);
   if (a[i] == value)
   {
      return(value);  /* it was found */
   }
   }
   return(-1);  /* if it was not found */
}


// ----------------------------------------------------------------- //
// ----- End Functions --------------------------------------------- //
// ----------------------------------------------------------------- //
