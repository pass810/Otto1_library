//*********************************************************
//-- OttoESP32: Library(cpp file)
//*********************************************************
#include "OttoESP32.h"
//---------------------------------------------------------
//-- OttoESP32: init
//-- Initialize servos,ssd1306,pca9685
//--  Parameters:
//--    zero_position[]: correct servos
//---------------------------------------------------------
void OttoESP32::init(int zero_position[]) {
  int i;
  //zero_position Init to correct servos
  for (i = 0; i < 6; i++) _zero_position[i] = zero_position[i];
  //Servo Module PCA9685 Init
  pwm = new Adafruit_PWMServoDriver();
  pwm->begin();
  pwm->setPWMFreq(FREQ);  // Analog servos run at ~60 Hz updates

  //OLED SSD1306 Init
  u8g2 = new U8G2_SSD1306_128X64_NONAME_1_HW_I2C(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
  u8g2->begin();
  u8g2->enableUTF8Print();    // enable UTF8 support for the Arduino print() function
  u8g2->setFont(u8g2_font_b16_t_japanese1);  //  japanese1: Lerning Level 1-6
  u8g2->setFontDirection(0);
}

//*********************************************************
//-- OttoESP32: basic moves
//*********************************************************
void OttoESP32::execute(int A[6], int O[6], int T, double phase_diff[6], int steps) {
  int i;
  int pos[i];
  double ts = 30;
  double phase = 0;
  double nn, inc;
  long previousMillis;
  long currentMillis;
  nn = T / ts;
  inc = 2 * M_PI / nn;
  double ref = millis();
  previousMillis = ref;
  for (double x = ref; x <= T * steps + ref; x = millis()) {
    currentMillis = millis();
    if ((currentMillis - previousMillis) > ts) {
      previousMillis = currentMillis;
      for (i = 0; i < 6; i++) {
        pos[i] = round(A[i] * sin(phase + phase_diff[i]) + O[i] + _zero_position[i]);
        servo_move(i, pos[i] + 90);
      }
      phase += inc;
    }
  }
}

void OttoESP32::moveServos(int time, int servo_target[]) {
  unsigned long final_time;
  unsigned long partial_time;
  float increment[6];
  for (int i = 0; i < 6; i++) {
    servo_move(i, _servo_position[i]);
  }
  if (time > 10) {
    for (int i = 0; i < 6; i++) increment[i] = ((servo_target[i]) - _servo_position[i]) / (time / 10.0);
    final_time =  millis() + time;

    for (int iteration = 1; millis() < final_time; iteration++) {
      partial_time = millis() + 10;
      for (int i = 0; i < 6; i++) servo_move(i, _servo_position[i] + _zero_position[i] + (iteration * increment[i]));
      while (millis() < partial_time); //pause
    }
  }
  else {
    for (int i = 0; i < 6; i++) servo_move(i, servo_target[i] + _zero_position[i]);
  }
  for (int i = 0; i < 6; i++) _servo_position[i] = servo_target[i];
}

void OttoESP32::moveServos2(int time, int servo_target[]) { //simple move
  for (int i = 0; i < 6; i++) servo_move(i, servo_target[i] + _zero_position[i]);
  delay(time);
}

void OttoESP32::servo_move(int target, int degree)
{
  float vv = degree / 180.0;
  float dd;
  dd = SERVOMIN + vv * (SERVOMAX - SERVOMIN);
  //Serial.println(ans);
  pwm->setPWM(target, 0, (int)dd);
}

//*********************************************************
//-- OttoESP32: normal moves
//*********************************************************
//---------------------------------------------------------
//-- OttoESP32: home(move to home position)
//---------------------------------------------------------
void OttoESP32::home() {
  int homes[6] = {90, 90, 90, 90, 0, 180}; //All the servos at rest position
  moveServos(500, homes);  //Move the servos in half a second
}

//---------------------------------------------------------
//-- OttoESP32: jump
//--  Parameters:
//--    * steps:  Number of steps
//--    * T : Period
//---------------------------------------------------------
void OttoESP32::jump(int steps, int T ) {
  int i;
  int up[] = {90, 90, 150, 30, 150, 30};
  int down[] = {90, 90, 90, 90, 30, 150};
  for (i = 0; i < steps; i++) {
    moveServos(T, up);
    moveServos(T, down);
  }
}

//---------------------------------------------------------
//-- OttoESP32: Walking  (forward or backward)
//--  Parameters:
//--    * steps:  Number of steps
//--    * T : Period
//--    * Dir: Direction: FORWARD / BACKWARD
//---------------------------------------------------------
void OttoESP32::walk(int steps, int T,  int dir) {
  //  int A[6] = {30, 28, 20, 18, -20, -20};
  //  int O[6] = {0, 0, 4, -4, -70, 70};
  int A[6] = {30, 30, 20, 20, -20, -20};
  int O[6] = {0, 0, 3, -3, -70, 70};
  double phase_diff[6] = {0, 0, DEG2RAD(dir * -90), DEG2RAD(dir * -90), 0, 0};
  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- OttoESP32: Turning (left or right)
//--  Parameters:
//--   * Steps: Number of steps
//--   * T: Period
//--   * Dir: Direction: LEFT / RIGHT
//---------------------------------------------------------
void OttoESP32::turn(int steps, int T,  int dir) {
  int A[6] = {30, 30, 20, 20, -20, -20};
  int O[6] = {0, 0, 3, -3, -70, 70};
  double phase_diff[6] = {0, 0, DEG2RAD(-90), DEG2RAD(-90), 0, 0};
  if (dir == 1) {
    A[0] = 30;
    A[1] = 10;
  } else {
    A[0] = 10;
    A[1] = 30;
  }
  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- OttoESP32: Lateral bend
//--  Parameters:
//--    steps: Number of bends
//--    T: Period of one bend
//--    dir: RIGHT=Right bend LEFT=Left bend
//---------------------------------------------------------
void OttoESP32::bend (int steps, int T, int dir) {

  //Parameters of all the movements. Default: Left bend
  int bend1[6] = {90, 90, 70, 35, 0, 135}; //62->70
  int bend2[6] = {90, 90, 70, 105, 0, 135};
  int homes[6] = {90, 90, 90, 90, 0, 180};

  //Time of one bend, constrained in order to avoid movements too fast.
  //T=max(T, 600);

  //Changes in the parameters if right direction is chosen
  if (dir == -1)
  {
    bend1[2] = 180 - 35;
    bend1[3] = 180 - 70; //Not 65. Otto is unbalanced 60->62->70
    bend1[4] = 45;
    bend1[5] = 180;
    bend2[2] = 180 - 105;
    bend2[3] = 180 - 70;
    bend2[4] = 45;
    bend2[5] = 180;
  }

  //Time of the bend movement. Fixed parameter to avoid falls
  int T2 = 800;

  //Bend movement
  for (int i = 0; i < steps; i++)
  {
    moveServos(T2 / 2, bend1);
    moveServos(T2 / 2, bend2);
    delay(T * 0.8);
    moveServos(500, homes);
  }
}

//---------------------------------------------------------
//-- OttoESP32: Shake a leg
//--  Parameters:
//--    steps: Number of shakes
//--    T: Period of one shake
//--    dir: RIGHT=Right leg LEFT=Left leg
//---------------------------------------------------------
void OttoESP32::shakeLeg (int steps, int T, int dir) {

  //This variable change the amount of shakes
  int numberLegMoves = 2;

  //Parameters of all the movements. Default: Right leg
  int shake_leg1[6] = {90, 90, 70, 35, 0, 150}; //58->68
  int shake_leg2[6] = {90, 90, 70, 120, 0, 150};
  int shake_leg3[6] = {90, 90, 70, 60, 0, 150};
  int homes[6] = {90, 90, 90, 90, 0, 180};

  //Changes in the parameters if left leg is chosen
  if (dir == -1)
  {
    shake_leg1[2] = 180 - 35;
    shake_leg1[3] = 180 - 70;
    shake_leg1[4] = 40;
    shake_leg1[5] = 180;
    shake_leg2[2] = 180 - 120;
    shake_leg2[3] = 180 - 70;
    shake_leg2[4] = 40;
    shake_leg2[5] = 180;
    shake_leg3[2] = 180 - 60;
    shake_leg3[3] = 180 - 70;
    shake_leg3[4] = 40;
    shake_leg3[5] = 180;
  }

  //Time of the bend movement. Fixed parameter to avoid falls
  int T2 = 1000;
  //Time of one shake, constrained in order to avoid movements too fast.
  T = T - T2;
  T = max(T, 200 * numberLegMoves);

  for (int j = 0; j < steps; j++)
  {
    //Bend movement
    moveServos(T2 / 2, shake_leg1);
    moveServos(T2 / 2, shake_leg2);

    //Shake movement
    for (int i = 0; i < numberLegMoves; i++)
    {
      moveServos(T / (2 * numberLegMoves), shake_leg3);
      moveServos(T / (2 * numberLegMoves), shake_leg2);
    }
    moveServos(500, homes); //Return to home position
  }

  delay(T);
}

//---------------------------------------------------------
//-- OttoESP32: up & down
//--  Parameters:
//--    * steps: Number of jumps
//--    * T: Period
//--    * h: Jump height: SMALL 5/ MEDIUM 15/ BIG 30
//--              (or a number in degrees 0 - 90)
//---------------------------------------------------------
void OttoESP32::updown(float steps, int T, int h) {

  //-- Both feet are 180 degrees out of phase
  //-- Feet amplitude and offset are the same
  //-- Initial phase for the right foot is -90, so that it starts
  //--   in one extreme position (not in the middle)
  int A[6] = {0, 0, h, h, h, h};
  int O[6] = {0, 0, h, -h, 0, 0};
  double phase_diff[6] = {0, 0, DEG2RAD(-90), DEG2RAD(90), DEG2RAD(-90), DEG2RAD(90)};

  //-- Let's execute the servos!
  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- OttoESP32: swinging side to side
//--  Parameters:
//--     steps: Number of steps
//--     T : Period
//--     h : Amount of swing (from 0 to 50 aprox)
//---------------------------------------------------------
void OttoESP32::swing(float steps, int T, int h) {

  //-- Both feets are in phase. The offset is half the amplitude
  //-- It causes the robot to swing from side to side
  int A[6] = {0, 0, h, h, h, h};
  int O[6] = {0, 0, h / 2, -h / 2, 0, 0};
  double phase_diff[6] = {0, 0, DEG2RAD(0), DEG2RAD(0), DEG2RAD(90), DEG2RAD(90)};

  //-- Let's execute the servos!
  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- OttoESP32: swinging side to side without touching the floor with the heel
//--  Parameters:
//--     steps: Number of steps
//--     T : Period
//--     h : Amount of swing (from 0 to 50 aprox)
//---------------------------------------------------------
void OttoESP32::tiptoeSwing(float steps, int T, int h) {

  //-- Both feets are in phase. The offset is not half the amplitude in order to tiptoe
  //-- It causes the robot to swing from side to side
  int A[6] = {0, 0, h, h, h, h};
  int O[6] = {0, 0, h, -h, 0, 0};
  double phase_diff[6] = {0, 0, 0, 0, DEG2RAD(90), DEG2RAD(90)};

  //-- Let's execute the servos!
  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- OttoESP32: Jitter
//--  Parameters:
//--    steps: Number of jitters
//--    T: Period of one jitter
//--    h: height (Values between 5 - 25)
//---------------------------------------------------------
void OttoESP32::jitter(float steps, int T, int h) {

  //-- Both feet are 180 degrees out of phase
  //-- Feet amplitude and offset are the same
  //-- Initial phase for the right foot is -90, so that it starts
  //--   in one extreme position (not in the middle)
  //-- h is constrained to avoid hit the feets
  h = min(25, h);
  int A[6] = {h, h, 0, 0, 10, -10};
  int O[6] = {0, 0, 0, 0, -80, 80};
  double phase_diff[6] = {DEG2RAD(-90), DEG2RAD(90), 0, 0, DEG2RAD(90), DEG2RAD(90)};

  //-- Let's execute the servos!
  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- OttoESP32: Ascending & turn (Jitter while up&down)
//--  Parameters:
//--    steps: Number of bends
//--    T: Period of one bend
//--    h: height (Values between 5 - 15)
//---------------------------------------------------------
void OttoESP32::ascendingTurn(float steps, int T, int h) {

  //-- Both feet and legs are 180 degrees out of phase
  //-- Initial phase for the right foot is -90, so that it starts
  //--   in one extreme position (not in the middle)
  //-- h is constrained to avoid hit the feets
  h = min(13, h);
  int A[6] = {h, h, h, h, h, h};
  int O[6] = {0, 0, h + 4, -h + 4, -90 + h, 90 - h};
  double phase_diff[6] = {DEG2RAD(-90), DEG2RAD(90), DEG2RAD(-90), DEG2RAD(90), DEG2RAD(-90), DEG2RAD(90)};

  //-- Let's execute the servos!
  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- OttoESP32: Moonwalker. Otto moves like Michael Jackson
//--  Parameters:
//--    Steps: Number of steps
//--    T: Period
//--    h: Height. Typical valures between 15 and 40
//--    dir: Direction: LEFT / RIGHT
//---------------------------------------------------------
void OttoESP32::moonwalker(float steps, int T, int h, int dir) {

  //-- This motion is similar to that of the caterpillar robots: A travelling
  //-- wave moving from one side to another
  //-- The two Otto's feet are equivalent to a minimal configuration. It is known
  //-- that 2 servos can move like a worm if they are 120 degrees out of phase
  //-- In the example of Otto, the two feet are mirrored so that we have:
  //--    180 - 120 = 60 degrees. The actual phase difference given to the oscillators
  //--  is 60 degrees.
  //--  Both amplitudes are equal. The offset is half the amplitud plus a little bit of
  //-   offset so that the robot tiptoe lightly

  int A[6] = {0, 0, h, h, h, h};
  int O[6] = {0, 0, h / 2 + 2, -h / 2 - 2, -90 + h, 90 - h};
  int phi = -dir * 90;
  double phase_diff[6] = {0, 0, DEG2RAD(phi), DEG2RAD(-60 * dir + phi), DEG2RAD(phi), DEG2RAD(-60 * dir + phi)};

  //-- Let's execute the servos!
  execute(A, O, T, phase_diff, steps);
}

//----------------------------------------------------------
//-- OttoESP32: Crusaito. A mixture between moonwalker and walk
//--   Parameters:
//--     steps: Number of steps
//--     T: Period
//--     h: height (Values between 20 - 50)
//--     dir:  Direction: LEFT / RIGHT
//-----------------------------------------------------------
void OttoESP32::crusaito(float steps, int T, int h, int dir) {

  int A[6] = {25, 25, h, h, h, h};
  int O[6] = {0, 0, h / 2 + 4, -h / 2 - 4, -90 + h, 90 - h};
  double phase_diff[6] = {90, 90, DEG2RAD(0), DEG2RAD(-60 * dir), DEG2RAD(0), DEG2RAD(-60 * dir)};

  //-- Let's execute the servos!
  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- OttoESP32: Flapping
//--  Parameters:
//--    steps: Number of steps
//--    T: Period
//--    h: height (Values between 10 - 30)
//--    dir: direction: FOREWARD, BACKWARD
//---------------------------------------------------------
void OttoESP32::flapping(float steps, int T, int h, int dir) {

  int A[6] = {12, 12, h, h, h, h};
  int O[6] = {0, 0, h - 10, -h + 10, -90 + h, 90 - h};
  double phase_diff[6] = {DEG2RAD(0), DEG2RAD(180), DEG2RAD(-90 * dir), DEG2RAD(90 * dir), DEG2RAD(-90 * dir), DEG2RAD(90 * dir)};

  //-- Let's execute the servos!
  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- OttoESP32: handup
//---------------------------------------------------------
void OttoESP32::handsup() {
  int homes[6] = {90, 90, 90, 90, 160, 20}; //
  moveServos(500, homes);  //Move the servos in half a second
}

//---------------------------------------------------------
//-- OttoESP32: handwave
//--  Parameters:
//--    dir: direction: LEFT,RIGHT
//---------------------------------------------------------
void OttoESP32::handwave(int dir) {
  //-- Wave , either left or right
  //--
  if (dir == RIGHT)
  {
    int A[6] = {0, 0, 0, 0, 0, 30}; // right hand wave
    int O[6] = {0, 0, 0, 0, -40, -30};
    double phase_diff[6] = {0, 0, 0, 0, 0,DEG2RAD(0)};
    //-- Let's oscillate the servos!
    execute(A, O, 500, phase_diff, 5);
  }
  if (dir == LEFT)
  {

    int A[6] = {0, 0, 0, 0, 30,0}; // left hand wave
    int O[6] = {0, 0, 0, 0, 60, 40};
    double phase_diff[6] = {0, 0, 0, 0,  DEG2RAD(0),0};
    //-- Let's oscillate the servos!
    execute(A, O, 500, phase_diff, 5);
  }
}


//*********************************************************
//-- OttoESP32: Gesture
//*********************************************************
void OttoESP32::playGesture(int gesture) {

  int sadPos[6] =      {110, 70, 20, 160, 0, 180};
  int bedPos[6] =      {100, 80, 60, 120, 0, 180};
  int fartPos_1[6] =   {90, 90, 145, 110, 45, 180}; //rightBend 122->110 145->135
  int fartPos_2[6] =   {90, 90, 80, 110, 45, 180};
  int fartPos_3[6] =   {90, 90, 130, 85, 0, 180};
  int confusedPos[6] = {110, 70, 90, 90, 45, 135};
  int angryPos[6] =    {90, 90, 70, 110, 0, 180};
  int headLeft[6] =    {110, 110, 90, 90, 45, 180};
  int headRight[6] =   {70, 70, 90, 90, 0, 180};
  int fretfulPos[6] =  {90, 90, 90, 120, 30, 180};
  int bendPos_1[6] =   {90, 90, 70, 35, 0, 180};
  int bendPos_2[6] =   {90, 90, 55, 35, 90, 180};
  int bendPos_3[6] =   {90, 90, 42, 35, 180, 180};
  int bendPos_4[6] =   {90, 90, 34, 35, 180, 180};

  switch (gesture) {

    case OttoHappy:
      tone(note_E5, 50, 30);
      putMouth(smile);
      sing(S_happy_short);
      swing(1, 800, 20);
      sing(S_happy_short);

      home();
      putMouth(happyOpen);
      break;


    case OttoSuperHappy:
      putMouth(happyOpen);
      sing(S_happy);
      putMouth(happyClosed);
      tiptoeSwing(1, 500, 20);
      putMouth(happyOpen);
      sing(S_superHappy);
      putMouth(happyClosed);
      tiptoeSwing(1, 500, 20);

      home();
      putMouth(happyOpen);
      break;


    case OttoSad:
      putMouth(sad);
      moveServos(700, sadPos);
      bendTones(880, 830, 1.02, 20, 200);
      putMouth(sadClosed);
      bendTones(830, 790, 1.02, 20, 200);
      putMouth(sadOpen);
      bendTones(790, 740, 1.02, 20, 200);
      putMouth(sadClosed);
      bendTones(740, 700, 1.02, 20, 200);
      putMouth(sadOpen);
      bendTones(700, 669, 1.02, 20, 200);
      putMouth(sad);
      delay(500);

      home();
      delay(300);
      putMouth(happyOpen);
      break;


    case OttoSleeping:
      moveServos(700, bedPos);

      for (int i = 0; i < 4; i++) {
        putAnimationMouth(dreamMouth, 0);
        bendTones (100, 200, 1.04, 10, 10);
        putAnimationMouth(dreamMouth, 1);
        bendTones (200, 300, 1.04, 10, 10);
        putAnimationMouth(dreamMouth, 2);
        bendTones (300, 500, 1.04, 10, 10);
        delay(500);
        putAnimationMouth(dreamMouth, 1);
        bendTones (400, 250, 1.04, 10, 1);
        putAnimationMouth(dreamMouth, 0);
        bendTones (250, 100, 1.04, 10, 1);
        delay(500);
      }

      putMouth(lineMouth);
      sing(S_cuddly);

      home();
      putMouth(happyOpen);
      break;


    case OttoFart:
      moveServos(500, fartPos_1);
      delay(300);
      putMouth(lineMouth);
      sing(S_fart1);
      putMouth(tongueOut);
      delay(250);
      moveServos(500, fartPos_2);
      delay(300);
      putMouth(lineMouth);
      sing(S_fart2);
      putMouth(tongueOut);
      delay(800);//250->800
      moveServos(500, fartPos_3);
      delay(300);
      putMouth(lineMouth);
      sing(S_fart3);
      putMouth(tongueOut);
      delay(300);

      home();
      delay(500);
      putMouth(happyOpen);
      break;


    case OttoConfused:
      moveServos(300, confusedPos);
      putMouth(confused);
      sing(S_confused);
      delay(500);

      home();
      putMouth(happyOpen);
      break;


    case OttoLove:
      putMouth(heart);
      sing(S_cuddly);
      crusaito(2, 1500, 15, 1);

      home();
      sing(S_happy_short);
      putMouth(happyOpen);
      break;


    case OttoAngry:
      moveServos(300, angryPos);
      putMouth(angry);

      tone(note_A5, 100, 30);
      bendTones(note_A5, note_D6, 1.02, 7, 4);
      bendTones(note_D6, note_G6, 1.02, 10, 1);
      bendTones(note_G6, note_A5, 1.02, 10, 1);
      delay(15);
      bendTones(note_A5, note_E5, 1.02, 20, 4);
      delay(400);
      moveServos(200, headLeft);
      bendTones(note_A5, note_D6, 1.02, 20, 4);
      moveServos(200, headRight);
      bendTones(note_A5, note_E5, 1.02, 20, 4);

      home();
      putMouth(happyOpen);
      break;


    case OttoFretful:
      putMouth(angry);
      bendTones(note_A5, note_D6, 1.02, 20, 4);
      bendTones(note_A5, note_E5, 1.02, 20, 4);
      delay(300);
      putMouth(lineMouth);

      for (int i = 0; i < 4; i++) {
        moveServos(100, fretfulPos);
        home();
      }

      putMouth(angry);
      delay(500);

      home();
      putMouth(happyOpen);
      break;


    case OttoMagic:

      //Initial note frecuency = 400
      //Final note frecuency = 1000

      // Reproduce the animation four times
      for (int i = 0; i < 4; i++) {

        int noteM = 400;

        for (int index = 0; index < 6; index++) {
          putAnimationMouth(adivinawi, index);
          bendTones(noteM, noteM + 100, 1.04, 10, 10);  //400 -> 1000
          noteM += 100;
        }

        bendTones(noteM - 100, noteM + 100, 1.04, 10, 10); //900 -> 1100

        for (int index = 0; index < 6; index++) {
          putAnimationMouth(adivinawi, index);
          bendTones(noteM, noteM + 100, 1.04, 10, 10);  //1000 -> 400
          noteM -= 100;
        }
      }

      delay(300);
      putMouth(happyOpen);
      break;


    case OttoWave:

      // Reproduce the animation four times
      for (int i = 0; i < 2; i++) {

        int noteW = 500;

        for (int index = 0; index < 10; index++) {
          putAnimationMouth(wave, index);
          bendTones(noteW, noteW + 100, 1.02, 10, 10);
          noteW += 101;
        }
        for (int index = 0; index < 10; index++) {
          putAnimationMouth(wave, index);
          bendTones(noteW, noteW + 100, 1.02, 10, 10);
          noteW += 101;
        }
        for (int index = 0; index < 10; index++) {
          putAnimationMouth(wave, index);
          bendTones(noteW, noteW - 100, 1.02, 10, 10);
          noteW -= 101;
        }
        for (int index = 0; index < 10; index++) {
          putAnimationMouth(wave, index);
          bendTones(noteW, noteW - 100, 1.02, 10, 10);
          noteW -= 101;
        }
      }
      delay(100);
      putMouth(happyOpen);
      break;

    case OttoVictory:

      putMouth(smallSurprise);
      //final pos   = {90,90,150,30}
      for (int i = 0; i < 60; ++i) {
        int pos[] = {90, 90, 90 + i, 90 - i, i, 180 - i};
        moveServos(10, pos);
        tone(1600 + i * 20, 15, 1);
      }

      putMouth(bigSurprise);
      //final pos   = {90,90,90,90}
      for (int i = 0; i < 60; ++i) {
        int pos[] = {90, 90, 150 - i, 30 + i, 60 - i, 120 + i};
        moveServos(10, pos);
        tone(2800 + i * 20, 15, 1);
      }

      putMouth(happyOpen);
      //SUPER HAPPY
      //-----
      tiptoeSwing(1, 500, 20);
      sing(S_superHappy);
      putMouth(happyClosed);
      tiptoeSwing(1, 500, 20);
      //-----

      home();
      putMouth(happyOpen);

      break;

    case OttoFail:

      putMouth(sadOpen);
      moveServos(300, bendPos_1);
      tone(900, 200, 1);
      putMouth(sadClosed);
      moveServos(300, bendPos_2);
      tone(600, 200, 1);
      putMouth(confused);
      moveServos(300, bendPos_3);
      tone(300, 200, 1);
      moveServos(300, bendPos_4);
      putMouth(xMouth);
      //detachServos();
      tone(150, 2200, 1);
      delay(2000);
      putMouth(happyOpen);
      home();
      break;
  }
}

//*********************************************************
//-- OttoESP32: mouth
//*********************************************************
unsigned long int OttoESP32::getAnimShape(int anim, int index) {

  unsigned long int littleUuh_code[] = {
    0b00000000000000001100001100000000,
    0b00000000000000000110000110000000,
    0b00000000000000000011000011000000,
    0b00000000000000000110000110000000,
    0b00000000000000001100001100000000,
    0b00000000000000011000011000000000,
    0b00000000000000110000110000000000,
    0b00000000000000011000011000000000
  };

  unsigned long int dreamMouth_code[] = {
    0b00000000000000000000110000110000,
    0b00000000000000010000101000010000,
    0b00000000011000100100100100011000,
    0b00000000000000010000101000010000
  };

  unsigned long int adivinawi_code[] = {
    0b00100001000000000000000000100001,
    0b00010010100001000000100001010010,
    0b00001100010010100001010010001100,
    0b00000000001100010010001100000000,
    0b00000000000000001100000000000000,
    0b00000000000000000000000000000000
  };

  unsigned long int wave_code[] = {
    0b00001100010010100001000000000000,
    0b00000110001001010000100000000000,
    0b00000011000100001000010000100000,
    0b00000001000010000100001000110000,
    0b00000000000001000010100100011000,
    0b00000000000000100001010010001100,
    0b00000000100000010000001001000110,
    0b00100000010000001000000100000011,
    0b00110000001000000100000010000001,
    0b00011000100100000010000001000000
  };

  switch  (anim) {

    case littleUuh:
      return littleUuh_code[index];
      break;
    case dreamMouth:
      return dreamMouth_code[index];
      break;
    case adivinawi:
      return adivinawi_code[index];
      break;
    case wave:
      return wave_code[index];
      break;
  }
}


void OttoESP32::putAnimationMouth(unsigned long int aniMouth, int index) {
  int i, j, x, y;
  unsigned long int mouthShape;
  mouthShape = getAnimShape(aniMouth, index);
  u8g2->firstPage();
  do {
    for (i = 0; i < 5; i++) {
      for (j = 0; j < 6; j++) {
        if (mouthShape & (1 << (i * 6 + j))) {
          x = 128 - (j * 16 + 32);
          //y = 64 - (i * 8 + 16);
          y = 64 - (i * 12 + 12);
          //u8g2.drawRFrame(x, y,16,8, 3);
          u8g2->drawRBox(x, y, 16, 12, 3);
        }
      }
    }
  } while ( u8g2->nextPage() );
}

unsigned long int OttoESP32::getMouthShape(int number) {
  unsigned long int types [] = {zero_code, one_code, two_code, three_code, four_code, five_code, six_code, seven_code, eight_code,
                                nine_code, smile_code, happyOpen_code, happyClosed_code, heart_code, bigSurprise_code, smallSurprise_code, tongueOut_code,
                                vamp1_code, vamp2_code, lineMouth_code, confused_code, diagonal_code, sad_code, sadOpen_code, sadClosed_code,
                                okMouth_code, xMouth_code, interrogation_code, thunder_code, culito_code, angry_code
                               };

  return types[number];
}

void OttoESP32::putMouth(unsigned long int mouth) {
  int i, j, x, y;
  unsigned long int mouthShape;
  mouthShape = getMouthShape(mouth);
  //u8g2.setFontDirection(0);
  u8g2->firstPage();
  do {
    for (i = 0; i < 5; i++) {
      for (j = 0; j < 6; j++) {
        if ((mouthShape & (1 << (i * 6 + j))) != 0) {
          x = 128 - (j * 16 + 32);
          y = 64 - (i * 12 + 12);
          u8g2->drawRBox(x, y, 16, 12, 3);
        }
      }
    }
  } while ( u8g2->nextPage() );
}

void OttoESP32::oled_print(char str[]) {
  u8g2->firstPage();
  do {
    u8g2->setCursor(0, 17);
    u8g2->print("Otto ESP32");
    u8g2->setCursor(0, 34);
    u8g2->print(str);
  } while ( u8g2->nextPage() );
}
//*********************************************************
//-- OttoESP32: sound
//*********************************************************
void OttoESP32::tone(float frequency, long noteDuration, int silentDuration)
{
  uint8_t pin = TONE_PIN;
  uint8_t channel = TONE_CHANNEL;
  if (silentDuration == 0) {
    silentDuration = 1;
  }
  if (ledcRead(channel)) {
    log_e("Tone channel %d is already in use", ledcRead(channel));
    return;
  }
  ledcAttachPin(pin, channel);
  ledcWriteTone(channel, frequency);
  if (noteDuration) {
    delay(noteDuration);
    noTone();
    delay(silentDuration);
  }
}

void OttoESP32::noTone()
{
  uint8_t pin = TONE_PIN;
  uint8_t channel = TONE_CHANNEL;
  ledcDetachPin(pin);
  ledcWrite(channel, 0);
}

void OttoESP32::bendTones (float initFrequency, float finalFrequency, float prop, long noteDuration, int silentDuration) {

  //Examples:
  //  bendTones (880, 2093, 1.02, 18, 1);
  //  bendTones (note_A5, note_C7, 1.02, 18, 0);

  if (silentDuration == 0) {
    silentDuration = 1;
  }

  if (initFrequency < finalFrequency)
  {
    for (int i = initFrequency; i < finalFrequency; i = i * prop) {
      tone(i, noteDuration, silentDuration);
    }

  } else {

    for (int i = initFrequency; i > finalFrequency; i = i / prop) {
      tone(i, noteDuration, silentDuration);
    }
  }
}

void OttoESP32::sing(int songName) {
  switch (songName) {

    case S_connection:
      tone(note_E5, 50, 30);
      tone(note_E6, 55, 25);
      tone(note_A6, 60, 10);
      break;

    case S_disconnection:
      tone(note_E5, 50, 30);
      tone(note_A6, 55, 25);
      tone(note_E6, 50, 10);
      break;

    case S_buttonPushed:
      bendTones (note_E6, note_G6, 1.03, 20, 2);
      delay(30);
      bendTones (note_E6, note_D7, 1.04, 10, 2);
      break;

    case S_mode1:
      bendTones (note_E6, note_A6, 1.02, 30, 10);  //1318.51 to 1760
      break;

    case S_mode2:
      bendTones (note_G6, note_D7, 1.03, 30, 10);  //1567.98 to 2349.32
      break;

    case S_mode3:
      tone(note_E6, 50, 100); //D6
      tone(note_G6, 50, 80); //E6
      tone(note_D7, 300, 0); //G6
      break;

    case S_surprise:
      bendTones(800, 2150, 1.02, 10, 1);
      bendTones(2149, 800, 1.03, 7, 1);
      break;

    case S_OhOoh:
      bendTones(880, 2000, 1.04, 8, 3); //A5 = 880
      delay(200);

      for (int i = 880; i < 2000; i = i * 1.04) {
        tone(note_B5, 5, 10);
      }
      break;

    case S_OhOoh2:
      bendTones(1880, 3000, 1.03, 8, 3);
      delay(200);

      for (int i = 1880; i < 3000; i = i * 1.03) {
        tone(note_C6, 10, 10);
      }
      break;

    case S_cuddly:
      bendTones(700, 900, 1.03, 16, 4);
      bendTones(899, 650, 1.01, 18, 7);
      break;

    case S_sleeping:
      bendTones(100, 500, 1.04, 10, 10);
      delay(500);
      bendTones(400, 100, 1.04, 10, 1);
      break;

    case S_happy:
      bendTones(1500, 2500, 1.05, 20, 8);
      bendTones(2499, 1500, 1.05, 25, 8);
      break;

    case S_superHappy:
      bendTones(2000, 6000, 1.05, 8, 3);
      delay(50);
      bendTones(5999, 2000, 1.05, 13, 2);
      break;

    case S_happy_short:
      bendTones(1500, 2000, 1.05, 15, 8);
      delay(100);
      bendTones(1900, 2500, 1.05, 10, 8);
      break;

    case S_sad:
      bendTones(880, 669, 1.02, 20, 200);
      break;

    case S_confused:
      bendTones(1000, 1700, 1.03, 8, 2);
      bendTones(1699, 500, 1.04, 8, 3);
      bendTones(1000, 1700, 1.05, 9, 10);
      break;

    case S_fart1:
      bendTones(1600, 3000, 1.02, 2, 15);
      break;

    case S_fart2:
      bendTones(2000, 6000, 1.02, 2, 20);
      break;

    case S_fart3:
      bendTones(1600, 4000, 1.02, 2, 20);
      bendTones(4000, 3000, 1.02, 2, 20);
      break;

  }
}
