#include "FreeRTOS.h"
#include "task.h"

// Include header files for your implemented classes
#include "Stock.h"
#include "CSVReader.h"
#include "StockManager.h"
#include "SimulatedTime.h"

// Define the simulatedTime variable


int main() {
    StockManager stockManager;
    stockManager.loadStockData("stocks.txt");
    xTaskCreate(csvReaderTask, "CSVReader", configMINIMAL_STACK_SIZE, &stockManager, 1, NULL);




    vTaskStartScheduler();

    // Use the stockManager and stocks vector as needed

    return 0;
}
