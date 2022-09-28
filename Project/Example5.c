/* Creating one-shot and auto-reload timers */

#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"		
#include "task.h"			
#include "timers.h"
#include "queue.h"

#define ONE_SHOT_TIMER      pdMS_TO_TICKS(3333)
#define AUTO_RELOAD_TIMER   pdMS_TO_TICKS(500)

TimerHandle_t xAutoReloadTimer, xOneShotTimer;
BaseType_t xTimer1, xTimer2; 

static void OneShotTimerCallBack(TimerHandle_t xTimer);
static void AutoReloadCallBack(TimerHandle_t xTimer);

int main(void){

    xOneShotTimer = xTimerCreate("OneShotTimer", ONE_SHOT_TIMER, pdFALSE, 0, OneShotTimerCallBack);

    xAutoReloadTimer = xTimerCreate("AutoReloadTimer", AUTO_RELOAD_TIMER, pdTRUE, 0, AutoReloadCallBack);

    if ((xOneShotTimer != NULL) && (xAutoReloadTimer != NULL)){

        xTimer1 = xTimerStart(xOneShotTimer, portMAX_DELAY);

        xTimer2 = xTimerStart(xAutoReloadTimer, portMAX_DELAY);

        if ((xTimer1 == pdPASS) && (xTimer2 == pdPASS)){

            vTaskStartScheduler();
        }

    }
    for(;;);
}

static void OneShotTimerCallBack(TimerHandle_t xTimer){
    TickType_t xTimeNow;

    xTimeNow = xTaskGetTickCount();

    printf("One-shot timer callback executing: %d\r\n", xTimeNow);
}

static void AutoReloadCallBack(TimerHandle_t xTimer){
    TickType_t xTimeNow;

    xTimeNow = xTaskGetTickCount();

    printf("Auto-reload timer callback executing: %d\r\n", xTimeNow);
}

void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
    taskENTER_CRITICAL();
    {
        printf("[ASSERT] %s:%lu\n", pcFileName, ulLine);
        fflush(stdout);
    }
    taskEXIT_CRITICAL();
    exit(-1);
}

void vApplicationIdleHook(void)
{
}