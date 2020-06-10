#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>

struct {
	int type;
	int pnr;
	char text[100];
} ms_type;

int main(int argc, char *argv[]) {
	int i, steps, res;
	mqd_t mq;
	unsigned int prior;
	steps = atoi(argv[1]);

	mq = mq_open("/Kolejka", O_RDWR, 0660, NULL);
	if (mq < 0) {
		perror("Błąd otwarcia kolejki!");
		exit(0);
	}

	for (i=0; i<steps; i++) {
		res = mq_receive(mq, (char *)&ms_type, sizeof(ms_type), &prior);

		if (res < 0) {
			perror("Błąd przy odbiorze!");
		}

		printf("%s Odbior krok: %d \n", ms_type.text, i+1);
	}
	mq_close(mq);
}