#ifndef DISPLAYPANEL_H
#define DISPLAYPANEL_H

#include <map>
#include <string>
#include "Stock.h"
#include "CSVReader.h"
#include "UserOrder.h"

using namespace std;

class DisplayPanel {
public:
	void updateTime(int ts, const map<string, Stock> stocks);
	void printStocks(const map<string, Stock> stock) const;
	void printOrderStatus(User* user);
};

#endif
