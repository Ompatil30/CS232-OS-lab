
virtmem: main.o page_table.o disk.o
	gcc main.o page_table.o disk.o -o virtmem

main.o: main.c
	gcc -Wall -g -c main.c -o main.o

page_table.o: page_table.c
	gcc -Wall -g -c page_table.c -o page_table.o

disk.o: disk.c
	gcc -Wall -g -c disk.c -o disk.o

clean:
	rm -f *.o virtmem
