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
    // Map Stock with Price
    while (simulatedTime < stock.getTimestamp()-1) {
        vTaskDelay(pdMS_TO_TICKS(1000));// wait for 1 second;
    }
    if (stocks.count(stock.getSymbol()) > 0) {
        //Stock already exists
        stocks[stock.getSymbol()].updatePrice(stock.getPrice());
        stocks[stock.getSymbol()].updateTime(stock.getTimestamp());
    }
    else {
        //Stock is new
        stocks[stock.getSymbol()] = stock;
    }

    // Send new stock data to DisplayPanel
    displayPanel.updateStock(stocks);

    // Send order to OrderManager
    orderManager.processStock(stock);
    
}

map<string, Stock> StockManager::getMap() {
    return stocks;
}


