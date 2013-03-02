#include <Shieldbot.h>

#define COMMAND_KEY 33
#define CMD_RETRACT 65
#define CMD_EXTEND 66
#define CMD_REST 67

#define STATUS_LED_PIN 13

#define MOTOR_PWR 96


byte msg[2];

Shieldbot shieldbot = Shieldbot();

void setup()
{
	Serial.begin(9600);
        pinMode(13,OUTPUT);
        delay(500);
        shieldbot.setMaxSpeed(255);
}

void loop()
{

  if (Serial.available() > 1) {
    while(Serial.peek() != COMMAND_KEY){
      Serial.read();//Discard mis-aligned data.
    }
    int len = Serial.readBytes((char*)msg, 2);
    if (len == 2 && msg[0]==COMMAND_KEY) {
      switch(msg[1]){
        case CMD_RETRACT:
          Serial.println("RET");
          shieldbot.drive(-MOTOR_PWR,-MOTOR_PWR);
          delay(500);
          shieldbot.drive(0,0);
        break;
        case CMD_EXTEND:
          Serial.println("EXT");
          shieldbot.drive(MOTOR_PWR,MOTOR_PWR);
          delay(500);
          shieldbot.drive(0,0);
        break;
        case CMD_REST:
          Serial.print("REST");
          shieldbot.drive(0,0);
        break;
      }
    }
  }
  
  
}
