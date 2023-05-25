#include <string>
#include "UserOrder.h"

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


// User class implementation
// Constructor
User::User(const string& username, double balance) {
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

