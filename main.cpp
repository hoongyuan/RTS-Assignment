#include "FreeRTOS.h"
#include "task.h"

// Include header files for your implemented classes
#include "Stock.h"
#include "CSVReader.h"
#include "StockManager.h"
#include "SimulatedTime.h"
#include "DisplayPanel.h"

// Define the simulatedTime variable
void incrementSimulatedTimeTask(void* pvParameters)
{
    while (true) {
        simulatedTime++;  // Increment the simulated time
        vTaskDelay(pdMS_TO_TICKS(1000));  // 1 second delay
    }
}


int main() {

    StockManager stockManager;
    //stockManager.loadStockData("stocks.txt");
    DisplayPanel displayPanel;
    stockManager.setupDisplayPanel(displayPanel);

    //Create file reader task
    xTaskCreate(csvReaderTask, "CSVReader", configMINIMAL_STACK_SIZE, &stockManager, 1, NULL);

    //Create time simulator task
    xTaskCreate(incrementSimulatedTimeTask, "SimulatedTime", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();

    // Use the stockManager and stocks vector as needed

    return 0;
}
