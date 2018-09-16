#include <AccelStepper.h>

#define homeSW 2
#define pinSW 3
#define actionWater 0

AccelStepper steppers[3] = {
  AccelStepper(1, 4, 5),
  AccelStepper(1, 6, 7),
  AccelStepper(1, 8, 9)
};

void setup() {
  Serial.begin(9600);
//  pinMode(homeSW, INPUT_PULLUP);
//  pinMode(pinSW, INPUT_PULLUP);
//  delay(5);
//
//  stepper.setMaxSpeed(5000);
//  stepper.setAcceleration(5000);
//  Serial.begin(9600);
//  Serial.println("Started!");
//
//  for(int i = 0; i < steppers.length; i++) {
//      doHoming(i);
//  }
}

void loop() {
  handleCommands();
}

void split(String s, char del, String arr[]) {
   int numWords = 0;
   for(int i = 0; i < s.length(); i++) {
    char c = s.charAt(i);
    if(c == del) {
      numWords += 1;  
    }
  }
  arr[numWords];
  String currentWord = "";
  int filledWords = 0;
  for(int i = 0; i < s.length(); i++) {
    
    char c = s.charAt(i);
    currentWord += c;
    if(c == del) {
      arr[filledWords] = currentWord;
      currentWord = "";
      filledWords += 1;
    }  
  }
}
void handleCommands() {
  if(!Serial) return;
  String input = Serial.readString();
  if(input == "") return;
  //String* words;
  //split(input, ' ', words);
  //Serial.println("Number of words: " + (String) strlen(words));

  input.toLowerCase();
  if(input.charAt(0) == 'x') {
    input = input.substring(1);
    moveToPos(input.toInt(), 0, 0);  
  }
  if(input.charAt(0) == 'y') {
    input = input.substring(1);;
    moveToPos(0, input.toInt(), 0);  
  }
  if(input.charAt(0) == 'z') {
      input = input.substring(1); 
    moveToPos(0, 0, input.toInt());
  }
}

/**
 * Accepts stepper index and moves it back to 0.
 * @param st - The stepper's index.
 */
void doHoming(int st) {
  
}

/**
 * Moves motor to x, y, z
 * @param x the x location
 * @param y the y location
 * @param z the z location
 */
void moveToPos(int x, int y, int z) {
  Serial.println("Moved to: " + (String) x + ", " + (String) y + ", " + (String) z);
}

/**
 * Does an action all the grid points
 */
void grid(int action) {
  Serial.println("Did action " + action); 
}
