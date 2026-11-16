CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = -lm

all: main test_vector

vec_tr.o: vec_tr.c vec_tr.h models.h
	$(CC) $(CFLAGS) -c vec_tr.c

vec_cl.o: vec_cl.c vec_cl.h client.h vec_tr.h
	$(CC) $(CFLAGS) -c vec_cl.c

service.o: service.c service.h vec_cl.h vec_tr.h
	$(CC) $(CFLAGS) -c service.c

main.o: main.c service.h
	$(CC) $(CFLAGS) -c main.c

test_vector.o: test_vector.c vec_tr.h vec_cl.h
	$(CC) $(CFLAGS) -c test_vector.c

main: main.o service.o vec_cl.o vec_tr.o
	$(CC) $(CFLAGS) main.o service.o vec_cl.o vec_tr.o -o main $(LDFLAGS)

test_vector: test_vector.o vec_cl.o vec_tr.o
	$(CC) $(CFLAGS) test_vector.o vec_cl.o vec_tr.o -o test_vector

# Генерация тестовых данных
generate:
	python3 generate_tests.py

# Запуск тестов и демонстрации работы
test: main test_vector generate
	./test_vector
	./main test_data_50lines.txt 01.01.24 31.12.24

# Проверка на утечки памяти
valgrind: main test_vector generate
	valgrind --leak-check=full --error-exitcode=1 ./test_vector
	valgrind --leak-check=full --error-exitcode=1 ./main test_data_50lines.txt 01.01.24 31.12.24

clean:
	rm -f *.o main test_vector test_data_50lines.txt

.PHONY: all clean test generate valgrind
