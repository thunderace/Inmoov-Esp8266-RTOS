#include "mauriceServos.h"
#include "jaw.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define JAW_SCN_OPEN 0
#define JAW_SCN_CLOSE 1
#define JAW_SCN_TALK 2

static xTaskHandle taskHandle = NULL;


static void jawTask(void *scenario) {
  switch((int)scenario) {
    case JAW_SCN_OPEN:
      //servoGoTo(JAW, 0); // TODO
      break;
    case JAW_SCN_CLOSE:
      //servoGoTo(JAW, 180); // TODO
      break;
    case JAW_SCN_TALK:
    // servoGoTo(JAW, 150); //TODO
    //servoGoTo(JAW, 0); // TODO
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
  xTaskCreate(jawTask, "jawTask", 2048, (void *)scenario, 10, &taskHandle);
}

void openMouth() {
  startTask(JAW_SCN_OPEN);
}

void closeMouth() {
  startTask(JAW_SCN_CLOSE);
}

void talk() {
  startTask(JAW_SCN_TALK);
}




