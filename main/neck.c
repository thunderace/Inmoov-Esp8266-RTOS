#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "mauriceServos.h"
#include "neck.h"

#define NECK_SCN_UP 0
#define NECK_SCN_DOWN 1
#define NECK_SCN_LEFT 0
#define NECK_SCN_RIGHT 1

static xTaskHandle verticalTaskHandle = NULL;
static xTaskHandle horizontalTaskHandle = NULL;


static void neckVTask(void *scenario) {
  switch((int)scenario) {
    case NECK_SCN_UP:
      //servoGoTo(NECK_FRONT, 0); // TODO
      break;
    case NECK_SCN_DOWN:
      //servoGoTo(NECK_FRONT); // TODO
      break;
    default:
      return;
  }      
}

static void neckHTask(void *scenario) {
  switch((int)scenario) {
    // here i have to synchronize left and right servos : when the one turn clockwise, the other one must turn counterclockwise and vice versa
    // so don't use servoGoto here
    case NECK_SCN_LEFT:
      //dualServoGoTo(NECK_LEFT, NECK_RIGHT, 0); // TODO
      break;
    case NECK_SCN_RIGHT:
      //dualServoGoTo(NECK_LEFT, NECK_RIGHT, 180); // TODO
      break;
    default:
      return;
  }      
}


void startVTask(int scenario) {
  // kill old one if any
  if (verticalTaskHandle != NULL) {
    vTaskDelete(verticalTaskHandle);
  }
  // restart the same
  xTaskCreate(neckVTask, "", 2048, (void *)scenario, 10, &verticalTaskHandle);
}

void startHTask(int scenario) {
  // kill old one if any
  if (horizontalTaskHandle != NULL) {
    vTaskDelete(horizontalTaskHandle);
  }
  // restart the same
  xTaskCreate(neckHTask, "", 2048, (void *)scenario, 10, &horizontalTaskHandle);
}

void neckUp() {
  startVTask(NECK_SCN_UP);
}

void neckDown() {
  startVTask(NECK_SCN_DOWN);
}

void neckLeft() {
  startHTask(NECK_SCN_LEFT);
}

void neckRight() {
  startHTask(NECK_SCN_RIGHT);
}





