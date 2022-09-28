/* Using a queue set */

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"		
#include "task.h"			
#include "timers.h"
#include "queue.h"

#define queueLength 1
#define sumQueueLength ( queueLength*2 )

void vApplicationIdleHook(void);

static QueueHandle_t xQueue1 = NULL, xQueue2 = NULL;
static QueueSetHandle_t xQueueSet = NULL;

TaskHandle_t Task1, Task2, ReceiveTask;

void vSenderTask1(void *pvParameters);
void vSenderTask2(void *pvParameters);

void vReceiverTask(void *pvParameters);


int main(void){
    xQueue1 = xQueueCreate(queueLength, sizeof(char *));
    xQueue2 = xQueueCreate(queueLength, sizeof(uint32_t));

    xQueueSet = xQueueCreateSet(sumQueueLength);

    xQueueAddToSet(xQueue1, xQueueSet);
    xQueueAddToSet(xQueue2, xQueueSet);

    xTaskCreate(vSenderTask1, "Sender1", 1000, NULL, 2, &Task1);
    xTaskCreate(vSenderTask2, "Sender2", 1000, NULL, 2, &Task2);

    xTaskCreate(vReceiverTask, "Receiver", 1000, NULL, 3, &ReceiveTask);
    
    vTaskStartScheduler();

    for (;;);
}

void vSenderTask1(void *pvParameters){
    const char * message = "Message from Sender 1.\r\n";
    while (1){
        xQueueSendToBack(xQueue1, &message , pdMS_TO_TICKS(100));
        // printf("Task 2 piority: %ld\r\n", uxTaskPriorityGet(Task2));
    }
}

void vSenderTask2(void *pvParameters){
    static uint32_t i = 0;
    while (1){
        xQueueSendToBack(xQueue2, &i, pdMS_TO_TICKS(100));
        i++;
    }
}

void vReceiverTask(void *pvParameters){
    QueueSetMemberHandle_t xHandle;
    char *  messageReceived;
    uint32_t valueReceived;
    while (1){
        xHandle = xQueueSelectFromSet(xQueueSet, pdMS_TO_TICKS(100));
        if (xHandle == NULL){
            printf("Error\r\n");
        }
        else if (xHandle == (QueueSetMemberHandle_t) xQueue2)
        {
            xQueueReceive(xQueue2, &valueReceived, 0);
            printf("Received: %d\r\n", valueReceived);
        }
        else if (xHandle == (QueueSetMemberHandle_t) xQueue1)
        {
            xQueueReceive(xQueue1, &messageReceived, 0);
            printf("Received: %s\r\n", messageReceived);
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