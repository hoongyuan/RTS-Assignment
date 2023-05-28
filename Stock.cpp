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

double Stock::getPreviousPrice() const {
    return previousPrice;
}

int Stock::getTimestamp() const {
    return timestamp;
}

// Setter method
void Stock::setPreviousPrice(double price) {
    this->previousPrice = price;
}

void Stock::updatePrice(double price) {
    this->previousPrice = this->price;
    this->price = price;
}

void Stock::updateTime(int timestamp) {
    this->timestamp = timestamp;
}

