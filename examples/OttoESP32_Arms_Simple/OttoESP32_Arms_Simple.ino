#include<OttoESP32.h>

OttoESP32 Otto;

int zero_position[6] = { -5, 5, -6, 0, 0, 0}; //correct servos

void setup()
{
  //Initialization
  //Serial.begin(115200);
  Otto.init(zero_position);
  Otto.oled_print("Simple Test");
  Otto.home();
  delay(1000);
}

void loop()
{
  //Move
  Otto.oled_print("Jump");
  Otto.home();
  delay(500);
  Otto.putMouth(happyOpen);
  Otto.sing(S_happy);
  Otto.jump(3, 500);
  Otto.home();

  Otto.oled_print("Walk Forward");
  delay(500);
  Otto.putMouth(smile);
  Otto.sing(S_cuddly);
  Otto.walk(4, 1000, 1);
  Otto.home();
  /*
  Otto.oled_print("Walk Backward");
  delay(500);
  Otto.putMouth(happyOpen);
  Otto.sing(S_happy);
  Otto.walk(4, 1000, -1);
  Otto.home();

  Otto.oled_print("turn left");
  delay(500);
  Otto.putMouth(bigSurprise);
  Otto.sing(S_OhOoh);
  Otto.turn(3, 1000,  1);
  Otto.home();

  Otto.oled_print("turn right");
  delay(500);
  Otto.putMouth(smallSurprise);
  Otto.sing(S_OhOoh2);
  Otto.turn(3, 1000, -1);
  Otto.home();

  Otto.oled_print("bend left");
  delay(500);
  Otto.putMouth(lineMouth);
  Otto.sing(S_mode1);
  Otto.bend(1, 500, 1);
  Otto.home();

  Otto.oled_print("bend right");
  delay(500);
  Otto.putMouth(tongueOut);
  Otto.sing(S_mode2);
  Otto.bend(1, 500, -1);
  Otto.home();

  Otto.oled_print("shake Leg");
  delay(500);
  Otto.putMouth(vamp1);
  Otto.sing(S_mode3);
  Otto.shakeLeg(1, 2000, 1);
  Otto.home();

  Otto.oled_print("shake Leg");
  delay(500);
  Otto.putMouth(vamp2);
  Otto.sing(S_connection);
  Otto.shakeLeg(1, 2000, -1);
  Otto.home();

  Otto.oled_print("up and down");
  delay(500);
  Otto.putMouth(okMouth);
  Otto.sing(S_disconnection);
  Otto.updown(2, 1000, 30);
  Otto.home();

  Otto.oled_print("swing");
  delay(500);
  Otto.putMouth(three);
  Otto.sing(S_buttonPushed);
  Otto.swing(2, 2000, 30);
  Otto.home();

  Otto.oled_print("tiptoe swing");
  delay(500);
  Otto.putMouth(two);
  Otto.sing(S_confused);
  Otto.tiptoeSwing(2, 2000, 30);
  Otto.home();

  Otto.oled_print("jitter");
  delay(500);
  Otto.putMouth(zero);
  Otto.sing(S_buttonPushed);
  Otto.jitter(3, 1000, 20);
  Otto.home();

  Otto.oled_print("ascendingTurn");
  delay(500);
  Otto.putMouth(xMouth);
  Otto.sing(S_buttonPushed);
  Otto.ascendingTurn(4, 1000, 15);
  Otto.home();

  Otto.oled_print("moonWalker");
  delay(500);
  Otto.putMouth(happyClosed);
  Otto.sing(S_happy);
  Otto.moonwalker(4, 2000, 30, 1);
  Otto.home();

  Otto.oled_print("moonWalker");
  delay(500);
  Otto.putMouth(eight);
  Otto.sing(S_sad);
  Otto.moonwalker(4, 2000, 30, -1);
  Otto.home();

  Otto.oled_print("crusaito");
  delay(500);
  Otto.putMouth(heart);
  Otto.sing(S_happy);
  Otto.crusaito(4, 2000, 30, 1);
  Otto.home();

  Otto.oled_print("crusaito");
  delay(500);
  Otto.putMouth(heart);
  Otto.sing(S_happy_short);
  Otto.crusaito(4, 2000, 30, -1);
  Otto.home();

  Otto.oled_print("flapping");
  delay(500);
  Otto.putMouth(six);
  Otto.sing(S_OhOoh);
  Otto.flapping(4, 2000, 30, 1);
  Otto.home();

  Otto.oled_print("flapping");
  delay(500);
  Otto.putMouth(seven);
  Otto.sing(S_OhOoh);
  Otto.flapping(4, 2000, 30, -1);
  Otto.home();

  //Gesture
  Otto.oled_print("Happy");
  Otto.playGesture(OttoHappy);
  Otto.home();

  Otto.oled_print("Super Happy");
  Otto.playGesture(OttoSuperHappy);
  Otto.home();

  Otto.oled_print("Sad");
  Otto.playGesture(OttoSad);
  Otto.home();

  Otto.oled_print("Sleeping");
  Otto.playGesture(OttoSleeping);
  Otto.home();

  Otto.oled_print("Fart");
  Otto.playGesture(OttoFart);
  Otto.home();

  Otto.oled_print("Confused");
  Otto.playGesture(OttoConfused);
  Otto.home();

  Otto.oled_print("Love");
  Otto.playGesture(OttoLove);
  Otto.home();

  Otto.oled_print("Angry");
  Otto.playGesture(OttoAngry);
  Otto.home();

  Otto.oled_print("Fretful");
  Otto.playGesture(OttoFretful);
  Otto.home();

  Otto.oled_print("Magic");
  Otto.playGesture(OttoMagic);
  Otto.home();

  Otto.oled_print("Wave");
  Otto.playGesture(OttoWave);
  Otto.home();

  Otto.oled_print("Victory");
  Otto.playGesture(OttoVictory);
  Otto.home();

  Otto.oled_print("Fail");
  Otto.playGesture(OttoFail);
  Otto.home();
  */
}
