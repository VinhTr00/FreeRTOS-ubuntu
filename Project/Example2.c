// Sending structures

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"		
#include "task.h"			
#include "timers.h"
#include "queue.h"

void vApplicationIdleHook(void);

QueueHandle_t xQueue;

typedef enum{
    eSender1,
    eSender2
} DataSource_t;

typedef struct 
{
    uint8_t ucValue;
    DataSource_t eDataSource;
} Data_t;

static const Data_t xStructsToSend[2] = {
    {100, eSender1},
    {200, eSender2}
};


void vSenderTask(void *pvParameters);
void vReceiverTask(void *pvParameters);


int main(void){
    xQueue = xQueueCreate(100, sizeof(Data_t));
    if (xQueue != NULL){
        xTaskCreate(vSenderTask, "Sender1", 1000, &xStructsToSend[0], 2, NULL);
        xTaskCreate(vSenderTask, "Sender2", 1000, &xStructsToSend[1], 2, NULL);

        xTaskCreate(vReceiverTask, "Receiver", 1000, NULL, 1, NULL);
        
        vTaskStartScheduler();
    }
    else {
        printf("The queue could not be created.\r\n");
    }

    for(;;);
}

void vSenderTask(void *pvParameters){
    BaseType_t xStatus;
    const TickType_t xTicksToWait = pdMS_TO_TICKS(1000);
    while (1){
        xStatus = xQueueSendToBack(xQueue, pvParameters , xTicksToWait);
        if (xStatus != pdPASS){
            printf("Could not send to the queue.\r\n");
        }
    }
}

void vReceiverTask(void *pvParameters){
    BaseType_t xStatus;
    Data_t xReceivedStructure;
    const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
    while (1){
        if (uxQueueSpacesAvailable(xQueue) != 0){
            printf("Queue should have been full!\r\n");
        }
        xStatus = xQueueReceive(xQueue, &xReceivedStructure, 0);
        if (xStatus == pdPASS){
            if (xReceivedStructure.eDataSource == eSender1){
                printf("From Sender 1: %d\r\n", xReceivedStructure.ucValue);
            }
            else {                  
                printf("From Sender 2: %d\r\n", xReceivedStructure.ucValue);
            }
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