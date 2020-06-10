#include <stdio.h>
#include <mqueue.h>

#define PROD 1
#define KONS 2

struct ms_type {
	int type;
	int pnr;
	char text[100];
};

int main(int argc, char *argv[]) {
	int i, steps;
	mqd_t mq;
	struct ms_type msg;
	steps = atoi(argv[2]);
	mq = mq_open("/Kolejka", O_RDWR | O_CREAT, 0660, NULL);
	if (mq < 0) {
		perror("Błąd otwarcia kolejki!");
		exit(0);
	}

	for(i=0;i<steps;i++){
		msg.pnr=atoi(argv[1]);
		msg.type=PROD;
		sprintf(msg.text, "Producent %d krok %d\n", msg.pnr, i);
		printf("Producent %d krok %d\n", msg.pnr, i);
		mq_send(mq, (const char *)&msg, sizeof(msg), 10);
		sleep(1);
	}

	mq_close(mq);
	return 0;
}
