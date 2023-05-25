#ifndef CSVREADER_H
#define CSVREADER_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class CSVReader {
private:
    ifstream fileStream;
    string filename;

public:
    CSVReader(const string& filename);
    bool readNextRow(vector<string>& rowData);
    void reset();
};

void csvReaderTask(void* pvParameters);
void csvReadOrder(void* pvParameters);

#endif  // CSVREADER_H