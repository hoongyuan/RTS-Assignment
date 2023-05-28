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


void StockManager::setupDisplayPanel(const DisplayPanel& dp) {
    this->displayPanel = dp;
}

void StockManager::addStock(const Stock& stock)
{
    const double price = stock.getPrice();
    // Map Stock with Price
    while (simulatedTime < stock.getTimestamp()) {
        vTaskDelay(pdMS_TO_TICKS(1000));// wait for 1 second;
    }
    if (stocks.count(stock.getSymbol()) > 0) {
        //Stock already exists
        stocks[stock.getSymbol()].updatePrice(price);
        stocks[stock.getSymbol()].updateTime(stock.getTimestamp());
    }
    else {
        //Stock is new
        stocks[stock.getSymbol()] = stock;
    }

    // Send new stock data to DisplayPanel
    displayPanel.updateStock(stocks);

    // Send order to OrderManager
    orderManager.processStock(stocks[stock.getSymbol()]);
    vTaskDelay(pdMS_TO_TICKS(100));// wait for 100 ms;
    
}

map<string, Stock> StockManager::getMap() {
    return stocks;
}


