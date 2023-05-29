#ifndef STOCKMANAGER_H
#define STOCKMANAGER_H
#define MAX_STOCK_QUEUE_SIZE 10

#include "FreeRTOS.h"
#include <map>
#include <string>
#include "Stock.h"
#include "CSVReader.h"
#include "DisplayPanel.h"
#include "queue.h"
#include "OrderManager.h"
#include "task.h"

using namespace std;
extern QueueHandle_t stockQueue;
extern Stock sendStockData;
extern Stock currentStockData;

class StockManager {
private:
    OrderManager orderManager;
    DisplayPanel displayPanel;
    QueueHandle_t stockQueue;  // Queue to hold stock data

public:
    void addStock(const Stock& stock);
    void setupDisplayPanel(const DisplayPanel& dp);
    map<string, Stock> getMap();

};

#endif  // STOCKMANAGER_H