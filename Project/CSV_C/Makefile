CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: test_c

test_c: CSV.c CSV_test.c
	$(CC) $(CFLAGS) -o main CSV.c CSV_test.c

clean:
	rm -f main *.o *.csv