#include "FreeRTOS.h"
#include <iostream>
#include "StockManager.h"
#include "CSVReader.h"
#include <Windows.h>
#include "SimulatedTime.h"
#include <iomanip>
#include "DisplayPanel.h"
#include "task.h"
#include <algorithm>

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
QueueHandle_t orderQueue = xQueueCreate(50, 50); //50 queue, 50 orders
QueueHandle_t stockQueue = xQueueCreate(100, 60); //100 queue, 60 stocks
vector<Stock> stockPanel;
vector<string> orderPanel;
vector<User*> usersList;
bool refresh = false;
bool receiveOrder = false;
bool receiveStock = false;

string DisplayPanel::stockMapToString(const Stock stocks) {
    string result;
    Stock stock = stocks;

    result = to_string(stock.getTimestamp()) + "," + stock.getSymbol() + "," + to_string(stock.getPrice()) + "," + to_string(stock.getPreviousPrice());

    // Remove the trailing comma
    if (!result.empty()) {
        result.pop_back();
    }
    return result;
}

// Convert the data from map to string and store in array
void DisplayPanel::updateStock(const Stock stock) {

    string stockString = to_string(stock.getTimestamp()) + "," + stock.getSymbol() + "," + to_string(stock.getPrice());
    if (stock.getPreviousPrice() > 0) {
        stockString += to_string(stock.getPreviousPrice());
    }
    // Send data to display panel
    xQueueSend(stockQueue, stockString.c_str(), 1000);
}

void DisplayPanel::updateOrderStatus(const Stock receivedStock, const vector<User*> users)
{
    //const int size = sizeof(users);
    const int SIZE = 150; 
    string orderArr[SIZE];
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
            if (order->getOrderStatus() == true && order->getOrderCloseStatus() != true) {
                order->setProfitPercentage(receivedStock.getPrice());
                string profit = to_string(order->getProfitPercentage());
                string timetaken = to_string(order->getExecutionTime());
                stringOrder += ",[Open Order]," + profit + "%," + timetaken;
            }
            else if (order->getOrderCloseStatus() == true) {
                if (order->getOrderType() == "BUY") {
                    string timetaken = to_string(order->getExecutionTime());
                    stringOrder += ",[Close Order],NaN," + timetaken;
                }else if (order->getOrderType() == "SELL") {
                    string profit = to_string(order->getCloseProfit());
                    string timetaken = to_string(order->getExecutionTime());
                    stringOrder += ",[Close Order]," + profit + "%," + timetaken;
                }
            }
            orderArr[i++] = stringOrder;
        }
    }
    orderArr[i] = "END";
    // Allocate memory for the dynamic array
    string* orderDataArray = new string[i];
    copy(orderArr, orderArr + i, orderDataArray);
    // Send data to display panel
    xQueueSend(orderQueue, &orderDataArray, 0);
    usersList = users;
}


void DisplayPanel::updateOrderStatus(const double price, const vector<User*> users, const Order* pendingUpdateOrder) {
    //const int size = sizeof(users);
    const int SIZE = 150;
    string orderArr[SIZE];
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
            if (order->getOrderStatus() == true && order->getOrderCloseStatus() != true) {
                if (order == pendingUpdateOrder) {
                    order->setProfitPercentage(price);
                }
                string profit = to_string(order->getProfitPercentage());
                string timetaken = to_string(order->getExecutionTime());
                stringOrder += ",[Open Order]," + profit + "%," + timetaken;
            }
            else if (order->getOrderCloseStatus() == true) {
                if (order->getOrderType() == "BUY") {
                    string timetaken = to_string(order->getExecutionTime());
                    stringOrder += ",[Close Order],NaN," + timetaken;
                }
                else if (order->getOrderType() == "SELL") {
                    string profit = to_string(order->getCloseProfit());
                    string timetaken = to_string(order->getExecutionTime());
                    stringOrder += ",[Close Order]," + profit + "%," + timetaken;
                }
            }

            orderArr[i] = stringOrder;
            i++;
        }
    }
    orderArr[i++] = "END";
    // Allocate memory for the dynamic array
    string* orderDataArray = new string[i];
    copy(orderArr, orderArr + i, orderDataArray);
    // Send data to display panel
    xQueueSend(orderQueue, &orderDataArray, 0);
    usersList = users;
}

