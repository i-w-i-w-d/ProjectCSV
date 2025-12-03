/*
 * Крос-тест: C vs C++
 */
#include "CSV.hpp"
extern "C" {
    #include "../CSV_C/CSV.h"
}

int main() {
    cout << "--- Крос-тест ---" << endl;
    
    // Створення еталону
    string fname = "cross.csv";
    ofstream f(fname);
    f << "A,B\n1,2";
    f.close();

    // C++
    CSVHandler cppObj(',', true);
    cppObj.readFile(fname);
    string valCpp = cppObj.getField(0, 0);

    // C
    CSVTable *cObj = csv_create(',', 1);
    csv_read_file(cObj, fname.c_str());
    char* valC = csv_get_field(cObj, 0, 0);

    cout << "C++: " << valCpp << endl;
    cout << "C:   " << (valC ? valC : "NULL") << endl;

    if (valCpp == string(valC)) cout << "[OK] Збігаються!" << endl;
    else cout << "[FAIL] Різні значення." << endl;

    csv_free(cObj);
    return 0;
}