/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"		/* RTOS firmware */
#include "task.h"			/* Task */
#include "timers.h"
#include "queue.h"

QueueHandle_t xQueue;

void vSenderTask(void *pvParameters);
void vReceiverTask(void *pvParameters);

void vApplicationIdleHook(void);

int main(void){
    xQueue = xQueueCreate(5, sizeof(int32_t));
    if (xQueue != NULL){
        xTaskCreate(vSenderTask, "Sender1", 1000, (void *) 1000, 1, NULL);
        xTaskCreate(vSenderTask, "Sender2", 1000, (void *) 200, 1, NULL);
        xTaskCreate(vReceiverTask, "Receiver", 1000, NULL, 2, NULL); 
        vTaskStartScheduler();
    }
    else {
        printf("The queue could not be created");
    }

    for(;;);
}

void vSenderTask(void *pvParameters){
    int32_t ValueToSend;
    BaseType_t xStatus;
    ValueToSend = (int32_t ) pvParameters;
    while (1)
    {
        xStatus = xQueueSendToBack(xQueue, &ValueToSend, 0);
        if (xStatus != pdPASS){
            printf("Sender can't send");
        }
    }
}

void vReceiverTask(void *pvParameters){
    int32_t ReceiveValue;
    BaseType_t xStatus;
    const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
    while (1){
        if (uxQueueMessagesWaiting(xQueue) != 0){
            printf("abcx");
        }
        xStatus = xQueueReceive(xQueue, &ReceiveValue, xTicksToWait);
        if (xStatus == pdPASS){
            printf("Receive = %d\r\n", ReceiveValue);
        }
        else {
            printf("Could not receive from the queue\r\n");
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