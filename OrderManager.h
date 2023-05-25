#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <map>
#include <string>
#include "Stock.h"
#include "CSVReader.h"
#include "DisplayPanel.h"
#include "UserOrder.h"

using namespace std;

class OrderManager {
private:
    map<string, vector<Order*>> orderBUser;
    map<string, vector<User*>> orderBStock;
    DisplayPanel displayPanel;

public:
    void addOrder(string username, string stock, string ordertype, double price);
    void processStock(const Stock& newStock);
    void processOrder(Stock newStock, vector<User*> users);
    void executeOrder(vector<Order*> ordersToExecute);
};


#endif
