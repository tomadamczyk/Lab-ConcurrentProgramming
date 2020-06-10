#include <stdio.h>
#include <mqueue.h>

struct {
	int type;
	int pnr;
	char text[100];
} ms_type;

int main (int argc, char *argv[]) {
	mqd_t mq;
	struct mq_attr attr;
	attr.mq_msgsize = sizeof(ms_type);
	attr.mq_maxmsg=4;
	attr.mq_flags=0;
	mq_unlink("/Kolejka");
	mq = mq_open("/Kolejka", O_RDWR | O_CREAT, 0660, &attr);
	if (mq < 0) {
		perror("Kolejka nie została utworzona");
	}
	mq_close(mq);

	return 0;
}