/*
 * Реалізація класу CSVHandler.
 */
#include "CSV.hpp"

CSVHandler::CSVHandler(char delim, bool headers_on) 
    : delimiter(delim), has_headers(headers_on) {}

bool CSVHandler::readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;
    data.clear(); headers.clear();
    string line;
    bool first = true;
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;
        while (getline(ss, cell, delimiter)) row.push_back(cell);
        
        if (first && has_headers) {
            headers = row;
            first = false;
        } else {
            data.push_back(row);
        }
    }
    return true;
}

bool CSVHandler::writeFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) return false;
    if (has_headers) {
        for (size_t i = 0; i < headers.size(); ++i) 
            file << headers[i] << (i == headers.size()-1 ? "" : string(1, delimiter));
        file << "\n";
    }
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) 
            file << row[i] << (i == row.size()-1 ? "" : string(1, delimiter));
        file << "\n";
    }
    return true;
}

void CSVHandler::print() {
    if (has_headers) {
        for (auto& h : headers) cout << "[" << h << "] ";
        cout << endl;
    }
    for (auto& row : data) {
        for (auto& c : row) cout << c << "\t";
        cout << endl;
    }
}

string CSVHandler::getField(size_t row, size_t col) {
    if (row < data.size() && col < data[row].size()) return data[row][col];
    return "";
}

string CSVHandler::getField(size_t row, const string& name) {
    auto it = find(headers.begin(), headers.end(), name);
    if (it != headers.end()) return getField(row, distance(headers.begin(), it));
    return "";
}

void CSVHandler::setField(size_t row, size_t col, const string& val) {
    if (row < data.size() && col < data[row].size()) data[row][col] = val;
}

void CSVHandler::deleteRow(size_t row) {
    if (row < data.size()) data.erase(data.begin() + row);
}

void CSVHandler::addColumn(const string& name, const string& def) {
    if (has_headers) headers.push_back(name);
    for (auto& row : data) row.push_back(def);
}

// НОВЕ: Реалізація додавання рядка
void CSVHandler::addRow(const vector<string>& rowValues) {
    size_t colCount = getColCount();
    vector<string> newRow = rowValues;
    
    // Вирівнювання розміру рядка під кількість колонок
    if (newRow.size() < colCount) {
        newRow.resize(colCount, "");
    } else if (newRow.size() > colCount && colCount > 0) {
        newRow.resize(colCount);
    }
    
    data.push_back(newRow);
}