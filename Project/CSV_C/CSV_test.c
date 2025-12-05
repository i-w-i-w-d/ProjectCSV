/*
 * Завдання: 16 (CSV) 
 * Студент: Максим Ульянов
 * Опис: Демонстрація роботи з CSV (автоматичний тест та інтерактивне меню).
 */

#include "CSV.h"

void print_menu() {
    printf("\n--- МЕНЮ РЕДАГУВАННЯ ---\n");
    printf("1. Показати таблицю\n");
    printf("2. Отримати значення клітинки\n");
    printf("3. Змінити значення клітинки (set_field)\n");
    printf("4. Додати новий стовпчик (add_column)\n");
    printf("5. Видалити рядок (delete_row)\n");
    printf("6. Додати новий РЯДОК (add_row)\n");
    printf("7. Зберегти у файл та вийти\n");
    printf("Ваш вибір: ");
}

int main() {
    printf("--- Тест бібліотеки CSV на C ---\n");
    int mode;
    printf("Оберіть режим (1 - Авто-тест, 2 - Інтерактивне меню): ");
    if(scanf("%d", &mode) != 1) return 0;

    if (mode == 1) {
        // --- АВТОМАТИЧНИЙ ТЕСТ ---
        FILE *f = fopen("test.csv", "w");
        fprintf(f, "Name,Age,Group\nMaxim,19,Math\nIvan,20,CS");
        fclose(f);

        CSVTable *t = csv_create(',', 1);
        if(csv_read_file(t, "test.csv")) {
            printf("Файл завантажено.\n");
            csv_print_console(t);
            csv_set_field(t, 0, 2, "MechMat");
            
            // Тест додавання рядка
            printf("Додавання рядка...\n");
            char *new_row[] = {"Petro", "21", "Physics"};
            csv_add_row(t, new_row);
            
            csv_print_console(t);
            csv_write_file(t, "result_c.csv");
            printf("Збережено у result_c.csv\n");
            csv_free(t);
        }
    } else {
        // --- ІНТЕРАКТИВНЕ МЕНЮ ---
        char filename[100];
        char delim;
        
        printf("Введіть ім'я файлу для відкриття: ");
        scanf("%s", filename);
        printf("Введіть роздільник у цьому файлі (наприклад , або ; ): ");
        scanf(" %c", &delim);

        CSVTable *t = csv_create(delim, 1);
        
        if (!csv_read_file(t, filename)) {
            printf("Увага: Файл не знайдено або порожній. Працюємо з новою таблицею.\n");
            // Можна додати стовпчики за замовчуванням, якщо ваша ласка :)
        } else {
            printf("Файл успішно завантажено!\n");
        }

        int running = 1;
        while (running) {
            print_menu();
            int action;
            if (scanf("%d", &action) != 1) break;

            int r, c;
            char buf[256];

            switch (action) {
                case 1:
                    csv_print_console(t);
                    break;
                case 2:
                    printf("Введіть номер рядка та стовпчика (наприклад: 0 1): ");
                    scanf("%d %d", &r, &c);
                    char *val = csv_get_field(t, r, c);
                    printf("Значення: %s\n", val ? val : "Помилка (поза межами)");
                    break;
                case 3: // Редагування
                    printf("Введіть Рядок Стовпчик НовеЗначення: ");
                    scanf("%d %d %s", &r, &c, buf);
                    if (csv_set_field(t, r, c, buf)) printf("Змінено.\n");
                    else printf("Помилка індексів.\n");
                    break;
                case 4: // Додавання стовпчика
                    printf("Введіть назву нового стовпчика: ");
                    scanf("%s", buf);
                    csv_add_column(t, buf, "-"); 
                    printf("Стовпчик додано.\n");
                    break;
                case 5: // Видалення рядка
                    printf("Введіть номер рядка для видалення: ");
                    scanf("%d", &r);
                    if (csv_delete_row(t, r)) printf("Рядок видалено.\n");
                    else printf("Помилка індексу.\n");
                    break;
                case 6: // Додавання рядка
                    if (t->cols == 0) {
                        printf("Помилка: Немає стовпчиків. Спочатку додайте стовпчики.\n");
                    } else {
                        printf("Введіть %d значень (по одному слову) через пробіл:\n", t->cols);
                        char **temp_row = (char**)malloc(t->cols * sizeof(char*));
                        for (int i = 0; i < t->cols; i++) {
                            if(t->headers_enabled) printf("[%s]: ", t->headers[i]);
                            else printf("Col %d: ", i);
                            scanf("%s", buf);
                            temp_row[i] = strdup(buf);
                        }
                        csv_add_row(t, temp_row);
                        printf("Рядок додано.\n");
                        
                        for(int i=0; i<t->cols; i++) free(temp_row[i]);
                        free(temp_row);
                    }
                    break;
                case 7: // Збереження
                    printf("Введіть ім'я файлу для збереження: ");
                    scanf("%s", buf);
                    printf("Введіть роздільник для збереження: ");
                    char save_delim;
                    scanf(" %c", &save_delim);
                    
                    t->delimiter = save_delim;
                    csv_write_file(t, buf);
                    printf("Файл збережено. Вихід.\n");
                    running = 0;
                    break;
                default:
                    printf("Невірний вибір.\n");
            }
        }
        csv_free(t);
    }
    return 0;
}