#include <MPU6050_tockn.h>
#include <Wire.h>
#include "OneButton.h"
MPU6050 mpu6050(Wire);

long timer = 0;
int Xac; //Angle Variable X Axis
int Yac; // Angle Variable Y Axis
int target; // Target Angle, Maximum allowed to trigger alarm
int Fail = 0; // int to trigger alarm
int triggerCount; // trigger count
int lifeLED1 =4; //Digital pin 4
int lifeLED2 =5; //Digital pin 5
int lifeLED3 =6; //Digital pin 6
int failBuzzer =8; //Digital pin 8 (PWM)
int balanceLED1 = 7; //Digital pin 7
int balanceLED2 =9; //Digital pin 9
int balanceLED3 =10; //Digital pin 10
int balanceLED4 =11; //Digital pin 11
int balanceLED5 =12;//Digital pin 12
int balanceVal; //Used to calculate LED balance/angle status
int toggleState; //used to flash LEDS awaiting reset
int Xoff; //Determines the X axis offset on startup
int Yoff; //Determines the Y axis offset on startup
int restart=0;
OneButton resetButton(3, true);

void setup() {

  Serial.begin(9600);
  Wire.begin();
  tone(failBuzzer,1400,300);
   delay(300);
  mpu6050.begin(); //Begin Gyro Module
  mpu6050.calcGyroOffsets(true);
 
  ////////////////////Adjustable   Variables ///////////////////////

  target=8; //Target Angle, in degrees

  /////////////////////////////////////////////////////////////////
  
  triggerCount = 0; //Reset trigger count on startp
   pinMode(LED_BUILTIN, OUTPUT);
   pinMode(lifeLED1, OUTPUT);
   pinMode(lifeLED2, OUTPUT);
   pinMode(lifeLED3, OUTPUT);
   pinMode(failBuzzer,OUTPUT);
   pinMode(balanceLED1, OUTPUT);
   pinMode(balanceLED2, OUTPUT);
   pinMode(balanceLED3, OUTPUT);
   pinMode(balanceLED4, OUTPUT);
   pinMode(balanceLED5, OUTPUT);
   mpu6050.update(); // Update gyro module
//Read X and Y angle for offset calculation
  Xoff=(mpu6050.getAccAngleX()); 
  Yoff=(mpu6050.getAccAngleY());

   resetButton.attachClick(mute);
   resetButton.attachLongPressStop(offsetCal); 
   resetButton.attachDoubleClick (fullLife);
   tone(failBuzzer,200,200);
   delay(220);
   tone(failBuzzer,1200,200);
   delay(220);
   tone(failBuzzer,1400,200);
   delay(220);
   
}

void spaceGun(int maximum){         //Generates increasing zap noise
  for(int i = 0; i <maximum; i++){
  digitalWrite(failBuzzer,HIGH);
  delayMicroseconds(i);
  digitalWrite(failBuzzer,LOW);
  delayMicroseconds(i);
  } 
   noTone(failBuzzer);
}

void revGun(int maximum) {        //Generates decreasing zap noise
  for(int i = maximum; i>0; i--){
  digitalWrite(failBuzzer,HIGH);
  delayMicroseconds(i);
  digitalWrite(failBuzzer,LOW);
  delayMicroseconds(i);
  }
  noTone(failBuzzer);
}
 void offsetCal(){
      tone(failBuzzer,800,100);
   delay(120);
  digitalWrite(lifeLED1,HIGH);
      digitalWrite(lifeLED2,HIGH);
      digitalWrite(lifeLED3,HIGH);
      digitalWrite(balanceLED1,HIGH);
      digitalWrite(balanceLED2,HIGH);
      digitalWrite(balanceLED3,HIGH);
      digitalWrite(balanceLED4,HIGH);
      digitalWrite(balanceLED5,HIGH);
  mpu6050.update(); // Update gyro module
  Xoff=(mpu6050.getAccAngleX()); 
  Yoff=(mpu6050.getAccAngleY());
   tone(failBuzzer,200,200);
   delay(220);
   tone(failBuzzer,1200,200);
   delay(220);
   tone(failBuzzer,1400,200);
   delay(220);
   tone(failBuzzer,1400,200);
   delay(220);
   tone(failBuzzer,1200,200);
   delay(220);
   tone(failBuzzer,200,200);
   delay(220);
 }
 void mute(){
 restart=1;
 }

 void fullLife(){
   triggerCount=0;
   revGun(1200);
   tone (failBuzzer, 1200, 400);
   delay(420);
     
 }
 
