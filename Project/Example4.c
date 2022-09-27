// Using a queue to create a Mailbox

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"		
#include "task.h"			
#include "timers.h"
#include "queue.h"

typedef struct 
{
    TickType_t xTimeStamp;
    uint32_t value;
} Example_t;

QueueHandle_t xMailBox;
TaskHandle_t Task1, Task2;

void vApplicationIdleHook(void);

void vAFunction(void){
    xMailBox = xQueueCreate(1, sizeof(Example_t));
}

void vUpdateMailbox (uint32_t newValue){
    Example_t xData;
    xData.xTimeStamp = xTaskGetTickCount();
    xData.value = newValue;
    xQueueOverwrite(xMailBox, &xData);
}

BaseType_t vReadMailBox(Example_t *pxData){
    TickType_t xPreviousTimeStamp;
    BaseType_t xDataUpdated;
    xPreviousTimeStamp = pxData->xTimeStamp;
    xQueuePeek(xMailBox, pxData, portMAX_DELAY);
    if (pxData->xTimeStamp > xPreviousTimeStamp){
        xDataUpdated = pdTRUE;
    }
    else {
        xDataUpdated = pdFALSE;
    }
    // printf("a: %d\r\n", pxData->xTimeStamp);
    // printf("b: %d\r\n", xPreviousTimeStamp);

    return xDataUpdated;
}

void vSenderTask(void *pvParameters);
void vReceiverTask(void *pvParameters);


int main(void){
    xTaskCreate(vSenderTask, "Sender", 1000, NULL, 2, &Task1);
    xTaskCreate(vReceiverTask, "Receiver", 1000, NULL, 2, &Task2);
    vAFunction();

    vTaskStartScheduler();
    for(;;);
    return 0;
}

void vSenderTask(void *pvParameters){
    uint32_t count = 0;
    while (1)
    {
        vUpdateMailbox(count);
        count++;
        if (count > 10000) count = 0;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void vReceiverTask(void *pvParameters){
    Example_t xReceiveData;
    while (1)
    {
        if (vReadMailBox(&xReceiveData) == pdTRUE){
            printf("Value read from mailbox: %d\r\n", xReceiveData.value);
            printf("Time read from mailbox: %d\r\n", xReceiveData.xTimeStamp);
        }
    }
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