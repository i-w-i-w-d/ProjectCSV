/*
 * Завдання: 16 (CSV)
 * Студент: Максим Ульянов
 * Опис: Заголовочний файл класу CSV на C++.
 */

#ifndef CSV_HPP
#define CSV_HPP

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

class CSVHandler {
private:
    vector<vector<string>> data;
    vector<string> headers;
    char delimiter;
    bool has_headers;

public:
    CSVHandler(char delim = ',', bool headers_on = true);

    bool readFile(const string& filename);
    bool writeFile(const string& filename);
    void print();

    string getField(size_t row, size_t col);
    string getField(size_t row, const string& colName);
    void setField(size_t row, size_t col, const string& value);
    void deleteRow(size_t row);
    void addColumn(const string& headerName, const string& defaultValue);
    
    // НОВЕ: Додавання рядка
    void addRow(const vector<string>& rowValues);
    
    // Ітератор
    class CSVIterator {
        vector<vector<string>>::iterator ptr;
    public:
        CSVIterator(vector<vector<string>>::iterator p) : ptr(p) {}
        CSVIterator& operator++() { ptr++; return *this; }
        bool operator!=(const CSVIterator& other) { return ptr != other.ptr; }
        vector<string>& operator*() { return *ptr; }
    };
    CSVIterator begin() { return CSVIterator(data.begin()); }
    CSVIterator end() { return CSVIterator(data.end()); }

    size_t getRowCount() const { return data.size(); }
    size_t getColCount() const { return headers.size() > 0 ? headers.size() : (data.empty() ? 0 : data[0].size()); }
};

#endif