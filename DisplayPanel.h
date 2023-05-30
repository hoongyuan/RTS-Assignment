#ifndef DISPLAYPANEL_H
#define DISPLAYPANEL_H

#include <map>
#include <string>
#include "Stock.h"
#include "CSVReader.h"
#include "UserOrder.h"

using namespace std;

class DisplayPanel {
private:
	vector<User*> userOrders;

public:
	string stockMapToString(const Stock stocks);
	void updateStock(const Stock stock);
	void updateOrderStatus(const Stock receivedStock, vector<User*> user);
	void updateOrderStatus(const double price, const vector<User*> users, const Order* order);
};
void saveStockArray(const string stockReceived);
void printAll(void* pvParameters);
void updateProfit(void* pvParameters);

#endif
