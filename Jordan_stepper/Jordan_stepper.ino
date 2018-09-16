// ProportionalControl.pde
// -*- mode: C++ -*-
//
// Make a single stepper follow the analog value read from a pot or whatever
// The stepper will move at a constant speed to each newly set posiiton, 
// depending on the value of the pot.
//
// Copyright (C) 2012 Mike McCauley
// $Id: ProportionalControl.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>

// Define a stepper and the pins it will use
AccelStepper stepper(1,4,5); // Defaults to AccelStepper:(3 pins) on 1, 4, 5



void setup()
{  
  Serial.begin(9600);
  stepper.setMaxSpeed(5000);
  stepper.setAcceleration(10000);
  
   
  

}

void loop() {
  Serial.println(stepper.distanceToGo());
  while( stepper.distanceToGo()!=0){
  stepper.moveTo(1000);  
  stepper.run();  
      
  }
   stepper.setCurrentPosition(0);
  }

