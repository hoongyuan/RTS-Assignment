#include "FreeRTOS.h"
#include <iostream>
#include "StockManager.h"
#include "CSVReader.h"
#include <Windows.h>
#include "SimulatedTime.h"
#include <iomanip>
#include "DisplayPanel.h"
#include "task.h"

//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void DisplayPanel::updateTime(int ts, const map<string, Stock> stocks) { //smt = simulated time, ts = timestamp
    while (simulatedTime < ts) {
        printStocks(stocks);
        vTaskDelay(pdMS_TO_TICKS(1000));// wait for 1 second
    }
}

void DisplayPanel::printStocks(const map<string,Stock> stocks) const{
    system("cls");

    //SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

    cout << R"(
  ____   ______    ____    ______   _   _
 / ___| |_    _| /  __  \ /  ____| | | / /
 | |__    |  |   | |  | | | |      | |/ /
 \___ \   |  |   | |  | | | |      |   |
  ___) |  |  |   | |  | | | |____  | |\ \
 |____/   |__|   \ ____ / \______| |_| \_\
                                                     
    )" << endl << endl;

    cout << "Time: " << simulatedTime << endl << endl;

    // Display headers
    cout << left << setw(20) << "Last Updated";
    cout << setw(20) << "Stock";
    cout << setw(20) << "Price";
    cout << setw(20) << "%Change" << right << endl;

    for (const auto& pair : stocks) {
        string key = pair.first;
        const Stock& obj = pair.second;

        cout << left << setw(20) << obj.getTimestamp();
        cout << setw(20) << key;
        //SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        cout << setw(20) << obj.getPrice();
        //SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
        cout << endl;
    }
    //cout << left << setw(20) << stock.getTimestamp();
    //cout << setw(20) << stock.getSymbol();
    ////SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    //cout << setw(20) << stock.getPrice();
    ////SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
    //cout << endl;

}



void DisplayPanel::printOrderStatus(User* user) {
    vector<Order*> orders = user->getOrders();
    for (const auto& order : orders) {
        cout << user->getUsername() << " has placed an order: BUY " << order->getStock() << " at " << order->getTargetPrice() << endl;
    }
}