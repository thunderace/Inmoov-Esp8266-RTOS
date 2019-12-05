// head
#define EYE_BALL_RIGHT_VERT  0
#define EYE_BALL_RIGHT_HORI 1
#define EYE_BALL_LEFT_HORI  2
#define EYE_BALL_LEFT_VERT  3
//#define EYE_LID_UP      X
//#define EYE_LID_DOWN    X
#define NECK_FRONT  11
#ifdef NEW_NECK
#define NECK_LEFT  12
#define NECK_RIGHT  13
#endif
#define JAW  14
#define HEAD_ROTATION 15


void servoSetup();
void setMiddle(int servo);
int getMiddle(int servo);
void setMin(int servo);
int getMin(int servo);
void setMax(int servo);
int getMax(int servo);
void setValue(int servo, int value);
void servoGoTo(int servo, int angle, int speed);
void dualServoGoTo(int servo1, int servo2, int angle, int speed);