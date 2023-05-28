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
vector<string> orderPanel;
bool refresh = false;
bool receiveOrder = false;
bool receiveStock = false;

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

// Convert the data from map to string and store in array
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

        // Send data to display panel
        xQueueSend(stockQueue, &stockDataArray, 0);
}

void DisplayPanel::updateOrderStatus(const Stock receivedStock,vector<User*> users) {

    const int size = sizeof(users);
    string orderArr[size];
    int i = 0;

    // Convert the data from vector to string and store in array
    for (User* user : users) {
        vector<Order*> orders = user->getOrders();

        for (Order* order : orders) {
            string stringOrder = 
                user->getUsername() + "," + 
                order->getStock() + "," + 
                order->getOrderType() + "," + 
                to_string(order->getTargetPrice());

            if (order->getOrderStatus() == true) {
                order->setProfitPercentage(receivedStock.getPrice());
                string profit = to_string(order->getProfitPercentage());
                stringOrder += ",[Completed]," + profit + "%";
            }

            orderArr[i++] = stringOrder;
        }
    }

    // Allocate memory for the dynamic array
    string* orderDataArray = new string[size];
    copy(orderArr, orderArr + size, orderDataArray);

    // Send data to display panel
    xQueueSend(orderQueue, &orderDataArray, 0);
}

void readOrderArray(string* orderReceived) {
    string username;
    string stockname;
    string ordertype;
    string targetprice;
    string orderstatus;
    string profitpercentage;

    for (int i = 0; !orderReceived[i].empty(); i++) {
        istringstream iss(orderReceived[i]);
        getline(iss, username, ',');
        getline(iss, stockname, ',');
        getline(iss, ordertype, ',');
        getline(iss, targetprice, ',');

        string line = username + ": " + ordertype + " " + stockname + " stock at " + targetprice + ". ";

        if (getline(iss, orderstatus, ',') && getline(iss, profitpercentage, ',')) {
            if (stod(profitpercentage) > 0) {
                profitpercentage = "+" + profitpercentage;
            }
            line += orderstatus + " Current Profit = " + profitpercentage;
        }

        orderPanel.push_back(line);
    }

}

void saveStockArray(string* stockReceived) {
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
    string* orderListReceived;
    string* stockReceived = nullptr;


    while (true) {
        refresh = true;
        system("cls");
        cout << R"(
  ____   ______    ____    ______   _   _
 / ___| |_    _| /  __  \ /  ____| | | / /
 | |__    |  |   | |  | | | |      | |/ /
 \___ \   |  |   | |  | | | |      |   |
  ___) |  |  |   | |  | | | |____  | |\ \
 |____/   |__|   \ ____ / \______| |_| \_\
                                                     
    )" << endl;

        //print time
        cout << "Time: " << simulatedTime << endl << endl;

        //print orders
        cout << "Orders: " << endl;
        cout << "==============================================" << endl;

        if (xQueueReceive(orderQueue, &orderListReceived, 0)==pdTRUE) {
            orderPanel.clear();
            readOrderArray(orderListReceived);
            //refresh = true;
            continue; //restart the while loop again
        }
        else {
            refresh = false;
        }

        if (orderPanel.size() != 0) {
            for (int i = 0; i < orderPanel.size(); i++) {
                cout << orderPanel[i] << endl;
            }
        }else{
            cout << "There are no order!" << endl;
        }
        cout << "==============================================" << endl;

        //print stocks
        // Display headers

        cout << left << setw(20) << "Last Updated";
        cout << setw(20) << "Stock";
        cout << setw(20) << "Price";
        cout << right << "%Change"  << endl;
        

        if (xQueueReceive(stockQueue, &stockReceived, 0) == pdTRUE) {
            stockPanel.clear();
            saveStockArray(stockReceived);
            //refresh = true;
            continue;
        }
        else {
            refresh = false;
        }

        for (int i = 0; i < stockPanel.size(); i++) {
            cout << left << setw(20) << stockPanel[i].getTimestamp();
            cout << setw(20) << stockPanel[i].getSymbol();
            if (stockPanel[i].getPrice() > stockPanel[i].getPreviousPrice()) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            }
            else {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            }
            cout << setw(20) << stockPanel[i].getPrice() << " PREV : " << stockPanel[i].getPreviousPrice();
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
            cout << endl;
        }

        if (!refresh) {
            vTaskDelay(pdMS_TO_TICKS(1000));// wait for 1 second
        }
    }
}


