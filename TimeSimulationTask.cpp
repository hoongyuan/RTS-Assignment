#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>
#include <cstdint>

// Define the starting time for the simulated system
#define START_TIME_SECONDS 1624567890 // Example: June 24, 2021, 15:31:30

// Define the tick rate in Hz (ticks per second)
#define TICK_RATE_HZ 1000

// Define the time increment per tick (in seconds)
#define TIME_INCREMENT_SECONDS (1.0 / TICK_RATE_HZ)

// Define the stock data timestamp array (example values)
uint32_t stockDataTimestamps[] = { 1624567890, 1624567895, 1624567900, 1624567905 };

// Task function to simulate time and check for matching timestamps
void timeSimulationTask(void* pvParameters)
{
    TickType_t lastWakeTime = xTaskGetTickCount();

    // Initialize the simulated time to the starting time
    uint32_t simulatedTime = START_TIME_SECONDS;

    while (1)
    {
        // Check if the simulated time matches any of the stock data timestamps
        for (size_t i = 0; i < sizeof(stockDataTimestamps) / sizeof(stockDataTimestamps[0]); ++i)
        {
            if (simulatedTime == stockDataTimestamps[i])
            {
                // Display the stock data timestamp or perform desired actions
                printf("Matching timestamp found: %u\n", stockDataTimestamps[i]);
            }
        }

        // Increment the simulated time by the time increment per tick
        simulatedTime += TIME_INCREMENT_SECONDS;

        // Delay the task to maintain the desired tick rate
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1000 / TICK_RATE_HZ));
    }
}

void setup()
{
    // Create the time simulation task
    xTaskCreate(timeSimulationTask, "TimeSimulationTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();
}

void loop()
{
    // This loop is not used as the scheduler starts and takes over the execution of tasks
}
