#include <OttoESP32.h>
#include <US100.h>
#include "BluetoothSerial.h"
//This Otto1 works with bluetooth on android and iOS?
//Android application is https://play.google.com/store/apps/details?id=com.ottodiy&hl=en

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

OttoESP32 Otto;
BluetoothSerial SerialBT;

int ii;
char buf[20],buf2[20];

void setup()
{
  int zero_position[6] = { -5, 5, -7, 0, 0, 0}; //correct servos
  //Initialization
  //Serial
  Serial.begin(115200);
  //bluetooth
  SerialBT.begin("OttoESP32"); //Bluetooth device name
  //Otto
  Otto.init(zero_position);
  Otto.oled_print("Bluetooth");
  Otto.home();
}

void loop()
{
  char c;
  char *cmd;
  char *dat;
  int dd[2], i;
  if (SerialBT.available()) {
    c = (char)SerialBT.read();
    //Serial.print(c);
    if (c == '\r') {
      buf[ii] = '\0';
      //Serial.println(" ");
      Serial.println(buf);
      cmd = strtok(buf, " ");
      switch (cmd[1]) {
        case 'M':
          for (i = 0; i < 2; i++) {
            dat = strtok(NULL, " ");
            if (dat != "" and dat != NULL) {
              dd[i] = atoi(dat);
            }
          }
          sprintf(buf2, "%s,%d,%d", cmd, dd[0], dd[1]);
          move(dd[0], dd[1]);
          Serial.println(buf2);
          break;
        case 'H': case 'K':
          dat = strtok(NULL, " ");
          i = 0;
          if (dat != "" and dat != NULL) {
            dd[i] = atoi(dat);
          }
          sprintf(buf2, "%s,%d", cmd, dd[0]);
          Serial.println(buf2);
          if (cmd[1] == 'H') GesturePlay(dd[0]);
          if (cmd[1] == 'K') SingSong(dd[0]);
          break;
      }
      ii = 0;
    }
    buf[ii] = c;
    ii++;
    delay(20);
  }
}

void move(int moveId, int T) {
  int moveSize = 30;

  switch (moveId) {
    case 0:
      Otto.home();
      break;
    case 1: //M 1 1000
      Otto.putMouth(happyOpen);
      Otto.walk(3, T, 1);
      break;
    case 2: //M 2 1000
      Otto.walk(3, T, -1);
      break;
    case 3: //M 3 1000
      Otto.turn(3, T, 1);
      break;
    case 4: //M 4 1000
      Otto.turn(3, T, -1);
      break;
    case 5: //M 5 1000 30
      Otto.updown(2, 1000, moveSize);
      break;
    case 6: //M 6 1000 30
      Otto.moonwalker(3, 1000, moveSize, 1);
      break;
    case 7: //M 7 1000 30
      Otto.moonwalker(3, 1000, moveSize, -1);
      break;
    case 8: //M 8 1000 30
      Otto.swing(2, 1000, moveSize);
      break;
    case 9: //M 9 1000 30
      Otto.crusaito(3, 1000, moveSize, 1);
      break;
    case 10: //M 10 1000 30
      Otto.crusaito(3, 1000, moveSize, -1);
      break;
    case 11: //M 11 1000
      Otto.jump(2, T);
      break;
    case 12: //M 12 1000 30
      Otto.flapping(3, 1000, moveSize, 1);
      break;
    case 13: //M 13 1000 30
      Otto.flapping(3, 1000, moveSize, -1);
      break;
    case 14: //M 14 1000 20
      Otto.tiptoeSwing(2, T, moveSize);
      break;
    case 15: //M 15 500
      Otto.bend(1, 500, 1);
      break;
    case 16: //M 16 500
      Otto.bend(1, 500, -1);
      break;
    case 17: //M 17 500
      Otto.shakeLeg(1, 2000, 1);
      break;
    case 18: //M 18 500
      Otto.shakeLeg(1, 2000, -1);
      break;
    case 19: //M 19 500 20
      moveSize = 20;
      Otto.jitter(1, T, moveSize);
      break;
    case 20: //M 20 500 15
      moveSize = 15;
      Otto.ascendingTurn(1, T, moveSize);
    case 21: //M 21
      Otto.handsup();
      break;
    case 22: //M 22 right arm
      Otto.handwave(-1);
      break;
    case 23: //M 23 left arm
      Otto.handwave(1);
      break;
  }
  Otto.home();
}

