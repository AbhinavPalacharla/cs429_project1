CC = gcc
CFLAGS = -g -W -Wall -std=c99

a.out: main.o operation.o cache.o k_way_cache.o fully_assoc_cache.o
	$(CC) $(CFLAGS) -o a.out main.o operation.o cache.o k_way_cache.o fully_assoc_cache.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

operation.o: operation.c
	$(CC) $(CFLAGS) -c operation.c

cache.o: cache.c
	$(CC) $(CFLAGS) -c cache.c

k_way_cache.o: k_way_cache.c
	$(CC) $(CFLAGS) -c k_way_cache.c

fully_assoc_cache.o: fully_assoc_cache.c
	$(CC) $(CFLAGS) -c fully_assoc_cache.c

clean:
	rm -f *.o a.out

run:
	python3 driver.py