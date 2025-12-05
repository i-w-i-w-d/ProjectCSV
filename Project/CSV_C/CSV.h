/*
 * Завдання: 16 (CSV)
 * Студент: Максим Ульянов
 * Група: 2 курс, Мехмат, Комп'ютерна математика
 * Опис: Заголовочний файл бібліотеки C.
 */

#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура таблиці CSV
typedef struct {
    char ***data;       // [рядок][стовпчик]
    int rows;
    int cols;
    char delimiter;     // Роздільник
    char **headers;     // Заголовки
    int headers_enabled;
} CSVTable;

CSVTable* csv_create(char delimiter, int has_headers);
void csv_free(CSVTable *table);

int csv_read_file(CSVTable *table, const char *filename);
int csv_write_file(CSVTable *table, const char *filename);
void csv_print_console(CSVTable *table);

char* csv_get_field(CSVTable *table, int row, int col);
char* csv_get_field_by_name(CSVTable *table, int row, const char *col_name);
int csv_set_field(CSVTable *table, int row, int col, const char *value);
int csv_delete_row(CSVTable *table, int row_idx);
int csv_add_column(CSVTable *table, const char *header_name, const char *default_val);

int csv_add_row(CSVTable *table, char **values);

#endif