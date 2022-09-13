// Using a queue set

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"		
#include "task.h"			
#include "timers.h"
#include "queue.h"

void vApplicationIdleHook(void);

static QueueHandle_t xQueue1 = NULL, xQueue2 = NULL;
static QueueSetHandle_t xQueueSet = NULL;


void vSenderTask1(void *pvParameters);
void vSenderTask2(void *pvParameters);

void vReceiverTask(void *pvParameters);


int main(void){
    xQueue1 = xQueueCreate(1, sizeof(char *));
    xQueue2 = xQueueCreate(1, sizeof(char *));

    xQueueSet = xQueueCreateSet(2);

    xQueueAddToSet(xQueue1, xQueueSet);
    xQueueAddToSet(xQueue2, xQueueSet);

    xTaskCreate(vSenderTask1, "Sender1", 1000, NULL, 1, NULL);
    xTaskCreate(vSenderTask2, "Sender2", 1000, NULL, 1, NULL);

    xTaskCreate(vReceiverTask, "Receiver", 1000, NULL, 2, NULL);
    
    vTaskStartScheduler();

    for (;;);
    return 0;
}

void vSenderTask1(void *pvParameters){
    const char * const message = "Message from Sender 1.\r\n";
    while (1){
        // vTaskDelay(100);
        xQueueSendToBack(xQueue1, &message , 0);
    }
}

void vSenderTask2(void *pvParameters){
    const char * const message = "Message from Sender 2.\r\n";
    while (1){
        // vTaskDelay(200);
        xQueueSendToBack(xQueue2, &message, 0);
    }
}

void vReceiverTask(void *pvParameters){
    QueueHandle_t xQueueThatContainsData;
    char *  messageReceived;
    while (1){
        xQueueThatContainsData = (QueueHandle_t) xQueueSelectFromSet(xQueueSet, pdMS_TO_TICKS(100));
        xQueueReceive(xQueueThatContainsData, &messageReceived, 0);
        printf("Received: %s", messageReceived);
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