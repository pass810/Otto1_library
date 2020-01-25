#include <OttoESP32.h>
#include <US100.h>

OttoESP32 Otto;
US100 US;

int zero_position[6] = { -5, 5, -6, 0, 0, 0}; //correct servos
bool obstacleDetected = false;

//---- Init HCSR04
#define trig 5
#define echo 18

void setup()
{
  //Initialization
  //Serial.begin(115200);
  Otto.init(zero_position);
  Otto.oled_print("Avoid Test");

  Otto.home();

  //double Core task
  xTaskCreatePinnedToCore(obstacleDetector, "Task0", 4096, NULL, 1, NULL, 0);
}

void loop()
{
  if (obstacleDetected) {
    Otto.sing(S_surprise);
    Otto.playGesture(OttoFretful);
    Otto.sing(S_fart3);
    Otto.walk(2, 1300, -1);
    Otto.turn(3, 1000, 1);
    delay(50);
    Otto.home();
  }
  else {
    Otto.walk(1, 1000, 1);
  }
}

//----------------------------------------------------- Solve distance
//////////////////////////////////////////////////////////////////////////////////
//-- Function to read distance sensor & to actualize obstacleDetected variable //
/////////////////////////////////////////////////////////////////////////////////
void obstacleDetector(void *arg) {
  //Setup US-100
  US.init(trig,echo);
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
