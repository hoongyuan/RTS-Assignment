#include "FreeRTOS.h"
#include "StockManager.h"
#include <iostream>
#include "CSVReader.h"
#include "task.h"
#include <iomanip>
#include "SimulatedTime.h"
#include <Windows.h>
#include "DisplayPanel.h"
#include "queue.h"
#include "OrderManager.h"

map<string, Stock> stocks;

void StockManager::setupDisplayPanel(const DisplayPanel& dp) {
    this->displayPanel = dp;
}

void StockManager::addStock(const Stock& stock)
{
    try {
        const int timestamp = stock.getTimestamp();

        while (simulatedTime < timestamp) {
            //wait = (timestamp - simulatedTime) * 1000;
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        const string stockSymbol = stock.getSymbol();
        const double price = stock.getPrice();

        auto it = stocks.find(stockSymbol);
        if (it != stocks.end()) {
            //Stock already exists
            stocks[stock.getSymbol()].updatePrice(price);
            stocks[stock.getSymbol()].updateTime(timestamp);
        }
        else {
            //Stock is new
            stocks[stock.getSymbol()] = stock;
        }

        // Send new stock data to DisplayPanel
        displayPanel.updateStock(stocks[stockSymbol]);

        // Send order to OrderManager
        orderManager.processStock(stocks[stock.getSymbol()]);
        //vTaskDelay(pdMS_TO_TICKS(200));// wait for 100 ms;
    }
    catch (exception& ex) {
        // Code to handle exception1
    }
        
}




map<string, Stock> StockManager::getMap() {
    return stocks;
}


