// this skecth is tested on a Arduino UNO, Arduino PRO MINI will be used for Blackberry JAM
/*
  Hardware hookup:
  Arduino --------------- MMA8452Q Breakout
    3.3V  ---------------     3.3V
    GND   ---------------     GND
  SDA (A4) --\/330 Ohm\/--    SDA
  SCL (A5) --\/330 Ohm\/--    SCL
*/

#include "Wire.h"
#include "SparkFun_MMA8452Q.h"

//onboard LED
#define LED 13 

//Accelerometer
#define INT_1 2
#define INT_2 3

//Dual motor driver
#define STBY 4
#define Pumps_enable 5 //AI 1
#define Pumps 9 //PWMA
#define Motor_enable 6 //BI 1
#define Motor 10 //PWMB


MMA8452Q accel;
bool triggered = 0;
unsigned long previousTime;
unsigned long currentTime;
const float cutoff = 0.25; //squared

float x,y,z;

int status = 1; //variable created for Serial test



void setup(){

  Serial.begin(9600);
  while(!Serial);

  pinMode(STBY, OUTPUT);
  pinMode(Pumps_enable,OUTPUT);
  pinMode(Pumps,OUTPUT);
  pinMode(Motor_enable,OUTPUT);
  pinMode(Motor,OUTPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(STBY,LOW);
  digitalWrite(Pumps,LOW);
  digitalWrite(Pumps_enable,LOW);
  digitalWrite(Motor_enable,LOW);
  digitalWrite(Motor,LOW);

  Wire.begin();
  
  if (accel.begin()){
    /*
    for (int i = 0; i < 3; i++){
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      delay(200);
    } */    
  } else {/*
      while(1){
          digitalWrite(LED, HIGH);
      }*/
  }

    accel.active();

    if (accel.isActive()){
      
      /* flashing Arduino LED to indicate IMU status
      for (int i = 0; i < 5; i++){
        digitalWrite(LED, HIGH);
        delay(200);
        digitalWrite(LED, LOW);
        delay(500);
      } 
      */
     Serial.println("MMA8452Q is active");  
    }

  
}

void loop(){

  while(!is_1G()){ //while not 1G (micro g)
    digitalWrite(STBY, HIGH); //STBY (H)
    digitalWrite(Motor_enable, HIGH); //PWMA (H)
    digitalWrite(Motor,HIGH); //turn on motor -> AIN1 (H)
    triggered = 1;
  }
  digitalWrite(Motor_enable,LOW); //turn off motor -> AIN1 (L)

  if(is_1G() && triggered ){
    currentTime = millis();
    previousTime = currentTime;
    
    while(((currentTime - previousTime)<= 2000) && is_1G()){
      Serial.println(currentTime - previousTime);
      digitalWrite(Pumps_enable, HIGH); //PWMB (H)
      digitalWrite(Pumps, HIGH); //turn on motor -> BIN1 (H)
      currentTime = millis();
    }
    digitalWrite(Pumps_enable, LOW); //turn off motor -> BIN1 (L)
    triggered = 0;
  }
   
}

bool is_1G(){
  /*
  x = accel.getCalculatedX();
  y = accel.getCalculatedY();
  z = accel.getCalculatedZ();

  if((x*x+y*y+z*z) >= 1.0){
    return 1;
  }else {
    return 0;
  }
  */
  if(Serial.available() > 0){
    status = Serial.read() - '0'; 
    Serial.println(status);
  }

  if (status == 1){
    return 1;
    
  } else {
    status = 0;
    return 0;
  }
}

/*
bool is_microG(){
  x = accel.getCalculatedX();
  y = accel.getCalculatedY();
  z = accel.getCalculatedZ();

  if((x*x+y*y+z*z) <= cutoff){
    triggered = 1;
    return 1;
  }else{
    return 0;
    }
}
*/




 
