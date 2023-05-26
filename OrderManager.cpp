#include "FreeRTOS.h"
#include <iostream>
#include "portmacro.h"
#include <string>
#include "OrderManager.h"
#include "queue.h"
#include "StockManager.h"
#include "task.h"

using namespace std;
const double USER_INITIAL_BALANCE = 100.00; 
Stock receivedStockData;

void OrderManager::addOrder(string username, string stock, string ordertype, double price) {
    // Map User with Order
    User* user = new User(username, USER_INITIAL_BALANCE); //Hoong 100
    Order order = Order(stock, ordertype, price); //MAYBANK BUY 8.30
    user->newOrder(order);

    orderBStock[stock].push_back(user);
    displayPanel.updateOrderStatus(returnOrderList());
}

void OrderManager::processStock(const Stock& newStock) 
{
    // Filter users based on the given stock
    const vector<User*> users = orderBStock[newStock.getSymbol()];
    processOrder(newStock, users);
}

void OrderManager::processOrder(Stock newStock, vector<User*> users) {

    for (const auto& user : users) {
        // Loop through users' orderlist to filter stocks the match the order condition
        vector<Order*> ordersToExecute = user->getOrdersToExecute(newStock);
    }
}

void OrderManager::executeOrder(vector<Order*> ordersToExecute) {

    for (const auto& order : ordersToExecute) {
        //update completed orders to a txt file

        //display notification on panel
    }
}

vector<User*> OrderManager::returnOrderList() {
    
    vector<User*> orderList;

    // Generate a list of users by looping through orderBStock
    for (const auto& pair : orderBStock) {
        for (const auto& user : pair.second) {
            orderList.push_back(user);
        }
    }

    return orderList;
}



