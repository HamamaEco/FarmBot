//==================================================================================================== Libraries ====================================================
 #include "AccelStepper.h" 
 // Library created by Mike McCauley at http://www.airspayce.com/mikem/arduino/AccelStepper/
 #include "Plant.h"
 // Library created by Hamama Students at http://www.greenhouse.org.il/
 


 //================================================================================================== Objects =======================================================
 // create array of plant objects:
 // plant( X position, Y position Z position)
 Plant plant[20] = {
 Plant(100,0,1000)/*plant 0*/,Plant(100,1000,1000)/*plant 1*/,Plant(100,2000,1000)/*plant 2*/,Plant(100,4000,1000)/*plant 3*/,Plant(100,6000,1000)/*plant 4*/,
 Plant(1100,0,1000)/*plant 5*/,Plant(1100,1000,1000)/*plant 6*/,Plant(1100,2000,1000)/*plant 7*/,Plant(1100,4000,1000)/*plant 8*/,Plant(1100,6000,1000)/*plant 9*/,
 Plant(100,0,1000)/*plant 10*/,Plant(2100,0,1000)/*plant 11*/,Plant(2100,2000,1000)/*plant 12*/,Plant(2100,4000,1000)/*plant 13*/,Plant(2100,6000,1000)/*plant 14*/,
 Plant(100,0,1000)/*plant 15*/,Plant(3100,0,1000)/*plant 16*/,Plant(3100,2000,1000)/*plant 17*/,Plant(3100,4000,1000)/*plant 18*/,Plant(3100,6000,1000)/*plant 19*/
}; 

  
  // create array of stepper objects:
  // 1 = Easy Driver interface // STEP pin of Easy Driver  //DIR pin of Easy Driver
 AccelStepper stepper[3] = {
 AccelStepper(1,4,5),
 AccelStepper(1,6,7),
 AccelStepper(1,8,9) 
};

//======================================================================== variables ==================================================================================
 // Define the Pins used
#define homeSW 2 // Pin 2 connected to Home Switch 
#define limitSW 3 // Pin 3 connected to Limit SW
#define waterOn 12 //pin 12 connected to water coil
#define humidityPin 0;
#define waterPin 21;
 bool Home=false,MOVING=false;
 long initial_homing=-1,TravelX=0;
 int dir=-1;
//=====================================================================================
//======================================== SETUP ======================================
//=====================================================================================

void setup() {
  
    pinMode(homeSW, INPUT_PULLUP); //set home pi sa input
    pinMode(limitSW, INPUT_PULLUP); // sel limit pin as input
    pinMode(waterOn, INPUT_PULLUP); // sel limit pin as input
    pinMode(humidityPin, INPUT);
    pinMode(waterPin, OUTPUT);
    delay(5);  // Wait for EasyDriver wake up
    
    Serial.begin(9600); // start serial communication port
    
    HOMING();
    
    Serial.println("Enter Travel distance (Positive for CW / Negative for CCW and Zero for back to Home): ");
    
}

//=======================================================================================
//======================================= MAIN LOOP =====================================
//=======================================================================================

void loop() {
  
  //if(Serial.available()>0)  { // Check if values are available in the Serial Buffer
  while(Serial.available()==0) { }
    TravelX= Serial.parseInt();  // Put numeric value from buffer in TravelX variable
    if ((TravelX>0)&&(!MOVING))
    {
      Serial.print("MOVING TO    ");
      Serial.println(TravelX);
      MOVE_TO("X",TravelX); //(axis number, target position)
      //MOVE_TO("Y",TravelX); //(axis number, target position)
      //MOVE_TO("Z",TravelX); //(axis number, target position)
    }
    if (TravelX== -1)
    {
      MAIN_PLANT_LOOP();
    }
}

