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
    cout << "Running CSVReaderTask" << endl;
    StockManager* stockManager = static_cast<StockManager*>(pvParameters);
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

            int timestamp = stoi(row[0]);
            string symbol = row[1];
            double price = stod(row[2]);
            
            cout << "The current Time is: ";
            cout << simulatedTime << endl;

            cout << symbol << " " << row[2] << endl;

            Stock stock(symbol, price, timestamp);
            stockManager->addStock(stock);

            row.clear();
        }

        // Add a delay here if needed to control the reading speed
        vTaskDelay(pdMS_TO_TICKS(100));  // 100 ms delay
    }
}

void CSVReader::reset()
{
    fileStream.close();
    fileStream.open(filename);
}

