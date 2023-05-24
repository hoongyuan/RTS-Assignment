#include "Stock.h"

using namespace std;

//Default Constructor
Stock::Stock() {}

// Constructor
Stock::Stock(const string symbol, double price, int timestamp)
    : symbol(symbol), price(price), timestamp(timestamp) {}

// Getter methods
string Stock::getSymbol() const {
    return symbol;
}

double Stock::getPrice() const {
    return price;
}

int Stock::getTimestamp() const {
    return timestamp;
}

// Setter method
void Stock::setPrice(double price) {
    this->price = price;
}
