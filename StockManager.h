#ifndef STOCKMANAGER_H
#define STOCKMANAGER_H

#include <map>
#include <string>
#include "Stock.h"
#include "CSVReader.h"
#include "DisplayPanel.h"

using namespace std;

class StockManager {
private:
    map<string, Stock> stocks;
    DisplayPanel displayPanel;

public:
    void displayData() const;
    void addStock(const Stock& stock);
    void loadStockData(const string& filename);
    void setupDisplayPanel(const DisplayPanel& dp);
    map<string, Stock> getMap();

};

#endif  // STOCKMANAGER_H