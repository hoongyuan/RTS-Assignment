#include "FreeRTOS.h"
#include <iostream>
#include "portmacro.h"
#include <string>
#include "OrderManager.h"
#include "queue.h"
#include "StockManager.h"
#include "task.h"

using namespace std;

TaskHandle_t receiverTaskHandle;
Stock receivedStock;
bool notify;
const double USER_INITIAL_BALANCE = 100.00; 
map<string, vector<User*>> orderBStock;

void OrderManager::addOrder(const string username, const string stock, const string ordertype, const double price) {
    // Map User with Order
    User* user = new User(username, USER_INITIAL_BALANCE); //Hoong 100
    Order order = Order(stock, ordertype, price); //MAYBANK BUY 8.30
    user->newOrder(order);

    // Map User with Stock
    orderBStock[stock].push_back(user);

    // Send updated Order List to DisplayPanel
    displayPanel.updateOrderStatus(receivedStock, getUserOrderList());
}

void OrderManager::processStock(const Stock& newStock) 
{
    receivedStock = newStock;

    // Filter users based on the given stock
    vector<User*> users = orderBStock[receivedStock.getSymbol()];

    if (!users.empty()) {
        notify = true;
    }
}

void OrderManager::processOrder(const Stock newStock) {
    const vector<User*> users = orderBStock[newStock.getSymbol()];
    for (const auto& user : users) {
        // Loop through users' orderlist to filter stocks the match the order condition
        vector<Order*> ordersToExecute = user->getOrdersToExecute(newStock);
        executeOrder(*user, ordersToExecute);
    }
}

void OrderManager::executeOrder(const User user, vector<Order*> ordersToExecute) {
    for (const auto& order : ordersToExecute) {
        order->setOrderComplete();
        displayPanel.updateOrderStatus(receivedStock, getUserOrderList());
    }
}

vector<User*> OrderManager::getUserOrderList() {
    vector<User*> orderList;

    // Generate a list of users by looping through orderBStock
    for (const auto& pair : orderBStock) {
        for (const auto& user : pair.second) {
            orderList.push_back(user);
        }
    }

    return orderList;
}

//void notifyOrder(void* pvParameters) {
//    while (true) {
//        if (notify = true) {
//            // Send a notification to the receiver task
//            xTaskNotify(receiverTaskHandle, 0, eNoAction);
//        }
//    }
//}

void manageOrder(void* pvParameters) {
    OrderManager* orderManager = static_cast<OrderManager*>(pvParameters);
    while (true) {
        //uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (notify) {
            orderManager->processOrder(receivedStock);
            notify = false;
        }
    }
}


