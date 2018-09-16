 #include "AccelStepper.h" 
 int plant_loc = {(10,10), (10,20), (10,30),
                  (20,10), (20,20), (20,30),
                  (30,10), (30,20), (30,30),
                  (40,10), (40,20), (40,30)
                  }
 

 bool Home=false,MOVING=false;
 long initial_homing=-1,TravelX=0;
  // Library created by Mike McCauley at http://www.airspayce.com/mikem/arduino/AccelStepper/
  
  // create array of stepper objects:
  // 1 = Easy Driver interface // NANO Pin 2 connected to STEP pin of Easy Driver  // NANO Pin 3 connected to DIR pin of Easy Driver
 AccelStepper stepper[3] = {
 AccelStepper(1,4,5), // (1=driver, step , direcion)
 AccelStepper(1,6,7),
 AccelStepper(1,8,9) 
};


                                  
                                    
  // Define the Pins used
#define homeSW 2 // Pin 2 connected to Home Switch (MicroSwitch)
#define limitSW 3

//=====================================================================================
//======================================== SETUP ======================================
//=====================================================================================

void setup() {
  // put your setup code here, to run once:
    pinMode(homeSW, INPUT_PULLUP);
    pinMode(limitSW, INPUT_PULLUP);
    delay(5);  // Wait for EasyDriver wake up
    
    Serial.begin(9600);
    
    HOMING();
    CONFIG();
     Serial.println("Enter Travel distance (Positive for CW / Negative for CCW and Zero for back to Home): ");
    
}

//=====================================================================================
//======================================= MAIN LOOP ===================================
//=======================================================================================

void loop() {
  
  //if(Serial.available()>0)  { // Check if values are available in the Serial Buffer
  while(Serial.available()==0) { }
    TravelX= Serial.parseInt();  // Put numeric value from buffer in TravelX variable
    if ((TravelX!=0)&&(!MOVING))
    {
      Serial.print("MOVING TO    ");
      Serial.println(TravelX);
      MOVE_TO("X",TravelX); //(axis number, target position)
      MOVE_TO("Y",TravelX); //(axis number, target position)
      MOVE_TO("Z",TravelX); //(axis number, target position)
      GRID(WAIT); // (
    }
}

//========================================================================================
//============================= HOMING ===================================================
//========================================================================================
void HOMING()
{
    
  
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
    stepper[i].setMaxSpeed(5000.0);      // Set Max Speed of Stepper (Faster for regular movements)
    stepper[i].setAcceleration(15000.0);  // Set Acceleration of Stepper
    
    
    Serial.println("HOMING ");
    if (i==0){Serial.print(" X ");} if (i==1){Serial.print(" Y ");} if (i==2){Serial.print(" Z ");} 
    Serial.println(" DONE ");

    Home=true;
    MOVING=false;

  }
  Serial.println (" HOMING X Y  Z DONE");
}





// ======================================================================================
//=================================  MOVE_TO  ===========================================
//=======================================================================================

void MOVE_TO(String c,long xPos)
{
  int axis;

  if (c=="X"){axis=0;}
  if (c=="Y"){axis=1;}
  if (c=="Z"){axis=2;}
  
  stepper[axis].moveTo(xPos);
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
    Serial.print(stepper[0].currentPosition ());
    Serial.print("  Y axis at   ");
    Serial.print(stepper[1].currentPosition ());  
    Serial.print("  Z axis at   ");
    Serial.println(stepper[2].currentPosition ());  
  
  
  
}

//==================================================================================================
//========================================= CONFIG =================================================
//=================================================================================================

void CONFIG()
  {
    stepper[0].setMaxSpeed(5000.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
    stepper[0].setAcceleration(5000.0);  // Set Acceleration of Stepper
    
    stepper[1].setMaxSpeed(5000.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
    stepper[1].setAcceleration(5000.0);  // Set Acceleration of Stepper

    stepper[2].setMaxSpeed(5000.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
    stepper[2].setAcceleration(5000.0);  // Set Acceleration of Stepper
   
  }

//=====================================================================================
//======================================== GRID ======================================
//=====================================================================================

void GRID(String ACTION)
  {
    serial.println(plant_loc);
    for (i = 0; i < length(plant_loc); i++) {
      
    }
    
  
}

//=====================================================================================
//======================================== WAIT ======================================
//=====================================================================================

void WAIT()
  {
    int wait_time = 2; // time to wait in grid in sec
    delay(wait_time);  
}

