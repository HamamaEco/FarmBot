#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Geometry.h>

Point p;
int Xborder = 20;
int MXborder = -20;
int Yborder = 20;
int MYborder = -20;
int Zborder = 20;
int MZborder = -20;
int movetox = 0;
int movetoy = 0;
int movetoz = 0;

AccelStepper stepper[3] = {
 AccelStepper(1,4,5),
 AccelStepper(1,6,7),
 AccelStepper(1,8,9) 
};

void setup() {
  for(int i = 0; i < 3; i++) {
  stepper[i].setMaxSpeed(5000);
  stepper[i].setAcceleration(10000);
  }
  Serial.begin(115200);
  
  p.Y() = 0;
  p.X() = 0;
  p.Z() = 0;
}

void loop() {
 Serial.print("X:");
 Serial.print(p.X());
 Serial.print(" Y:");
 Serial.println(p.Y());
 Serial.print(" Z:");
 Serial.println(p.Z());
  delay(100);
  movetopos(20,19,19);
  return;
}

void movetopos(int x,int y,int z) {
  Serial.println("Moving Pos");

  if (x > Xborder) {
    Serial.println("X Breaks borders! Defaulting to 20...");
    x = Xborder;
    return;
  } else if (x < MXborder) {
    Serial.println("X Breaks borders! Defaulting to -20...");
    x = MXborder;
    return;
  }
  
  if (y > Yborder) {
    Serial.println("Y Breaks borders! Defaulting to 20...");
    y = Yborder;
    return;
  } else if (y < MYborder) {
    Serial.println("Y Breaks borders! Defaulting to -20...");
    y = MYborder;
    return;
  }

  if (z > Zborder) {
    Serial.println("Z Breaks borders! Defaulting to 20...");
    z = Zborder;
    return;
  } else if (z < MZborder) {
    Serial.println("Z Breaks borders! Defaulting to -20...");
    z = MZborder;
    return;
  }
  

  if(p.X() < x) {
    p.X()++;
    stepper[1].move(200);
  } else if (p.X() > x) {
    p.X()--;
    stepper[1].move(-200);
  }

  if(p.Y() < y) {
    p.Y()++;
    stepper[2].move(20);
  } else if (p.Y() > y) {
    p.Y()--;
    stepper[2].move(-20);
  }

    if(p.Z() < z) {
    p.Z()++;
    stepper[3].move(20);
  } else if (p.Z() > z) {
    p.Z()--;
    stepper[3].move(-20);
  }
}
