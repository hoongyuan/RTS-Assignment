#ifndef STOCK_H
#define STOCK_H

#include <string>

using namespace std;

class Stock {
private:
    string symbol;  // Symbol of the stock
    double price;   // Current price of the stock
    double previousPrice;
    int timestamp;  // timestamp

public:
    //Default Constructor
    Stock();

    // Constructor
    Stock(const string symbol, double price, int timestamp);

    // Getter methods
    string getSymbol() const;
    double getPrice() const;
    double getPreviousPrice() const;
    int getTimestamp() const;

    // Setter methods
    void updatePrice(double price);
};

#endif  // STOCK_H
