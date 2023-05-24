#ifndef STOCKMANAGER_H
#define STOCKMANAGER_H

#include <map>
#include <string>
#include "Stock.h"
#include "CSVReader.h"

using namespace std;

class StockManager {
private:
    map<string,Stock> stocks;

public:
    void addStock(const Stock& stock);
    void loadStockData(const std::string& filename);
    void displayData()const;
};

#endif  // STOCKMANAGER_H