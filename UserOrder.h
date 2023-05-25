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

public:
    // Constructor
    Order(const std::string& stock, const std::string& orderType, double targetPrice);

    // Getter and setter for stock
    std::string getStock() const;
    void setStock(const std::string& stock);

    // Getter and setter for ordertype
    std::string getOrderType() const;
    void setOrderType(const std::string& orderType);

    // Getter and setter for targetprice
    double getTargetPrice() const;
    void setTargetPrice(double targetPrice);
};


class User {
private:
    string username;
    double balance;
    vector<Order*> orders;

public:
   
    // Constructor
    User(const std::string& username, double balance);

    // Getter and setter for username
    std::string getUsername() const;
    void setUsername(const std::string& username);

    // Getter and setter for balance
    double getBalance() const;
    void setBalance(double balance);

    // Getter and setter for orders array
    vector<Order*> getOrders() const;
    void newOrder(Order order);
    vector<Order*> getOrdersToExecute(const Stock& newStock);
};


#endif // !


