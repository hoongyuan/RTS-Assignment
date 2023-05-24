#include "FreeRTOS.h"
#include "StockManager.h"
#include <iostream>
#include "CSVReader.h"
#include "task.h"

void StockManager::addStock(const Stock& stock)
{
    // Map Stock with Price
    stocks[stock.getSymbol()] = stock;

}

void StockManager::loadStockData(const std::string& filename)
{
    // Create the CSVReader object and pass it to the task
    CSVReader csvReader(filename);

    cout << "Executing Task - csvReaderTask" << endl;
    // Create the task for CSV reading and pass the StockManager instance as the parameter
    /*xTaskCreate(csvReaderTask, "CSVReader", configMINIMAL_STACK_SIZE, this, 1, NULL);*/

}




