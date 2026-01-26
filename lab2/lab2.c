/**
 * @file    lab2.c
 * @brief   Starting point for WSU (Vancouver) cs466 Embedded Systems Lab2
 * @author  Miller Lowe
 * @date    2025-03-12
 *
 * This file is the starting point for Lab2.  It holds some very basic 
 * functionality but needs to be extended to complete the lab
 * 
 * As written this program will flash the LED at different rates when pressing
 * the SW1 button that we used in Lab1.  
 * 
 * Read the Lab-2 instructions to see the functionality changes to complete the lab.
 */
#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>
//#include <semphr.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"

const uint8_t LED_PIN = 25;
const uint8_t SW1_PIN = 17;

uint32_t heartbeatDelay = 500;  // poorly designed global shared between ISR and task code.

const uint32_t minPriority = 1;

//static SemaphoreHandle_t _semBtnSW1 = NULL;


/**
 * @brief   Interrupt Service Routine that should run when SW1 Pressed
 * @param   gpio: which GPIO caised the interrupt if more than a single one enabled
 * @param   events_unused 
 */
void gpio_int_callback(uint gpio, uint32_t events_unused) 
{
    // Note: It's a really bad habit to rely on print statements in an ISR
    printf("sw1_callback: GPIO ISR %u\n", gpio);

    if (gpio == SW1_PIN)
    {
        heartbeatDelay /= 2;
        //xSemaphoreGiveFromISR(_semBtnSW1, NULL);
    }

    if (heartbeatDelay < 50)
        heartbeatDelay = 500;
}

/**
 * @brief   Initalize hardware and register one-time registrations
 */
void hardware_init(void)
{
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(SW1_PIN);
    gpio_pull_up(SW1_PIN);
    gpio_set_dir(SW1_PIN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(SW1_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_int_callback);
}

#if 0
// Note the [#if 0 / #endif] pair is a great way to omit a large code segment.
/**
 * @brief   Task that is designed to react to the SW1 Semaphore available
 *          otherwise the task will happily wait forever but not consunme 
 *           processor resources.
 * @param   notUsed: Required for task but not used in this function
 */
void sw1_handler(void * notUsed)
{
    while (true)
    {
        xSemaphoreTake( _semBtnSW1, portMAX_DELAY);
        printf("sw1 Semaphore taken..\n");
    }
}
#endif

/**
 * @brief   Task that is designed to run as the minimum priority task 
 *          in the system.  It's primary task in this program is to slowly 
 *          blink the LED which will give the developer a visual indication
 *          of task health.  
 * @param   notUsed: Required for task but not used in this function
 */
void heartbeat(void * notUsed)
{   
    while (true) {
        printf("hb-tick: %d\n", heartbeatDelay);
        gpio_put(LED_PIN, 1);
        vTaskDelay(heartbeatDelay);
        gpio_put(LED_PIN, 0);
        vTaskDelay(heartbeatDelay);
    }
}

/**
 * @brief   Main entry point. The developer should strive to keep this function small.
 *          In thiis case performing...
 *            - setup hardware
 *            - declare tasks
 *            - start RTOS Scheduler  
 * @param   notUsed: Required for task but not used in this function
 */
int main(void)
{
    stdio_init_all();
    printf("lab2 Hello!\n");
    hardware_init();

    //_semBtnSW1 = xSemaphoreCreateBinary();

    xTaskCreate(heartbeat, "heartbeatTask", 256, NULL, minPriority, NULL);
    //xTaskCreate(sw1_handler, "SW1_Task", 256, NULL, minPriority+1, NULL);

    vTaskStartScheduler();

    // NOTE: The vTaskStartScheduler() function above should never return.  If execution 
    //       gets here it is an error.
    while(1){};
}
