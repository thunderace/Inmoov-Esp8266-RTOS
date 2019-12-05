#include "mauriceServos.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SCN_LOOK_AROUND   0
#define SCN_LOOK_LEFT     1
#define SCN_LOOK_RIGHT    2
#define SCN_LOOK_FORWARD  3
#define SCN_SAY_NO        4

static xTaskHandle taskHandle = NULL;


static void neckRotationTask(void *headRotationScenario) {
  switch((int)headRotationScenario) {
    case SCN_LOOK_AROUND:
      servoGoTo(HEAD_ROTATION, 90, 20);
      servoGoTo(HEAD_ROTATION, 180, 20);
      servoGoTo(HEAD_ROTATION, 135, 20);
      break;
    case SCN_SAY_NO:
      servoGoTo(HEAD_ROTATION, 120, 5);
      servoGoTo(HEAD_ROTATION, 150, 5);
      servoGoTo(HEAD_ROTATION, 135, 5);
      break;
    case SCN_LOOK_LEFT:
      servoGoTo(HEAD_ROTATION, 45, 20);
      break;
    case SCN_LOOK_RIGHT:
      servoGoTo(HEAD_ROTATION, 215, 20);
      break;
    case SCN_LOOK_FORWARD:
      servoGoTo(HEAD_ROTATION, 135, 20);
      break;
    default:
      return;
  }
}


static void startTask(int scenario) {
  // kill old one if any
  if (taskHandle != NULL) {
    vTaskDelete(taskHandle);
  }
  // restart the same
  xTaskCreate(neckRotationTask, "neckRotationTask", 2048, (void *)scenario, 10, &taskHandle);
}

void lookRight() {
  startTask(SCN_LOOK_RIGHT);
}
void lookLeft() {
  startTask(SCN_LOOK_LEFT);
}

void lookForward() {
  startTask(SCN_LOOK_FORWARD);
}

void sayNo() {
  startTask(SCN_SAY_NO);
}

void lookAround() {
  startTask(SCN_LOOK_AROUND);
}
