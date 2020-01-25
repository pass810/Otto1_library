#include<OttoESP32.h>
#include<US100.h>

OttoESP32 Otto;
US100 US;

//---- Init HCSR04
#define trig 5
#define echo 18

int zero_position[6] = { -5, 5, -6, 0, 0, 0}; //correct servos
bool obstacleDetected = false;

// constants won't change. They're used here to set pin numbers:
const int sensorPin = 4;     // the number of the pushbutton pin
//-- Movement parameters
int MoveNum = 1;

void setup()
{
  //Initialization
  //Serial.begin(115200);
  Otto.init(zero_position);
  Otto.oled_print("Touch sensor");
  Otto.home();
  Otto.sing(S_happy); // a happy Otto :)

  //touch sensor init
  pinMode(sensorPin, INPUT);

  //double Core task
  xTaskCreatePinnedToCore(obstacleDetector, "Task0", 4096, NULL, 1, NULL, 0);
}

void loop()
{
  int sensorState = digitalRead(sensorPin);
  if (obstacleDetected) {
    Otto.sing(S_fart1);
    Otto.walk(2, 500, -1);
    Otto.tiptoeSwing(2, 500, 40);
    Otto.home();
    Otto.sing(S_fart3);
    delay(50);
    Otto.home();
    Otto.putMouth(xMouth);
  }
  if (sensorState == HIGH)
  {
    switch (MoveNum) {
      case 1:
        Otto.oled_print("1:walk");
        Otto.sing(S_surprise);
        Otto.walk(1, 1300, 1);
        Otto.home();
        break;
      case 2:
        Otto.oled_print("2:moonwalker");
        Otto.sing(S_OhOoh);
        Otto.moonwalker(2, 1000, 30, 1);
        Otto.home();
        break;
      case 3:
        Otto.oled_print("3:crusaito");
        Otto.sing(S_happy);
        Otto.crusaito(2, 3000, 40, 1);
        Otto.home();
        break;
      case 4:
        Otto.oled_print("4:shakeLeg");
        Otto.sing(S_superHappy);
        Otto.shakeLeg(2, 1000, 1);
        Otto.home();
        break;
      case 5:
        Otto.oled_print("5:flapping");
        Otto.sing(S_fart1);
        Otto.flapping(2, 500, 40, 1);
        Otto.home();
        break;
      case 6:
        Otto.oled_print("6:tiptoeSwing");
        Otto.sing(S_confused);
        Otto.tiptoeSwing(2, 1000, 30);
        Otto.home();
        break;
      case 7:
        Otto.oled_print("7:swing");
        Otto.sing(S_connection);
        Otto.swing(2, 500, 40);
        Otto.home();
        break;
      case 8:
        Otto.oled_print("8:updown");
        Otto.sing(S_disconnection);
        Otto.updown(2, 1000, 30);
        Otto.home();
        break;
      case 9:
        Otto.oled_print("9:jitter");
        Otto.sing(S_buttonPushed);
        Otto.jitter(10, 500, 40);
        Otto.home();
        break;
      case 10:
        Otto.oled_print("10:ascendTurn");
        Otto.sing(S_fart3);
        Otto.ascendingTurn(2, 500, 50);
        Otto.home();
        break;
      case 11:
        Otto.oled_print("11:jump");
        Otto.sing(S_sad);
        Otto.jump(2, 500);
        Otto.home();
        break;
      case 12:
        Otto.oled_print("12:turnLeft");
        Otto.sing(S_cuddly);
        Otto.turn(3, 1000, 1);
        Otto.home();
        break;
      case 13:
        Otto.oled_print("13:bend");
        Otto.sing(S_superHappy);
        Otto.bend(2, 1000, 1);
        Otto.home();
        break;
    }
    Otto.putMouth(MoveNum);
    MoveNum++;
    if (MoveNum > 13)
    {
      MoveNum = 1;
    }
  }

}

//----------------------------------------------------- Solve distance
//////////////////////////////////////////////////////////////////////////////////
//-- Function to read distance sensor & to actualize obstacleDetected variable //
/////////////////////////////////////////////////////////////////////////////////
void obstacleDetector(void *arg) {
  //Setup US-100
  US.init(trig, echo);
  // Start Ranging
  while (1) {
    // Compute distance
    float distance = US.getDistance();
    //Serial.print(distance);
    //Serial.println("cm");
    if (distance < 15) {
      obstacleDetected = true;
      //Serial.println ("Obstacle Detected! Avoid collision");
    } else {
      obstacleDetected = false;
      //Serial.println ("No Obstacle detected! - Keep on walking");
    }
    delay(50);
  }
}