//-- Function to receive gesture commands
void GesturePlay(int gesture) {
  switch (gesture) {
    case 1: //H 1
      Otto.playGesture(OttoHappy);
      break;
    case 2: //H 2
      Otto.playGesture(OttoSuperHappy);
      break;
    case 3: //H 3
      Otto.playGesture(OttoSad);
      break;
    case 4: //H 4
      Otto.playGesture(OttoSleeping);
      break;
    case 5: //H 5
      Otto.playGesture(OttoFart);
      break;
    case 6: //H 6
      Otto.playGesture(OttoConfused);
      break;
    case 7: //H 7
      Otto.playGesture(OttoLove);
      break;
    case 8: //H 8
      Otto.playGesture(OttoAngry);
      break;
    case 9: //H 9
      Otto.playGesture(OttoFretful);
      break;
    case 10: //H 10
      Otto.playGesture(OttoMagic);
      break;
    case 11: //H 11
      Otto.playGesture(OttoWave);
      break;
    case 12: //H 12
      Otto.playGesture(OttoVictory);
      break;
    case 13: //H 13
      Otto.playGesture(OttoFail);
      break;
    default:
      break;
  }
  if (gesture != 13) {
    Otto.home();
  }
}

//-- Function to receive sing commands
void SingSong(int song) {
  switch (song) {
    case 1: //K 1
      Otto.putMouth(happyOpen);
      Otto.sing(S_connection);
      break;
    case 2: //K 2
      //Otto.putMouth(angry);
      Otto.putAnimationMouth(adivinawi,1);
      Otto.sing(S_disconnection);
      break;
    case 3: //K 3
      Otto.putMouth(bigSurprise);
      Otto.sing(S_surprise);
      break;
    case 4: //K 4
      Otto.putMouth(bigSurprise);
      Otto.sing(S_OhOoh);
      break;
    case 5: //K 5
      Otto.sing(S_OhOoh2);
      break;
    case 6: //K 6
      Otto.putMouth(heart);
      Otto.sing(S_cuddly);
      break;
    case 7: //K 7
      Otto.putMouth(smallSurprise);
      Otto.sing(S_sleeping);
      break;
    case 8: //K 8
      Otto.putMouth(smile);
      Otto.sing(S_happy);
      break;
    case 9: //K 9
      Otto.putMouth(happyOpen);
      Otto.sing(S_superHappy);
      break;
    case 10: //K 10
      Otto.putMouth(smile);
      Otto.sing(S_happy_short);
      break;
    case 11: //K 11
      Otto.putMouth(sad);
      Otto.sing(S_sad);
      break;
    case 12: //K 12
      Otto.putMouth(confused);
      Otto.sing(S_confused);
      break;
    case 13: //K 13
      Otto.putMouth(tongueOut);
      Otto.sing(S_fart1);
      break;
    case 14: //K 14
      Otto.putMouth(tongueOut);
      Otto.sing(S_fart2);
      break;
    case 15: //K 15
      Otto.putMouth(tongueOut);
      Otto.sing(S_fart3);
      break;
    case 16: //K 16
      Otto.putMouth(one);
      Otto.sing(S_mode1);
      break;
    case 17: //K 17
      Otto.putMouth(two);
      Otto.sing(S_mode2);
      break;
    case 18: //K 18
      Otto.putMouth(three);
      Otto.sing(S_mode3);
      break;
    case 19: //K 19
      Otto.putMouth(okMouth);
      Otto.sing(S_buttonPushed);
      break;
    default:
      break;
  }
}
