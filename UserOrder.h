#ifndef USERORDER_H
#define USERORDER_H

#include <string>
#include <vector>
#include "Stock.h"

using namespace std;

class Order {
private:
    string stock;
    string orderType;
    double targetPrice;
    bool orderStatus = false;
    double profitPercentage = 0.00;
    double executionTime = 0;

public:
    // Constructor
    Order(const string& stock, const string& orderType, double targetPrice);

    // Getter and setter for stock
    string getStock() const;
    void setStock(const string& stock);

    // Getter and setter for ordertype
    string getOrderType() const;
    void setOrderType(const string& orderType);

    // Getter and setter for targetprice
    double getTargetPrice() const;
    void setTargetPrice(double targetPrice);

    // Getter and setter for orderstatus
    bool getOrderStatus() const;
    void setOrderComplete();

    //Getter and setter for profit
    float getProfitPercentage();
    void setProfitPercentage(const double stockPrice);

    // // Getter and setter for executionTime
    double getExecutionTime() const;
    void setExecutionTime(const double duration);
};


class User {
private:
    string username;
    double balance;
    vector<Order*> orders;

public:
   
    // Constructor
    User(const string& username, double balance);

    // Getter and setter for username
    string getUsername() const;
    void setUsername(const string& username);

    // Getter and setter for balance
    double getBalance() const;
    void setBalance(double balance);

    // Getter and setter for orders array
    vector<Order*> getOrders() const;
    void newOrder(Order order);
    vector<Order*> getOrdersToExecute(const Stock& newStock);
};


#endif // !


