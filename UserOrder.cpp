#include <string>
#include "UserOrder.h"
#include "Stock.h"

using namespace std;

// Order class implementation
// Constructor
Order::Order(const string& stock, const string& orderType, double targetPrice)
    : stock(stock), orderType(orderType), targetPrice(targetPrice) {
}

// Getter and setter for stock
string Order::getStock() const {
    return stock;
}

void Order::setStock(const string& stock) {
    this->stock = stock;
}

// Getter and setter for ordertype
string Order::getOrderType() const {
    return orderType;
}

void Order::setOrderType(const string& orderType) {
    this->orderType = orderType;
}

// Getter and setter for targetprice
double Order::getTargetPrice() const {
    return targetPrice;
}

void Order::setTargetPrice(double targetPrice) {
    this->targetPrice = targetPrice;
}

// Getter and setter for orderstatus
bool Order::getOrderStatus() const{
    return orderStatus;
}

void Order::setOrderComplete() {
    this->orderStatus = true;
}

// Getter and setter for profit
float Order::getProfitPercentage() {
    return profitPercentage;
}

void Order::setProfitPercentage(const double stockPrice) {
    this->profitPercentage = (stockPrice - targetPrice)/ targetPrice * 100;
}

// Getter and setter for ExecutionTime
double
Order::getExecutionTime() const{
    return executionTime;
}
void Order::setExecutionTime(const double duration) {
    this->executionTime = duration;
}


// User class implementation
// Constructor
User::User(const string& username, double balance) {
    this->username = username;
    this->balance = balance;
}

// Getter and setter for username
string User::getUsername() const {
    return username;
}

void User::setUsername(const string& username) {
    this->username = username;
}

// Getter and setter for balance
double User::getBalance() const {
    return balance;
}

void User::setBalance(double balance) {
    this->balance = balance;
}

// Getter and setter for orders
vector<Order*> User::getOrders() const {
    return this->orders;
};

void User::newOrder(Order order) {
    Order* newOrder = new Order(order);
    this->orders.push_back(newOrder);
}

vector<Order*> User::getOrdersToExecute(const Stock& newStock) {
    
    vector<Order*> orderList = this->orders;
    vector<Order*> orderByNewStock;

    for (const auto& order : orderList) {

        if (order->getStock() == newStock.getSymbol()) {
            double currprice = newStock.getPrice();
            double prevprice = newStock.getPreviousPrice();
            double targetprice = order->getTargetPrice();

            if ((currprice < targetprice && targetprice < prevprice)
                || (currprice > targetprice && targetprice > prevprice) 
                || targetprice == currprice) {
                orderByNewStock.push_back(order);
            }
        }
    }

    return orderByNewStock;
}