void loop() {

 resetButton.tick();
 delay(10);

if (triggerCount==3){ //After 3 fails trigger alarm
        
      toggleState =! toggleState;
      digitalWrite(lifeLED1,toggleState);
      digitalWrite(lifeLED2,toggleState);
      digitalWrite(lifeLED3,toggleState);
      digitalWrite(balanceLED1,toggleState);
      digitalWrite(balanceLED2,toggleState);
      digitalWrite(balanceLED3,toggleState);
      digitalWrite(balanceLED4,toggleState);
      digitalWrite(balanceLED5,toggleState);
      if(toggleState==1){
      spaceGun(800);}
      else{
      revGun(800);}
      
      noTone(failBuzzer);
      
      if (restart==1){
        triggerCount=0;
      }
      }
      else
      {
 mpu6050.update(); // Update gyro module
Xac = (mpu6050.getAccAngleX())-Xoff; //Xac is X axis angle -offset
Yac= (mpu6050.getAccAngleY())-Yoff; //Yac is Y axis angle -offset

if (Xac<0){ // If X angle is negative, multiply by -1 to make it positive so it matches target
  Xac = Xac * -1;
} else{}  //Else nothing

if (Yac<0){ // If Y angle is negative, multiply by -1 to make it positive so it matches target
  Yac = Yac * -1;
} else{}  //Else nothing

Serial.println ("Angles - Post Calculation");
Serial.print("Y Axis ");
Serial.print(Yac);
Serial.print("\tX Axis ");
Serial.println(Xac);
Serial.println("////////////////////////////////");

if (Yac>Xac){
balanceVal= map(Yac,0,target,0,5);
} else{
  balanceVal= map(Xac,0,target,0,5);
}

switch(balanceVal)
  {

    case 0:
    
      digitalWrite(balanceLED1,LOW);
      digitalWrite(balanceLED2,LOW);
      digitalWrite(balanceLED3,LOW);
      digitalWrite(balanceLED4,LOW);
      digitalWrite(balanceLED5,LOW);
      break;
      
    case 1:
    
      digitalWrite(balanceLED1,HIGH);
      digitalWrite(balanceLED2,LOW);
      digitalWrite(balanceLED3,LOW);
      digitalWrite(balanceLED4,LOW);
      digitalWrite(balanceLED5,LOW);
      break;
      
    case 2:
    
     digitalWrite(balanceLED1,HIGH);
     digitalWrite(balanceLED2,HIGH);
     digitalWrite(balanceLED3,LOW);
     digitalWrite(balanceLED4,LOW);
     digitalWrite(balanceLED5,LOW);
     break;
      
    case 3:
    
     digitalWrite(balanceLED1,HIGH);
     digitalWrite(balanceLED2,HIGH);
     digitalWrite(balanceLED3,HIGH);
     digitalWrite(balanceLED4,LOW);
     digitalWrite(balanceLED5,LOW);
     break;

    case 4:
    
     digitalWrite(balanceLED1,HIGH);
     digitalWrite(balanceLED2,HIGH);
     digitalWrite(balanceLED3,HIGH);
     digitalWrite(balanceLED4,HIGH);
     digitalWrite(balanceLED5,LOW);
     break;

    case 5:
      digitalWrite(balanceLED1,HIGH);
      digitalWrite(balanceLED2,HIGH);
      digitalWrite(balanceLED3,HIGH);
      digitalWrite(balanceLED4,HIGH);
      digitalWrite(balanceLED5,HIGH);
      break;
      
  }
  

if (Xac>target){ //If trget angle is exceeded then trigger fail int and serial print data

    Fail=1;
    delay(1);
    Serial.println(Xac);
  
} else {}

if (Yac>target){ // If target angle is exceeded then trigger fail int and serial print data
 
    Fail=1;
    delay(1);
    Serial.println(Yac);
    
} else {} //Else nothing

if (Fail > 0) { //  If fail is 1 then increase trigger count

  triggerCount++;                  // add one to trigger count

  for (int i = 0; i < 3 ; i++){
   spaceGun(300);
   delay(300);
   }
   
   
  }else{}// Else nothing
  
  switch(triggerCount)
  {
    case 0: //full lives
    digitalWrite(lifeLED1,HIGH);
    digitalWrite(lifeLED2,HIGH);
    digitalWrite(lifeLED3,HIGH);
     break;

    case 1: // one fail
    digitalWrite(lifeLED1,HIGH);
    digitalWrite(lifeLED2,HIGH);
    digitalWrite(lifeLED3,LOW);
     break;

    case 2: //2 fails
    digitalWrite(lifeLED1,HIGH);
    digitalWrite(lifeLED2,LOW);
    digitalWrite(lifeLED3,LOW);    
    break;

    case 3: //3 fails - no lives left 
    digitalWrite(lifeLED1,LOW);
    digitalWrite(lifeLED2,LOW);
    digitalWrite(lifeLED3,LOW);
    break;
  }
  
 if (Fail > 0 && triggerCount<3) { //  If fail was triggered in this loop AND the game is not over, play reset tune

  
   revGun (800);
   tone (failBuzzer,200, 250);
   delay(250);
   noTone(failBuzzer);
    
  }else{} //else do nothing
  
  Fail=0;                          //Reset Fail to 0 for next cycle
 
      }
        restart=0;
}
