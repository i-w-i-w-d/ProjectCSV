/*
 * Реалізація функцій бібліотеки CSV на C.
 */

#include "CSV.h"

CSVTable* csv_create(char delimiter, int has_headers) {
    CSVTable *t = (CSVTable*)malloc(sizeof(CSVTable));
    t->data = NULL;
    t->rows = 0;
    t->cols = 0;
    t->delimiter = delimiter;
    t->headers = NULL;
    t->headers_enabled = has_headers;
    return t;
}

void csv_free(CSVTable *table) {
    if (!table) return;
    for (int i = 0; i < table->rows; i++) {
        for (int j = 0; j < table->cols; j++) free(table->data[i][j]);
        free(table->data[i]);
    }
    free(table->data);
    if (table->headers) {
        for (int j = 0; j < table->cols; j++) free(table->headers[j]);
        free(table->headers);
    }
    free(table);
}

static char** parse_line(const char *line, char delimiter, int *out_count) {
    int capacity = 10, count = 0;
    char **tokens = (char**)malloc(capacity * sizeof(char*));
    const char *ptr = line;
    const char *start = line;
    
    while (*ptr) {
        if (*ptr == delimiter || *ptr == '\n' || *ptr == '\r') {
            int len = ptr - start;
            char *token = (char*)malloc(len + 1);
            strncpy(token, start, len);
            token[len] = '\0';
            
            if (count >= capacity) {
                capacity *= 2;
                tokens = (char**)realloc(tokens, capacity * sizeof(char*));
            }
            tokens[count++] = token;
            if (*ptr == '\n' || *ptr == '\r') break;
            start = ptr + 1;
        }
        ptr++;
    }
    if (start <= ptr && *ptr == '\0' && start != ptr) {
         int len = ptr - start;
         char *token = (char*)malloc(len + 1);
         strncpy(token, start, len);
         token[len] = '\0';
         if (count >= capacity) tokens = (char**)realloc(tokens, (capacity+1) * sizeof(char*));
         tokens[count++] = token;
    }
    *out_count = count;
    return tokens;
}

int csv_read_file(CSVTable *table, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;
    char buffer[1024];
    int first_line = 1;

    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\n")] = 0;
        int count = 0;
        char **tokens = parse_line(buffer, table->delimiter, &count);

        if (first_line && table->headers_enabled) {
            table->headers = tokens;
            table->cols = count;
            first_line = 0;
        } else {
            if (table->rows == 0 && !table->headers_enabled) table->cols = count;
            table->data = (char***)realloc(table->data, (table->rows + 1) * sizeof(char**));
            table->data[table->rows] = tokens;
            table->rows++;
        }
    }
    fclose(fp);
    return 1;
}

int csv_write_file(CSVTable *table, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return 0;
    if (table->headers_enabled && table->headers) {
        for (int j = 0; j < table->cols; j++) 
            fprintf(fp, "%s%c", table->headers[j], (j == table->cols - 1) ? '\n' : table->delimiter);
    }
    for (int i = 0; i < table->rows; i++) {
        for (int j = 0; j < table->cols; j++) 
            fprintf(fp, "%s%c", table->data[i][j], (j == table->cols - 1) ? '\n' : table->delimiter);
    }
    fclose(fp);
    return 1;
}

void csv_print_console(CSVTable *table) {
    printf("--- CSV TABLE ---\n");
    if (table->headers_enabled && table->headers) {
        for (int j = 0; j < table->cols; j++) printf("[%s]\t", table->headers[j]);
        printf("\n");
    }
    for (int i = 0; i < table->rows; i++) {
        for (int j = 0; j < table->cols; j++) printf("%s\t", table->data[i][j]);
        printf("\n");
    }
}

char* csv_get_field(CSVTable *table, int row, int col) {
    if (row < 0 || row >= table->rows || col < 0 || col >= table->cols) return NULL;
    return table->data[row][col];
}

char* csv_get_field_by_name(CSVTable *table, int row, const char *col_name) {
    if (!table->headers_enabled) return NULL;
    for (int j = 0; j < table->cols; j++) {
        if (strcmp(table->headers[j], col_name) == 0) return csv_get_field(table, row, j);
    }
    return NULL;
}

int csv_set_field(CSVTable *table, int row, int col, const char *value) {
    if (row < 0 || row >= table->rows || col < 0 || col >= table->cols) return 0;
    free(table->data[row][col]);
    table->data[row][col] = strdup(value);
    return 1;
}

int csv_delete_row(CSVTable *table, int row_idx) {
    if (row_idx < 0 || row_idx >= table->rows) return 0;
    for (int j = 0; j < table->cols; j++) free(table->data[row_idx][j]);
    free(table->data[row_idx]);
    for (int i = row_idx; i < table->rows - 1; i++) table->data[i] = table->data[i + 1];
    table->rows--;
    table->data = (char***)realloc(table->data, table->rows * sizeof(char**));
    return 1;
}

int csv_add_column(CSVTable *table, const char *header_name, const char *default_val) {
    if (table->headers_enabled) {
        table->headers = (char**)realloc(table->headers, (table->cols + 1) * sizeof(char*));
        table->headers[table->cols] = strdup(header_name);
    }
    for (int i = 0; i < table->rows; i++) {
        table->data[i] = (char**)realloc(table->data[i], (table->cols + 1) * sizeof(char*));
        table->data[i][table->cols] = strdup(default_val);
    }
    table->cols++;
    return 1;
}

int csv_add_row(CSVTable *table, char **values) {
    if (!table) return 0;
    
    table->data = (char***)realloc(table->data, (table->rows + 1) * sizeof(char**));
    table->data[table->rows] = (char**)malloc(table->cols * sizeof(char*));
    
    for (int j = 0; j < table->cols; j++) {
        if (values && values[j]) {
            table->data[table->rows][j] = strdup(values[j]);
        } else {
            table->data[table->rows][j] = strdup("");
        }
    }
    
    table->rows++;
    return 1;
}