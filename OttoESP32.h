//*********************************************************
//-- OttoESP32: Library (Header file)
//*********************************************************
#ifndef OttoESP32_h
#define OttoESP32_h
#include <U8g2lib.h>
#include <Adafruit_PWMServoDriver.h>
#include "Otto_sounds.h"
#include "Otto_gestures.h"
#include "Otto_mouths.h"

//OttoESP32
#define FORWARD     1
#define BACKWARD    -1
#define LEFT        1
#define RIGHT       -1
#define SMALL       5
#define MEDIUM      15
#define BIG         30

//PCA9685,servos
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096) 4096*0.6*60/1000
#define SERVOMAX  590 // this is the 'maximum' pulse length count (out of 4096) 4096*2.4*60/1000
#define FREQ  60 // frequency(Hz)
#define FN 6 //degree of freedom
#define DEG2RAD(g) ((g)*M_PI)/180 //degree->radian

//Sounds
#define TONE_CHANNEL 0
#define TONE_PIN 12

//OttoESP32 class
class OttoESP32 {
  public:
    //Init
    void init(int zero_position[]);
    //basic move
    void execute(int A[FN], int O[FN], int T, double phase_diff[FN], int steps);
    void moveServos(int time, int servo_target[]);
    void moveServos2(int time, int servo_target[]);
    void servo_move(int target, int degree);
    //normal Move
    void home();
    void jump(int steps = 1,int T = 2000);
    void walk(int steps = 4,int T = 1000,  int dir = FORWARD);
    void turn(int steps = 4,int T = 2000,  int dir = LEFT);
    void bend (int steps = 1, int T=1400, int dir = LEFT);
    void shakeLeg (int steps = 1, int T = 2000, int dir = RIGHT);
    void updown(float steps = 1, int T = 1000, int h = 20);
    void swing(float steps = 1, int T = 1000, int h = 20);
    void tiptoeSwing(float steps = 1, int T = 900, int h = 20);
    void jitter(float steps = 1, int T = 500, int h = 20);
    void ascendingTurn(float steps = 1, int T = 900, int h = 20);
    void moonwalker(float steps = 1, int T = 900, int h = 20, int dir = LEFT);
    void crusaito(float steps = 1,  int T = 900, int h = 20, int dir = FORWARD);
    void flapping(float steps = 1, int T = 1000, int h = 20, int dir = FORWARD);
    void handsup();
    void handwave(int dir = LEFT);
    //Gestures
    void playGesture(int gesture);    
    //Mouth
    void oled_print(char str[]);
    void putAnimationMouth(unsigned long int aniMouth, int index);
    void putMouth(unsigned long int mouth);
    //Sound
    void tone(float frequency, long noteDuration, int silentDuration);
    void noTone();
    void bendTones (float initFrequency, float finalFrequency, float prop, long noteDuration, int silentDuration);
    void sing(int songName);
  private:
    int _zero_position[FN];
    int _servo_position[FN];
    Adafruit_PWMServoDriver *pwm;
    U8G2_SSD1306_128X64_NONAME_1_HW_I2C *u8g2;
    //Mouth
    unsigned long int getAnimShape(int anim, int index);
    unsigned long int getMouthShape(int number);
};
#endif
