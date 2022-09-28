/* Reseting a software timer */

/*  This example simulates the behavior of the backlight on a cell phone. The backlight:

    Turns on when a key is pressed.

    Remains on provided further keys are pressed within a certain time period.

    Automatically turns off if no key presses are made within a certain time period.
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termio.h>
#include "FreeRTOS.h"		
#include "task.h"			
#include "timers.h"
#include "queue.h"

#define ONE_SHOT_TIMER      pdMS_TO_TICKS(5000)

TimerHandle_t xBackLightTimer;
TaskHandle_t Task1;
uint8_t led = 0;

void vApplicationIdleHook(void);
static void prvBacklightTimerCallback(TimerHandle_t xTimer);
void vSenderTask1(void *pvParameters);

/* reads from keypress, doesn't echo */
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

/* reads from keypress, echoes */
int getch_echo(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

int main(void){
    xBackLightTimer = xTimerCreate("OneShotTimer", ONE_SHOT_TIMER, pdFALSE, 0, prvBacklightTimerCallback);
    xTimerStart(xBackLightTimer, portMAX_DELAY);
    xTaskCreate(vSenderTask1, "Sender1", 1000, NULL, 2, &Task1);

    vTaskStartScheduler();
    while (1);
}

static void prvBacklightTimerCallback(TimerHandle_t xTimer){
    TickType_t xTimeNow = xTaskGetTickCount();
    led = 0;
    printf("Timer expired, turning backlight OFF at time: %d and led %d\r\n", xTimeNow, led);
}

void vSenderTask1(void *pvParameters){
    const TickType_t xShortDelay = pdMS_TO_TICKS(50);
    TickType_t xTimeNow;
    printf("\r\nPress ENTER to turn backlight on\r\n");
    while (1)
    {
        if (getch() > 0){
            xTimeNow = xTaskGetTickCount();
            if (led == 0){
                led = 1;
                printf("Key pressed, turning backlight ON at time: %d and led: %d\r\n", xTimeNow, led);
            }
            else {
                printf("Key pressed, timer resetting at time: %d and led: %d\r\n", xTimeNow, led);
            }
            xTimerReset(xBackLightTimer, xShortDelay);
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

/* END OF FILE */