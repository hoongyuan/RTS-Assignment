#include "FreeRTOS.h"
#include "StockManager.h"
#include <iostream>
#include "CSVReader.h"
#include "task.h"
#include <iomanip>
#include "SimulatedTime.h"

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

void StockManager::displayData()const {
    system("cls");

    cout << R"(
  ____   ______    ____    ______   _   _
 / ___| |_    _| /  __  \ /  ____| | | / /
 | |__    |  |   | |  | | | |      | |/ /
 \___ \   |  |   | |  | | | |      |   |
  ___) |  |  |   | |  | | | |____  | |\ \
 |____/   |__|   \ ____ / \______| |_| \_\
                                                     
    )" << endl;

    cout << "Time: " << simulatedTime << endl;

    // Display headers
    cout << left << setw(20) << "Last Updated"
        << setw(20) << "Stock"
        << setw(20) << "Price"
        << setw(20) << "%Change" << right << endl;

    for (const auto& pair : stocks) {
        string key = pair.first;
        const Stock &obj = pair.second;

        cout << left << setw(20) << obj.getTimestamp()
            << setw(20) << key
            << setw(20) << obj.getPrice() << endl;
    }
}




