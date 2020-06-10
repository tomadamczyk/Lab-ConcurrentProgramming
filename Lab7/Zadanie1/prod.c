#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define BSIZE     4   // Rozmiar bufora
#define LSIZE    80   //  Dlugosc linii

typedef struct {
    char buf[BSIZE][LSIZE];
    int head;
    int tail;
    int cnt;
    sem_t mutex;
    sem_t empty;
    sem_t full;
} bufor_t;


int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Niepoprawna liczba argumentow\n");
        return -1;
    }

    int id = atoi(argv[1]);
    int steps = atoi(argv[2]);

    int i, stat, k, pid, size, fd, res;
    bufor_t  *wbuf ;
    char c;

    // Utworzenie segmentu ---------------------------
    fd = shm_open("bufor", O_RDWR , 0774);

    if (fd == -1) {

        return (-1);
    }

    printf("fd: %d\n", fd);

    // Odwzorowanie segmentu fd w obszar pamieci procesow
    wbuf = (bufor_t *)mmap(0, sizeof(bufor_t)
                           , PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (wbuf == NULL) {
        // perror("map");
        return (-1);
    }

    for (i = 0; i < steps; i++) {

        printf("Producent %d - cnt:%d head: %d tail: %d\n",  id,
               wbuf-> cnt, wbuf->head, wbuf->tail);
        sem_wait(&(wbuf->empty));
        sem_wait(&(wbuf->mutex));
        sprintf(wbuf->buf[wbuf->head], "Producent  nr %d, krok %d", id, i);
        wbuf-> cnt ++;
        wbuf->head = (wbuf->head + 1) % BSIZE;
        sem_post(&(wbuf->mutex));
        sem_post(&(wbuf->full));
        sleep(1);
    
    }

    return 1;
}