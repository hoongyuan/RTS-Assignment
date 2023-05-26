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
	void updateTime(int ts, const map<string, Stock> stocks);
	void printStocks(const map<string, Stock> stock) const;
	void updateStock(map<string, Stock> stocks);
	void updateOrderStatus(vector<User*> user);
};
void saveStockArray(string* stockReceived);
void printAll(void* pvParameters);

#endif
