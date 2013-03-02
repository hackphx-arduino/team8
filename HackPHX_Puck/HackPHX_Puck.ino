#include <AccelStepper.h>
#include <AFMotor.h>

#define COMMAND_KEY 33
#define CMD_RETRACT 65
#define CMD_EXTEND 66
#define CMD_REST 67

#define STATUS_LED_PIN 13


byte msg[2];

void setup()
{
	Serial.begin(9600);
        pinMode(13,OUTPUT);
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
