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
    DisplayPanel displayPanel;
    clock_t start;
    clock_t end;

public:
    void addOrder(const string username, const string stock, const string ordertype, const double price);
    void processStock(const Stock& newStock);
    void processOrder(Stock stock);
    void executeOrder(const User user, vector<Order*> ordersToExecute);
    vector<User*> getUserOrderList();
};
void notifyOrder(void* pvParameters);
void manageOrder(void* pvParameters);

#endif
