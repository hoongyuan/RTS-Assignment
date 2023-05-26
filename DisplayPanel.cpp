#include "FreeRTOS.h"
#include <iostream>
#include "StockManager.h"
#include "CSVReader.h"
#include <Windows.h>
#include "SimulatedTime.h"
#include <iomanip>
#include "DisplayPanel.h"
#include "task.h"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
QueueHandle_t orderQueue = xQueueCreate(10, 100); //10 queue, 100 orders
QueueHandle_t stockQueue = xQueueCreate(10, 50); //10 queue, 50 stocks
vector<Stock> stockPanel;

string DisplayPanel::stockMapToString(const Stock stocks) {
    string result;
    Stock stock = stocks;

    result = to_string(stock.getTimestamp()) + "," + stock.getSymbol() + "," + to_string(stock.getPrice());

    // Remove the trailing comma
    if (!result.empty()) {
        result.pop_back();
    }

    return result;
}

void DisplayPanel::updateTime(int ts, const map<string, Stock> stocks) { //smt = simulated time, ts = timestamp

    while (simulatedTime < ts) {
        printStocks(stocks);
        vTaskDelay(pdMS_TO_TICKS(1000));// wait for 1 second
    }
}

void DisplayPanel::printStocks(const map<string,Stock> stocks) const{
    //system("cls");

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

void DisplayPanel::updateStock(const map<string, Stock> stocks) {
    
        const int size = sizeof(stocks);
        string stockArr[size];
        int i = 0;

        for (const auto& stock : stocks) {
            string stringStock = stockMapToString(stock.second);
            stockArr[i++] = stringStock;
        }

        // Allocate memory for the dynamic array
        string* stockDataArray = new string[size];
        copy(stockArr, stockArr + size, stockDataArray);

        xQueueSend(stockQueue, &stockDataArray, 0);
}

void DisplayPanel::updateOrderStatus(vector<User*> users) {

    // Send latest user order list to displaypanel
    xQueueSend(orderQueue, &users, 0);

    /*for (const auto& user : users) {
        vector<Order*> orders = user->getOrders();
        for (const auto& order : orders) {
            cout << user->getUsername() << " has placed an order: BUY " << order->getStock() << " at " << order->getTargetPrice() << endl;
            cout << endl;
        }
    }*/
}

void readStockArray(string* stockReceived) {
    Stock stock;
    string timestamp;
    string stockname;
    string price;
    for (int i = 0; !stockReceived[i].empty(); i++) {
        istringstream iss(stockReceived[i]);
        getline(iss, timestamp, ',');
        getline(iss, stockname, ',');
        getline(iss, price, ',');
        stock = Stock(stockname, stod(price), stoi(timestamp));

        stockPanel.push_back(stock);
    }
}

void printAll(void* pvParameters) {
    
    DisplayPanel* displaypanel = static_cast<DisplayPanel*>(pvParameters);

    string timestamp;
    string stockname;
    string price;
    bool refresh = false;
    bool receive = true;

    while (true) {
        refresh = false;
        receive = true;
        system("cls");
        cout << R"(
  ____   ______    ____    ______   _   _
 / ___| |_    _| /  __  \ /  ____| | | / /
 | |__    |  |   | |  | | | |      | |/ /
 \___ \   |  |   | |  | | | |      |   |
  ___) |  |  |   | |  | | | |____  | |\ \
 |____/   |__|   \ ____ / \______| |_| \_\
                                                     
    )" << endl << endl;

        //print time
        cout << "Time: " << simulatedTime << endl << endl;

        //print orders
        vector<User*> orderListReceived;
        cout << "Orders: " << endl;
        cout << "==============================================" << endl;
        xQueueReceive(orderQueue, &orderListReceived, 0);

        if (!orderListReceived.empty()) {
            for (const auto& user : orderListReceived) {
                for (const auto& order : user->getOrders()) {
                    cout << user->getUsername() << " has placed an order: BUY " << order->getStock() << " at " << order->getTargetPrice() << endl;
                    cout << endl;
                }
            }
        }
        else {
            cout << "There are no order!" << endl;
        }
        cout << "==============================================" << endl;

        //print stocks
        // Display headers
        string* stockReceived = nullptr;

        cout << left << setw(20) << "Last Updated";
        cout << setw(20) << "Stock";
        cout << setw(20) << "Price";
        cout << setw(20) << "%Change" << right << endl;
        
        if (xQueueReceive(stockQueue, &stockReceived, 0) == pdFALSE) {
            receive = false;
        }
        else {
            stockPanel.clear();
            readStockArray(stockReceived);
        }

        for (int i = 0; i < stockPanel.size(); i++) {
            cout << left << setw(20) << stockPanel[i].getTimestamp();
            cout << setw(20) << stockPanel[i].getSymbol();
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            cout << setw(20) << stockPanel[i].getPrice();
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
            cout << endl;
        }
        /*for (size_t i = 0; !stockReceived[i].empty(); i++) {

            istringstream iss(stockReceived[i]);                
            getline(iss, timestamp, ',');
            getline(iss, stockname, ',');
            getline(iss, price, ',');

            cout << left << setw(20) << timestamp;
            cout << setw(20) << stockname;
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            cout << setw(20) << price;
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
            cout << endl;
        }*/

        if (xQueueReceive(stockQueue, &stockReceived, 0) == pdFALSE) {
            refresh = false;
        }
        else {
            refresh = true;
        }

        delete[] stockReceived;

        if (!refresh) {
            vTaskDelay(pdMS_TO_TICKS(1000));// wait for 1 second
        }
    }
}


