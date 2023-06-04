#include "FreeRTOS.h"
#include "task.h"

// Include header files for your implemented classes
#include "Stock.h"
#include "CSVReader.h"
#include "StockManager.h"
#include "SimulatedTime.h"
#include "DisplayPanel.h"
#include "OrderManager.h"


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
    OrderManager orderManager;
    //stockManager.loadStockData("stocks.txt");
    DisplayPanel displayPanel;
    stockManager.setupDisplayPanel(displayPanel);

    //Create time simulator task
    xTaskCreate(incrementSimulatedTimeTask, "SimulatedTime", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    //Create Read Stock task
    xTaskCreate(csvReaderTask, "CSVReadStock", configMINIMAL_STACK_SIZE, &stockManager, 1, NULL);

    //Create Read Order task
    xTaskCreate(csvReadOrder, "CSVReadOrder", configMINIMAL_STACK_SIZE, &orderManager, 1, NULL);

    //Create Manage Order task
    xTaskCreate(manageOrder, "ManageOrder", configMINIMAL_STACK_SIZE, &orderManager, 1, NULL);

    //Create Parse Stock task
    xTaskCreate(csvParseStock, "CSVParseStock", configMINIMAL_STACK_SIZE, &stockManager, 1, NULL);

    //Create display panel task
    xTaskCreate(printAll, "DisplayPanelStart", configMINIMAL_STACK_SIZE, &displayPanel, 1, NULL);

    //Create Update Profit task
    xTaskCreate(updateProfit, "updateOrderProfit", configMINIMAL_STACK_SIZE, &displayPanel, 1, NULL);

    vTaskStartScheduler();

    return 0;
}