//========================================================================================
//============================= HOMING ===================================================
//========================================================================================
void HOMING()
{
    
  Serial.println("HOMING...");
  for (int i=0;i<3;i++) //loop through all axis
  {
    stepper[i].setMaxSpeed(500.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
    stepper[i].setAcceleration(5000.0);  // Set Acceleration of Stepper
    stepper[i].moveTo(-1000000);  // Set the position to move to
    
    
    MOVING=true;

    while ((digitalRead(homeSW))&&(digitalRead(limitSW))) 
    {
      stepper[i].run();  // Start moving the stepper
    }
    if (!(digitalRead(limitSW)))
    {
      MOVING=false;
      Serial.println("LIMIT SWITCH PRESSED -  RESTART FOR HOMING ");
      while (true)
      {
        
      }
    }
    stepper[i].setCurrentPosition(0);  // Set the current position as zero for now
    MOVING =true;
    while ((!digitalRead(homeSW)))  
    { // Make the Stepper move CW until the switch is deactivated
      stepper[i].moveTo(initial_homing);  
      initial_homing++;
      delay(5);
      stepper[i].run();
    }
    
    stepper[i].setCurrentPosition(0);
    Serial.println("HOMING ");
    if (i==0){Serial.print(" X ");} if (i==1){Serial.print(" Y ");} if (i==2){Serial.print(" Z ");} 
    Serial.println(" DONE ");

    Home=true;
    MOVING=false;

  }
  Serial.println (" HOMING X Y Z DONE");
  CONFIG();
}

//=======================================================================================
//=================================  MOVE_TO  ===========================================
//=======================================================================================

void MOVE_TO(String c,long pos)
{
  int axis;
  pos=pos*dir;
  if (c=="X"){axis=0;}
  if (c=="Y"){axis=1;}
  if (c=="Z"){axis=2;}
  
  stepper[axis].moveTo(pos);
  MOVING=true;
  while ((digitalRead(homeSW))&&(digitalRead(limitSW))&&(stepper[axis].distanceToGo ()!=0))
  {
    stepper[axis].run();
  }
  if (!(digitalRead(limitSW)))
    {
      Serial.println("LIMIT SWITCH PRESSED -  RESTART FOR HOMING ");
      while (true)
      {
        
      }
    }
    if (!(digitalRead(homeSW)))
    {
      Serial.println("Home SWITCH PRESSED -  RESTART FOR HOMING ");
      while (true)
      {
        
      }
    }
    MOVING=false;
    Serial.print("X axis at   ");
    Serial.print(stepper[0].currentPosition ()*dir);
    Serial.print("  Y axis at   ");
    Serial.print(stepper[1].currentPosition ()*dir);  
    Serial.print("  Z axis at   ");
    Serial.println(stepper[2].currentPosition ()*dir);   
}

//==================================================================================================
//===================================== Main Plant Loop ============================================
//==================================================================================================

void MAIN_PLANT_LOOP()
{
  HOMING();
  Serial.println("Starting Main Plant Loop");
  for (int i=0;i<20;i++) //loop through all axis
  {
    if (plant[i].existing)
    {
    MOVE_TO("X",plant[i].X);//(axis number, target position)
    delay(100);
    MOVE_TO("Y",plant[i].Y);//(axis number, target position)
    delay(100);
    MOVE_TO("Z",plant[i].Z);//(axis number, target position)
    Serial.print("watering for ");
    Serial.print(plant[i].waterT/1000);
    Serial.println(" seconds");
    delay(plant[i].waterT);
    // WATERING();
    // MEASURING();
    MOVE_TO("Z",0);//(axis number, target position) 
    }
    else
    {
      Serial.println("no plant in database moving to next plant");
    }
  }
  MOVE_TO("X",0);//(axis number, target position)
  MOVE_TO("Y",0);//(axis number, target position)
  Serial.println("Finished Main Plant Loop");
}

//==================================================================================================
//========================================= PLANT FUNCTIONS ========================================
//==================================================================================================

void WATER_PLANT(long height) {
  MOVE_TO("Z",  height);
  delay(2);
  Serial.println("Watering..");
  int humidity = analogRead(humidityPin);
  plant.humidity = humidity;
  int mintime = 5;
  int maxtime = 20;
  int mappedmist = map(humidity, 0, 1023, mintime, maxtime);

  
  
  digitalWrite(waterPin, HIGH);
   Serial.println("Watering in progress..");
  delay(wateringtime);
  digitalWrite(waterPin, LOW);
   Serial.println("Done watering");
   MOVE_TO("Z", -height);
}


//==================================================================================================
//========================================= CONFIG =================================================
//==================================================================================================

void CONFIG()
  {
    stepper[0].setMaxSpeed(5000.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
    stepper[0].setAcceleration(5000.0);  // Set Acceleration of Stepper
    
    stepper[1].setMaxSpeed(5000.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
    stepper[1].setAcceleration(5000.0);  // Set Acceleration of Stepper

    stepper[2].setMaxSpeed(5000.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
    stepper[2].setAcceleration(5000.0);  // Set Acceleration of Stepper
   
  }