void readOrderArray(string* orderReceived) {
    const string sentinel = "END";
    string username;
    string stockname;
    string ordertype;
    string targetprice;
    string orderstatus;
    string profitpercentage;
    string timetaken;

    for (int i = 0;; i++) {
        orderstatus = "";
        profitpercentage = "";
        timetaken = "";
        try {
            if (!orderReceived[i].empty()) {
                istringstream iss(orderReceived[i]);
                getline(iss, username, ',');
                getline(iss, stockname, ',');
                getline(iss, ordertype, ',');
                getline(iss, targetprice, ',');

                string line = username + "," + ordertype + "," + stockname + "," + targetprice;

                if (getline(iss, orderstatus, ',') && getline(iss, profitpercentage, ',')) {
                    if (stod(profitpercentage) > 0) {
                        profitpercentage = "+" + profitpercentage;
                    }
                    getline(iss, timetaken, ',');
                    line += "," + orderstatus + "," + profitpercentage + "," + timetaken;
                }

                orderPanel.push_back(line);
            }
            else {
                break;
            }
            if (orderReceived[i + 1] == sentinel) {
                break;
            }
        }
        catch (exception& ex) {
            break;
        }
    }

}

void saveStockArray(const string stockReceived) {

    //Find and update the matching stock data received
    string timestamp;
    string stockname;
    string price;
    string prevprice;
    bool found = false;

    istringstream iss(stockReceived);
    getline(iss, timestamp, ',');
    getline(iss, stockname, ',');
    getline(iss, price, ',');
    getline(iss, prevprice, ',');

    for (auto& stock : stockPanel) {
        if (stockname == stock.getSymbol()) {
            stock.updatePrice(stod(price));
            stock.updateTime(stoi(timestamp));
            found = true;
            break;
        }
    }

    if (!found) {
        Stock stock = Stock(stockname, stod(price), stoi(timestamp));
        stockPanel.push_back(stock);
    }
}

void updateProfit(void* pvParameters) {
    DisplayPanel* displaypanel = static_cast<DisplayPanel*>(pvParameters);
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        const vector<Stock> stockList = stockPanel;
        //displaypanel->updateOrderStatus(stock, usersList);
        for (const auto& user : usersList) { //Loop through user list

            for (auto& order : user->getOrders()) { //Loop through all user's order

                if (order->getOrderStatus() == true && order->getOrderType() == "BUY") { //Find completed orders

                    for (const auto& stock : stockList) {
                        if (stock.getSymbol() == order->getStock()) {
                            //order->setProfitPercentage(stock.getPrice());
                            displaypanel->updateOrderStatus(stock.getPrice(), usersList, order);
                            break;
                        }
                    }

                }
            }
        }
    }
}

