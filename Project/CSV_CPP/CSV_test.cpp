/*
 * Завдання: 16 (CSV)
 * Студент: Максим Ульянов
 * Опис: Інтерактивне меню для роботи з класом CSVHandler.
 */

#include "CSV.hpp"

void showMenu() {
    cout << "\n--- МЕНЮ (C++) ---" << endl;
    cout << "1. Показати таблицю" << endl;
    cout << "2. Знайти значення (getField)" << endl;
    cout << "3. Змінити значення (setField)" << endl;
    cout << "4. Додати стовпчик (addColumn)" << endl;
    cout << "5. Видалити рядок (deleteRow)" << endl;
    cout << "6. Додати новий РЯДОК (addRow)" << endl;
    cout << "7. Зберегти та вийти" << endl;
    cout << "Ваш вибір: ";
}

int main() {
    cout << "--- C++ CSV Test ---" << endl;
    int choice;
    cout << "1. Авто-тест\n2. Інтерактивне меню\nВибір: ";
    cin >> choice;

    if (choice == 1) {
        // --- АВТОМАТИЧНИЙ ТЕСТ ---
        ofstream f("test_cpp.csv");
        f << "ID,Name\n1,Maxim\n2,Petro";
        f.close();
        CSVHandler csv(',', true);
        if (csv.readFile("test_cpp.csv")) {
            csv.print();
            
            // Тест додавання рядка
            cout << "Додавання рядка..." << endl;
            csv.addRow({"3", "Ivan"});
            
            csv.writeFile("result_cpp.csv");
            cout << "Збережено у result_cpp.csv" << endl;
        }
    } else {
        // --- ІНТЕРАКТИВНЕ МЕНЮ ---
        string fname, buf, val;
        char delim;
        int r, c;

        cout << "Введіть ім'я файлу: ";
        cin >> fname;
        cout << "Введіть роздільник: ";
        cin >> delim;

        CSVHandler csv(delim, true);
        if (csv.readFile(fname)) {
            cout << "Файл успішно завантажено." << endl;
        } else {
            cout << "Файл не знайдено (або помилка). Працюємо з порожньою таблицею." << endl;
        }

        bool running = true;
        while (running) {
            showMenu();
            int action;
            cin >> action;

            switch (action) {
            case 1:
                csv.print();
                break;
            case 2:
                cout << "Рядок та Стовпчик: ";
                cin >> r >> c;
                cout << "Значення: " << csv.getField(r, c) << endl;
                break;
            case 3:
                cout << "Рядок Стовпчик НовеЗначення: ";
                cin >> r >> c >> val;
                csv.setField(r, c, val);
                cout << "Виконано." << endl;
                break;
            case 4:
                cout << "Назва нового стовпчика: ";
                cin >> buf;
                csv.addColumn(buf, "-");
                cout << "Додано." << endl;
                break;
            case 5:
                cout << "Номер рядка для видалення: ";
                cin >> r;
                csv.deleteRow(r);
                cout << "Видалено." << endl;
                break;
            case 6: {
                vector<string> newRow;
                size_t cols = csv.getColCount();
                if (cols == 0) {
                     cout << "Спочатку додайте стовпчики!" << endl;
                } else {
                    cout << "Введіть " << cols << " значень через пробіл:" << endl;
                    for(size_t i=0; i < cols; ++i) {
                        string temp;
                        cin >> temp;
                        newRow.push_back(temp);
                    }
                    csv.addRow(newRow);
                    cout << "Рядок додано." << endl;
                }
                break;
            }
            case 7:
                cout << "Ім'я файлу для збереження: ";
                cin >> buf;
                csv.writeFile(buf);
                cout << "Збережено. До побачення." << endl;
                running = false;
                break;
            default:
                cout << "Невірний код операції." << endl;
            }
        }
    }
    return 0;
}