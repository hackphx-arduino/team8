#include <AccelStepper.h>
#include <AFMotor.h>

#define COMMAND_KEY 33
#define CMD_DRIVE_X_FWD 65
#define CMD_DRIVE_X_BACK 66
#define CMD_DRIVE_Y_FWD 67
#define CMD_DRIVE_Y_BACK 68
#define CMD_DRIVE_STEPMODE 69
#define CMD_DRIVE_X_SPS 70
#define CMD_DRIVE_Y_SPS 71

#define DATA_KEY 34
#define DATA_DISTANCE 48

#define STEPMODE_SINGLE 49
#define STEPMODE_DOUBLE 50
#define STEPMODE_INTERLEAVE 51
#define STEPMODE_MICROSTEP 52

#define STATUS_KEY 43
#define STATUS_DISABLED 63
#define STATUS_READY 64
#define STATUS_X_READY 60
#define STATUS_Y_READY 62

#define STATUS_LED_PIN 13

#define STEP_PER_ROTATION_X 48
#define STEP_PER_ROTATION_Y 48

//for bound limiters if we get to them
#define X_PULSE 8
#define X_DIR 9
#define Y_PULSE 10
#define Y_DIR 11

//AF_Stepper xmotor(48, 1);
//AF_Stepper ymotor(48, 2);


byte msg[4];

long length;

unsigned long nextReadyX;
unsigned long nextReadyY;

byte currentStatus = STATUS_READY;

boolean xRunning = false;
boolean yRunning = false;

byte stepperMode = SINGLE;

void forwardstepx() {  
  //xmotor.onestep(FORWARD, stepperMode);
  digitalWrite(X_DIR, HIGH);
  digitalWrite(X_PULSE, HIGH);
  delayMicroseconds(10);
  digitalWrite(X_PULSE, LOW);
}
void backwardstepx() {  
  //xmotor.onestep(BACKWARD, stepperMode);
  digitalWrite(X_DIR, LOW);
  digitalWrite(X_PULSE, HIGH);
  delayMicroseconds(10);
  digitalWrite(X_PULSE, LOW);
}
// wrappers for the second motor!
void forwardstepy() {  
  //ymotor.onestep(FORWARD, stepperMode);
  digitalWrite(Y_DIR, HIGH);
  digitalWrite(Y_PULSE, HIGH);
  delayMicroseconds(10);
  digitalWrite(Y_PULSE, LOW);
}
void backwardstepy() {  
  //ymotor.onestep(BACKWARD, stepperMode);
  digitalWrite(Y_DIR, LOW);
  digitalWrite(Y_PULSE, HIGH);
  delayMicroseconds(10);
  digitalWrite(Y_PULSE, LOW);
}

AccelStepper xAxis(forwardstepx, backwardstepx);
AccelStepper yAxis(forwardstepy, backwardstepy);

void setup()
{
	Serial.begin(9600);
        pinMode(13,OUTPUT);
        pinMode(X_PULSE,OUTPUT);
        pinMode(X_DIR,OUTPUT);
        pinMode(Y_PULSE,OUTPUT);
        pinMode(Y_DIR,OUTPUT);
        delay(500);
        nextReadyX = millis()+1000;
        nextReadyY = millis()+1000;
        xAxis.setMaxSpeed(STEP_PER_ROTATION_X);
        yAxis.setMaxSpeed(STEP_PER_ROTATION_Y);
        xAxis.setAcceleration(STEP_PER_ROTATION_X);
        yAxis.setAcceleration(STEP_PER_ROTATION_Y);
        xAxis.setCurrentPosition(0);
        yAxis.setCurrentPosition(0);
}

void loop()
{

  if (Serial.available() > 3) {
    while(Serial.peek() != COMMAND_KEY){
      Serial.read();//Discard mis-aligned data.
    }
    int len = Serial.readBytes((char*)msg, 4);
    if (len == 4 && msg[0]==COMMAND_KEY) {
      switch(msg[1]){
        case CMD_DRIVE_X_FWD:
          Serial.print("X-FWD-");
          length = (((uint16_t) msg[2])<<8) | msg[3];
          Serial.println(length);
          xAxis.move(length);
          xRunning = true;
        break;
        case CMD_DRIVE_X_BACK:
          Serial.print("X-BACK-");
          length = (((uint16_t) msg[2])<<8) | msg[3];
          Serial.println(length);
          xAxis.move(-length);
          xRunning = true;
        break;
        case CMD_DRIVE_Y_FWD:
          Serial.print("Y-FWD-");
          length = (((uint16_t) msg[2])<<8) | msg[3];
          Serial.println(length);
          yAxis.move(length);
          yRunning = true;
        break;
        case CMD_DRIVE_Y_BACK:
          Serial.print("Y-BACK-");
          length = (((uint16_t) msg[2])<<8) | msg[3];
          Serial.println(length);
          xAxis.move(-1*length);
          yRunning = true;
        break;
        case CMD_DRIVE_X_SPS:
          Serial.print("X-SPS-");
          Serial.println(msg[2]);
          xAxis.setMaxSpeed(msg[2]);
        break;
        case CMD_DRIVE_Y_SPS:
          Serial.print("Y-SPS-");
          Serial.println(msg[2]);
          yAxis.setMaxSpeed(msg[2]);
        break;
        case CMD_DRIVE_STEPMODE:
          switch(msg[2]){
            case STEPMODE_SINGLE:
              stepperMode = SINGLE;
              break;
            case STEPMODE_DOUBLE:
              stepperMode = DOUBLE;
              break;
            case STEPMODE_INTERLEAVE:
              stepperMode = INTERLEAVE;
              break;
            case STEPMODE_MICROSTEP:
              stepperMode = MICROSTEP;
              break;
          }
          Serial.write(currentStatus);
        break;
      }
    }
  }
  if(xRunning){
    xAxis.run();
    if(xAxis.distanceToGo() == 0){
      Serial.write(STATUS_X_READY);
      xRunning = false;
    }
  }else{
    if(nextReadyX < millis()){
      Serial.write(STATUS_X_READY);
      nextReadyX = millis() + 5000;
    }
  }
  if(yRunning){
    yAxis.run();
    if(yAxis.distanceToGo() == 0){
      Serial.write(STATUS_Y_READY);
      yRunning = false;
    }
  }else{
    if(nextReadyY < millis()){
      Serial.write(STATUS_Y_READY);
      nextReadyY = millis() + 5000;
    }
  }
}
