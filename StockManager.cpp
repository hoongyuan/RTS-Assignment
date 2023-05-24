#include "FreeRTOS.h"
#include "StockManager.h"
#include <iostream>
#include "CSVReader.h"
#include "task.h"
#include <iomanip>
#include "SimulatedTime.h"
#include <Windows.h>

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

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

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

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
    cout << left << setw(20) << "Last Updated";
    cout << setw(20) << "Stock";
    cout << setw(20) << "Price";
    cout << setw(20) << "%Change" << right << endl;

    for (const auto& pair : stocks) {
        string key = pair.first;
        const Stock &obj = pair.second;

        cout << left << setw(20) << obj.getTimestamp();
        cout << setw(20) << key;
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        cout << setw(20) << obj.getPrice();
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
        cout << endl;
        /*cout << setw(20) << "%Change" << right << endl;

        cout << left << setw(20) << obj.getTimestamp()
            << setw(20) << key
            << setw(20) << obj.getPrice() << endl;*/
    }
}




