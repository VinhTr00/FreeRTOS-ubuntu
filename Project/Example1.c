// Using a queue

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"		
#include "task.h"			
#include "timers.h"
#include "queue.h"

QueueHandle_t xQueue;

void vSenderTask(void *pvParameters);
void vReceiverTask(void *pvParameters);
void vApplicationIdleHook(void);

int main(void){
    xQueue = xQueueCreate(100, sizeof(int32_t));
    if (xQueue != NULL){
        xTaskCreate(vSenderTask, "Sender1", 1000, (void *) 100, 1, NULL);
        xTaskCreate(vSenderTask, "Sender2", 1000, (void *) 200, 1, NULL);

        xTaskCreate(vReceiverTask, "Receiver", 1000, NULL, 2, NULL);

        vTaskStartScheduler();
    }
    else {
        printf("The queue could not be created.\r\n");
    }

    for(;;);
}

void vSenderTask(void *pvParameters){
    int32_t ValueToSend;
    BaseType_t xStatus;
    const TickType_t xTicksToWait = pdMS_TO_TICKS(1000);
    ValueToSend = (int32_t) pvParameters;
    while (1)
    {
        for (int32_t i = 0; i < 100; i++){
            xStatus = xQueueSendToFront(xQueue, &i, 0);
            if (xStatus != pdPASS){
                printf("Could not send to the queue.\r\n");
            }
        }
    }
}

void vReceiverTask(void *pvParameters){
    int32_t ReceiveValue;
    BaseType_t xStatus;
    const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
    while (1){
        if (uxQueueSpacesAvailable(xQueue) == 0){
            printf("Queue should have been empty!\r\n");
        }
        xStatus = xQueueReceive(xQueue, &ReceiveValue, xTicksToWait);
        if (xStatus == pdPASS){
            printf("Receive = %d\r\n", ReceiveValue);
        }
        else {
            printf("Could not receive from the queue.\r\n");
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