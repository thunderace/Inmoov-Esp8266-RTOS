#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <pca9685.h>
#include <string.h>
#include "mauriceServos.h"

#define ADDR PCA9685_ADDR_BASE
#if defined(CONFIG_IDF_TARGET_ESP8266)
#define SDA_GPIO 4
#define SCL_GPIO 5
#else
#define SDA_GPIO 16
#define SCL_GPIO 17
#endif


#define TYPE_NONE 0
#define TYPE_SG90 1
#define TYPE_PDI_6221MG_360 2
#define TYPE_ALEXP_30KG 3
#define TYPE_996R 4
#define TYPE_995 5
#define TYPE_RDS3235  6
#define TYPE_PDI_6221MG_270 7
// servo 0 : (right eye V : // M : 455 - full up : 330 - full down : 600
// servo 5 : (left eye V : // M : 455 - full up : 330 - full down : 600
int servoDefaultPosition[16] = {
#ifdef ASSEMBLY_POSITIONS
  180,  // right eye full up
  90,   // right eye H center
  0,  // top lid full open
  0,    // bottom lid full open : TBD
  90,   // left eye H center
  0,    // left eye full up
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0
#else
  90,   // right eye V center : TBD
  90,   // right eye H center
  90,   // top lid full closed : TBD
  90,   // bottom lid full closed : TBD
  90,   // left eye H center
  90,   // left eye V center : TBD
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0
#endif  
};
long map(long x, long in_min, long in_max, long out_min, long out_max) {
    long divisor = (in_max - in_min);
    if(divisor == 0){
        return -1; //AVR returns -1, SAM returns 0
    }
    return (x - in_min) * (out_max - out_min) / divisor + out_min;
}  
int servoType[16] = {
  TYPE_SG90, // M : 455 - full up : 330 - full down : 600
  TYPE_SG90,  // M : 470 - full up : 600 - full down : 330
  TYPE_SG90, 
  TYPE_SG90, 
  TYPE_NONE,
  TYPE_NONE,
  TYPE_NONE,
  TYPE_NONE,
  TYPE_NONE,
  TYPE_ALEXP_30KG, // shoulder left
  TYPE_ALEXP_30KG, // shoulder right
  TYPE_PDI_6221MG_270, // neck front
  TYPE_PDI_6221MG_270,  // neck left
  TYPE_PDI_6221MG_270,  // neck right
  TYPE_996R, // jaw
  TYPE_RDS3235 //head rotation 270
};
                    
//#define ANGLE
#define MIN_POS 1
#define MIDDLE_POS 2
#define MAX_POS 3
int initialServoPosition[16] = { 
  MIDDLE_POS, 
  MIDDLE_POS, 
  MIDDLE_POS, 
  MIDDLE_POS, 
  0,
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  0, 
  MIN_POS, 
  MIDDLE_POS};

