#include <Pololu3piPlus32U4.h>

using namespace Pololu3piPlus32U4;

Encoders encoders;
LCD lcd;
Buzzer buzzer;
Motors motors;
ButtonA buttonA;

unsigned long currentMillis;
unsigned long prevMillis;
const unsigned long PERIOD = 15;

long countsLeft = 0;
long countsRight = 0;
long prevLeft = 0;
long prevRight = 0;

const int CLICKS_PER_ROTATION = 12;
const float GEAR_RATIO = 75.81F;
const float WHEEL_DIAMETER = 3.2;
const float WHEEL_CIRCUMFERENCE = 10.0531F;

//distance traveled by left/right wheels
float Sl = 0.0F;
float Sr = 0.0F;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  delay(1000);
  // play a beep sound 
  buzzer.play("c32");
}

void loop() {
  // put your main code here, to run repeatedly:
  checkEncoders();
}

void checkEncoders(){
  currentMillis = millis();
  if(currentMillis > prevMillis + PERIOD){
    countsLeft += encoders.getCountsAndResetLeft();
    countsRight += encoders.getCountsAndResetRight();

    // Distance achieved in cm
    Sl += ((countsLeft - prevLeft) / (CLICKS_PER_ROTATION * GEAR_RATIO) * WHEEL_CIRCUMFERENCE);
    Sr += ((countsRight - prevRight) / (CLICKS_PER_ROTATION * GEAR_RATIO) * WHEEL_CIRCUMFERENCE);

    int wheelSpeed = 80;
    if(Sr < 30.3){
      // Add a slow down after 20cm
      if(Sr > 20){
        wheelSpeed = 75 * ((30 - Sr)/ 10);
        if(wheelSpeed < 25) wheelSpeed = 25;
      }
      motors.setSpeeds(wheelSpeed, wheelSpeed);
    }else{
      motors.setSpeeds(0,0);
    }
    
    Serial.print("Left: ");
    Serial.print(Sl);
    Serial.print(" Right: ");
    Serial.println(Sr);

    prevLeft = countsLeft;
    prevRight = countsRight;
    prevMillis = currentMillis;
  }
}
