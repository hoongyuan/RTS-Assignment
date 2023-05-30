#include "FreeRTOS.h"
#include "task.h"
#include <fstream>
#include <sstream>
#include "CSVReader.h"
#include "StockManager.h"
#include <iostream>
#include "SimulatedTime.h"
#include "DisplayPanel.h"
#include "OrderManager.h"


// Include the header file for your CSVReader implementation
#include "CSVReader.h"
#include "Stock.h"

using namespace std;

// Define the stack size and priority for the CSV reader task
#define CSV_READER_TASK_STACK_SIZE    1024
#define CSV_READER_TASK_PRIORITY      1

Stock stock;
QueueHandle_t sendParseStock = xQueueCreate(50, 20); //10 queue, 50 stocks

CSVReader::CSVReader(const string& filename)
{
    fileStream.open(filename);
}

bool CSVReader::readNextRow(vector<string>& rowData)
{
    string line;
    if (getline(fileStream, line))
    {
        stringstream ss(line);
        string cell;

        rowData.clear();
        while (getline(ss, cell, ','))
        {
            rowData.push_back(cell);
        }

        return true;
    }

    return false;
}

void csvReaderTask(void* pvParameters)
{
    StockManager* stockManager = static_cast<StockManager*>(pvParameters);
    DisplayPanel displaypanel;
    CSVReader csvReader("stocks.txt");
    vector<string> header;
    csvReader.readNextRow(header);
    int wait = 0;

    while (true) {
        vector<string> row;
        if (csvReader.readNextRow(row)) {
            if (row.empty()) {
                // End of file reached, restart from the beginning
                csvReader.reset();
                continue;
            }

            // extract variables from row
            int timestamp = stoi(row[0]);
            string symbol = row[1];
            double price = stod(row[2]);
            Stock* stock = new Stock(symbol, price, timestamp);
            //string stockCSVString = symbol + "," + to_string(price) + "," + to_string(timestamp);
            //displaypanel.updateTime(timestamp,stockManager->getMap());

            //stockManager->addStock(stock);
            
            /*if (uxQueueSpacesAvailable(sendParseStock) == 0) {
                xQueueReset(sendParseStock);
            }*/

            xQueueSend(sendParseStock, &stock, 100);

            row.clear();
        }

    }
}

void csvParseStock(void* pvParameters) {
    Stock* parseStock;
    StockManager* stockManager = static_cast<StockManager*>(pvParameters);
    while (true) {
        if (xQueueReceive(sendParseStock, &parseStock, 0) == pdTRUE) {
            try {
                stockManager->addStock(*parseStock);
                //vTaskDelay(pdMS_TO_TICKS(1000));
            }
            catch (exception& ex) {
                // Code to handle exception1
            }
        }
        else {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

void csvReadOrder(void* pvParameters) {
    OrderManager* orderManager = static_cast<OrderManager*>(pvParameters);
    DisplayPanel displaypanel;
    CSVReader csvReader("orders.txt");
    vector<string> header;
    csvReader.readNextRow(header);

    while (true) {
        vector<string> row;
        if (csvReader.readNextRow(row)) {
            if (row.empty()) {
                // End of file reached, restart from the beginning
                csvReader.reset();
                continue;
            }

            // extract variables from row
            string user = row[0];
            string stock = row[1];
            string ordertype = row[2];
            double targetprice = stod(row[3]);

            orderManager->addOrder(user,stock,ordertype,targetprice);    


            row.clear();
        }
        else {
            vTaskSuspend(NULL);
        }

    }
}

void CSVReader::reset()
{
    fileStream.close();
    fileStream.open(filename);
}