int servoPosition[16] = { 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int nextPosition[16] = { 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

#define SERVO_MIN_IDX 0
#define SERVO_MAX_IDX 1
#define SERVO_MIDDLE_IDX 2
#define SERVO_MIN_ANGLE_IDX 3
#define SERVO_MAX_ANGLE_IDX 4

int servoData[8][5] = {
  {0, 0, 0, 0, 0},
  {300, 600, 450, 0, 180}, // SG90
  {300, 600, -1, -1, -1}, // PDI 360
  {300, 600, 450, 0, 180}, // 30KG
  {100, 570, 335, 0, 180}, // 996R
  {300, 600, 450, 0, 180}, // 995
  {125, 660, 392, 0, 270}, // RDS3235
  {300, 600, 450, 0, 270}  // PDI 270
};


void dualServoGoTo(int servo1, int servo2, int angle, int speed) {
  int cur1Pos = servoPosition[servo1]; // in  value
  int cur2Pos = servoPosition[servo2]; // in  value
  int next1Pos = map(angle, servoData[servoType[servo1]][SERVO_MIN_ANGLE_IDX], servoData[servoType[servo1]][SERVO_MAX_ANGLE_IDX], servoData[servoType[servo1]][SERVO_MIN_IDX], servoData[servoType[servo1]][SERVO_MAX_IDX]);
  int next2Pos = map(angle, servoData[servoType[servo2]][SERVO_MIN_ANGLE_IDX], servoData[servoType[servo2]][SERVO_MAX_ANGLE_IDX], servoData[servoType[servo2]][SERVO_MIN_IDX], servoData[servoType[servo2]][SERVO_MAX_IDX]);
  printf("Go from : %d to %d\n", cur1Pos, next1Pos);

  if (cur1Pos > next1Pos) {
    for (int i = cur1Pos; i >= next1Pos; i--) {
      setValue(servo1, i);
      setValue(servo2, cur2Pos + i - cur1Pos);
      vTaskDelay(speed / portTICK_PERIOD_MS);
    }
  } 
  if (cur1Pos < next1Pos) {
    for (int i = cur1Pos; i <= next1Pos; i++) {
      setValue(servo1, i);
      setValue(servo2, cur2Pos - i - cur1Pos);
      vTaskDelay(speed / portTICK_PERIOD_MS);
    }
  }
}

void servoGoTo(int servo, int angle, int speed) {
  int curPos = servoPosition[servo]; // in  value
  int nextPos = map(angle, servoData[servoType[servo]][SERVO_MIN_ANGLE_IDX], servoData[servoType[servo]][SERVO_MAX_ANGLE_IDX], servoData[servoType[servo]][SERVO_MIN_IDX], servoData[servoType[servo]][SERVO_MAX_IDX]);

  printf("Go from : %d to %d\n", curPos, nextPos);

  if (curPos > nextPos) {
    for (int i = curPos; i >= nextPos; i--) {
      setValue(servo, i);
      vTaskDelay(speed / portTICK_PERIOD_MS);
    }
  } 
  if (curPos < nextPos) {
    for (int i = curPos; i <= nextPos; i++) {
      setValue(servo, i);
      vTaskDelay(speed / portTICK_PERIOD_MS);
    }
  }
}

static i2c_dev_t dev;

void servoSetup() {
  memset(&dev, 0, sizeof(i2c_dev_t));

  ESP_ERROR_CHECK(pca9685_init_desc(&dev, ADDR, 0, SDA_GPIO, SCL_GPIO));
  ESP_ERROR_CHECK(pca9685_init(&dev));

  ESP_ERROR_CHECK(pca9685_restart(&dev));

  uint16_t freq;
  ESP_ERROR_CHECK(pca9685_set_pwm_frequency(&dev, 60));
  ESP_ERROR_CHECK(pca9685_get_pwm_frequency(&dev, &freq));
  printf("Freq 1000Hz, real %d\n", freq);

  for (int i = 0; i < 16; i++) {
    switch (initialServoPosition[i]) {
      case MIDDLE_POS:
        setMiddle(i);
        nextPosition[i] = getMiddle(i);
        break;
      case MAX_POS:
        nextPosition[i] = getMax(i);
        setMax(i);
        break;
      case MIN_POS:
        nextPosition[i] = getMin(i);
        setMin(i);
        break;
      default:
        //setValue(i, initialServoPosition[i]); 
        break;
      }
  }
}

void setMiddle(int servo) {
  if (servoType[servo] == TYPE_PDI_6221MG_360) {
    servoPosition[servo] = 395;
  } else {
    servoPosition[servo] = servoData[servoType[servo]][SERVO_MIN_IDX] + ((servoData[servoType[servo]][SERVO_MAX_IDX] - servoData[servoType[servo]][SERVO_MIN_IDX])/2);
  }
  setValue(servo, servoPosition[servo]);
}

int getMiddle(int servo) {
    return servoData[servoType[servo]][SERVO_MIN_IDX] + ((servoData[servoType[servo]][SERVO_MAX_IDX] - servoData[servoType[servo]][SERVO_MIN_IDX])/2);
}

void setMin(int servo) {
  servoPosition[servo] = servoData[servoType[servo]][SERVO_MIN_IDX];
  setValue(servo, servoPosition[servo]);
}

int getMin(int servo) {
  return servoData[servoType[servo]][SERVO_MIN_IDX];
}

void setMax(int servo) {
  servoPosition[servo] = servoData[servoType[servo]][SERVO_MAX_IDX];
  setValue(servo, servoPosition[servo]);
}

int getMax(int servo) {
  return servoData[servoType[servo]][SERVO_MAX_IDX];
}

void setValue(int servo, int value) {
  servoPosition[servo] = value;
  pca9685_set_pwm_value(&dev, servo, value);
}

#if 0
void goTo(int servo, int angle){
  //int curPos = servoPosition[servo]; // in  value
  nextPosition[servo] = map(angle, 0, 180, servoData[servoType[servo]][SERVO_MIN_IDX], servoData[servoType[servo]][SERVO_MAX_IDX]);
  servoThreadIdx = servo;
}
#endif
