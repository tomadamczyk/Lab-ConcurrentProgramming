#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define SIZE 60 //rozmiar bufora


typedef struct {
	int start;
	int end ;
	int sum;
} data_t;

typedef struct {
	int typ;
	data_t dane[SIZE];
} buf_t;

int pierwsza(int n) {	
	int i, j = 0;
	for (i = 2; i * i <= n; i++) {
		if (n % i == 0) return (0) ;
	}
	return (1);
}

int main(int argc, char *argv[]) {
	int i, j, status, k, pid, size, fd, res, nr, counter;
	int start = atoi(argv[1]);
	int end = atoi(argv[2]);
	int N = atoi(argv[3]);
	data_t tablica[SIZE];

	double partition2 = (double)(end - start) / N;
	int partition = (end - start) / N;

	if (partition2 > partition)
	{
		partition++;
	}

	buf_t *buf; // zmienna wsaznikowa
	int  fdd; // deskryptor segmentu

	shm_unlink("bufor");

	fd = shm_open("bufor", O_RDWR | O_CREAT , 0774);
	if (fd == -1) {
		perror("open"); 
		_exit(-1);
	}

	printf("fd: %d\n", fd);

	size = ftruncate(fd, sizeof(buf_t));
	if (size < 0) {
		perror("trunc");
		_exit(-1);
	}

	//Odwzorowanie segmentu fd w obszar pamieci procesow
	buf = (buf_t*)mmap(0, sizeof(buf_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (buf == NULL) {
		perror("map"); _exit(-1);
	}

	int start2 = 0, end2 = 0;
	int quantity = end / partition;
	for (i = 0; i < quantity; i++)
	{
		start2 = (i * partition) + start;
		end2 = ((i + 1) * partition) + start - 1;
		if (end > end2) {end = end2;}
		tablica[i].start = start2;
		tablica[i].end = end2;
		tablica[i].sum = i;
	}

	counter = 0; int sum = 0; int ilosc2 = quantity;
	for (i = 0; i < quantity; i++)
	{
		nr = j;
		if (fork() == 0) {

			int start = tablica[i].start;
			int end = tablica[i].end;
			int pom = tablica[i].sum;
			for (j = start; j < end; j++)
				if (pierwsza(j) == 1) sum++;
			buf->dane[i].start = start;
			buf->dane[i].end = end;
			buf->dane[i].sum = sum;

			printf("Poczatek: %d end: %d sum: %d \n", start, end, sum);
			exit(1);
		}
	}

	for (k = 0; k < quantity; k++)
	{
		wait(&status);
		status = WEXITSTATUS(status);
	}

	int sumAll = 0;
	for (i = 0; i < quantity; i++)
	{
		sumAll = sumAll + buf->dane[i].sum;
		printf("w procesie numer: %d bylo %d \n", i, buf->dane[i].sum);
	}
	printf ("sums calkowita: %d \n", sumAll);
}