void printAll(void* pvParameters) {

    DisplayPanel* displaypanel = static_cast<DisplayPanel*>(pvParameters);

    string timestamp;
    string stockname;
    string price;
    string* orderListReceived;
    const int MAX_STRING_SIZE = 150;
    char stockReceived[MAX_STRING_SIZE];

    while (true) {
        refresh = true;
        double averageExecuteTime = 0;
        if (xQueueReceive(orderQueue, &orderListReceived, 0) == pdTRUE) {
            orderPanel.clear();
            readOrderArray(orderListReceived);
            continue; //restart the while loop again
        }
        else {
            refresh = false;
        }

        // Check if receive new stock data
        if (xQueueReceive(stockQueue, &stockReceived, 0) == pdTRUE) {
            if (uxQueueSpacesAvailable(stockQueue) == 0) {
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
            saveStockArray(stockReceived);
            continue; //restart the while loop again
        }
        else {
            refresh = false;
        }
        system("cls");
        //       cout << R"(
        // ____   ______    ____    ______   _   _
        /// ___| |_    _| /  __  \ /  ____| | | / /
        //| |__    |  |   | |  | | | |      | |/ /
        //\___ \   |  |   | |  | | | |      |   |
        // ___) |  |  |   | |  | | | |____  | |\ \
        //|____/   |__|   \ ____ / \______| |_| \_\
        //                                                    
        //   )" << endl;

        //print time
        cout << "Time: " << simulatedTime << endl << endl;

        cout << left << setw(14) << "User";
        cout << setw(10) << "BUY/SELL";
        cout << setw(37) << "Stock";
        cout << setw(14) << "Order Price";
        cout << setw(15) << "Order Status";
        cout << setw(30) << "Profit Percentage";

        cout << left << setw(14) << "User";
        cout << setw(10) << "BUY/SELL";
        cout << setw(37) << "Stock";
        cout << setw(14) << "Order Price";
        cout << setw(15) << "Order Status";
        cout << setw(15) << "Profit Percentage";
        cout << endl;
        
        int length = 227;
        for (int i = 0; i < length; i++) {
            cout << "=";
        }
        cout << endl;

        if (orderPanel.size() != 0) {
            string username;
            string ordertype;
            string stock;
            string targetprice;
            string orderstatus;
            string profit;
            string timetaken;
            for (int i = 0; i < orderPanel.size(); i++) {
                //cout << orderPanel[i] << endl;
                orderstatus = "";
                profit = "";
                timetaken = "";
                istringstream iss(orderPanel[i]);
                getline(iss, username, ',');
                getline(iss, ordertype, ',');
                getline(iss, stock, ',');
                getline(iss, targetprice, ',');
                getline(iss, orderstatus, ',');
                getline(iss, profit, ',');
                getline(iss, timetaken, ',');
                int rightSidePadding = 59;

                if (orderstatus == "[Open Order]" || orderstatus == "[Close Order]") {
                    rightSidePadding = 14;
                }

                cout << left << setw(14) << username
                    << setw(10) << ordertype
                    << setw(37) << stock
                    << setw(rightSidePadding) << targetprice;

                if (orderstatus == "[Open Order]" || orderstatus == "[Close Order]") {
                    cout << left << setw(15) << orderstatus;
                    cout << setw(15) << profit;
                    cout << setw(15) << timetaken;
                    averageExecuteTime += stod(timetaken);
                }

                if (i + 1 < orderPanel.size()) {
                    string username1;
                    string ordertype1;
                    string stock1;
                    string targetprice1;
                    string orderstatus1;
                    string profit1;
                    string timetaken1;
                    istringstream iss(orderPanel[i+1]);
                    getline(iss, username1, ',');
                    getline(iss, ordertype1, ',');
                    getline(iss, stock1, ',');
                    getline(iss, targetprice1, ',');
                    getline(iss, orderstatus1, ',');
                    getline(iss, profit1, ',');
                    getline(iss, timetaken1, ',');

                    cout << left << setw(14) << username1
                        << setw(10) << ordertype1
                        << setw(37) << stock1
                        << setw(14) << targetprice1;

                    if (orderstatus1 == "[Open Order]" || orderstatus1 == "[Close Order]") {
                        cout << left << setw(15) << orderstatus1;
                        cout << setw(15) << profit1;
                        cout << setw(8) << timetaken1;
                        averageExecuteTime += stod(timetaken1);
                    }
                    i++;
                }
                cout << endl;
            }
        }
        else {
            cout << "There are no order!" << endl;
        }
        for (int i = 0; i < length; i++) {
            cout << "=";
        }

        //print average execution time of orders
        if (orderPanel.size() != 0) {
            cout << endl;
            cout << "Average Order Execution Time: " << fixed << setprecision(5) << averageExecuteTime / orderPanel.size() << endl;
        }
        cout << endl;

        //print stocks
        // Display headers
        cout << left << setw(20) << "Last Updated";
        cout << setw(40) << "Stock";
        cout << setw(20) << "Price";
        //cout << "%Change"  << endl;
        cout << left << setw(20) << "Last Updated";
        cout << setw(40) << "Stock";
        cout << setw(20) << "Price" << endl;
        sort(stockPanel.begin(), stockPanel.end(), [](const Stock& a, const Stock& b) {
            return a.getSymbol() < b.getSymbol();
            });
        for (int i = 0; i < stockPanel.size(); i++) {
            //print timestamp
            cout << left << setw(20) << stockPanel[i].getTimestamp();
            //print stock name
            cout << setw(40) << stockPanel[i].getSymbol();
            //price color
            if (stockPanel[i].getPrice() > stockPanel[i].getPreviousPrice()) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            }
            else {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            }
            //print price
            cout << setw(20) << stockPanel[i].getPrice();
            //reset color
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

            if (i + 1 < stockPanel.size()) {
                cout << left << setw(20) << stockPanel[i + 1].getTimestamp();
                cout << setw(40) << stockPanel[i + 1].getSymbol();
                if (stockPanel[i + 1].getPrice() > stockPanel[i + 1].getPreviousPrice()) {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                }
                else {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                }
                cout << setw(40) << stockPanel[i + 1].getPrice();
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
                i++;
            }
            cout << endl;
        }
        if (!refresh) {
            vTaskDelay(pdMS_TO_TICKS(1000));// wait for 1 second
        }
    }
}


