#include "FreeRTOS.h"
#include "task.h"
#include <fstream>
#include <sstream>

// Include the header file for your CSVReader implementation
#include "CSVReader.h"
#include "Stock.h"

using namespace std;

// Define task handles
TaskHandle_t csvReaderTaskHandle;

// Define the stack size and priority for the CSV reader task
#define CSV_READER_TASK_STACK_SIZE    1024
#define CSV_READER_TASK_PRIORITY      1

#include "CSVReader.h"
#include "StockManager.h"
#include <iostream>
#include "SimulatedTime.h"
#include "DisplayPanel.h"

//volatile uint32_t simulatedTime;

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
    cout << R"(
  ____   ______    ____    ______   _   _
 / ___| |_    _| /  __  \ /  ____| | | / /
 | |__    |  |   | |  | | | |      | |/ /
 \___ \   |  |   | |  | | | |      |   |
  ___) |  |  |   | |  | | | |____  | |\ \
 |____/   |__|   \ ____ / \______| |_| \_\
                                                     
    )" << endl;

    StockManager* stockManager = static_cast<StockManager*>(pvParameters);
    DisplayPanel displaypanel;
    CSVReader csvReader("stocks.txt");
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
            int timestamp = stoi(row[0]);
            string symbol = row[1];
            double price = stod(row[2]);
            Stock stock(symbol, price, timestamp);
            
            displaypanel.updateTime(timestamp,stockManager->getMap());

            stockManager->addStock(stock);

            row.clear();
        }

    }
}

void CSVReader::reset()
{
    fileStream.close();
    fileStream.open(filename);
}

