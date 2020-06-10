#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <mqueue.h>

struct ms_type {
	int start;
	int end;
	int howMany;
} ms_type;


int main(int argc, char *argv[])
{
	int pid, status, i, start, end, N, last, res, sum, first;
	unsigned int prior=10;
	start = atoi(argv[1]);
	end = atoi(argv[2]);
	N = atoi(argv[3]);
	double partition2 = (double)(end - start) / N;
	int partition = (end - start) / N;
	time_t timeCount;

	if (partition2 > partition)
	{
		partition++;
	}

	//otworzenie kolejki wejściowej i wyjściowej
	mqd_t mq_in, mq_out;
	struct mq_attr attr;
	attr.mq_msgsize = sizeof(ms_type);
	attr.mq_maxmsg = 4;
	attr.mq_flags = 0;
	mq_unlink("/Wejsciowa");
	mq_unlink("/Wyjsciowa");

	mq_in = mq_open("/Wejsciowa", O_RDWR | O_CREAT, 0660, &attr);
	mq_out = mq_open("/Wyjsciowa", O_RDWR | O_CREAT, 0660, &attr);
	if (mq_in < 0 || mq_out < 0) {
		perror("Kolejka nie została utworzona");
	}

	for (i = 0; i < N; i++)
	{
		first = (i * partition) + start;
		last = ((i + 1) * partition) + start - 1;
		if (last == end - 1 || last > end)
		{
			last = end;
		}
		struct ms_type part;
		part.start = first;
		part.end = last;
		mq_send(mq_in, (const char *)&part, sizeof(part), 10);

	}

	timeCount = time(NULL);
	for (i = 0; i < N; i++)
	{
		if (fork() == 0)
		{
			execl("./licz", "licz", NULL);
			exit(i + 1);
		}
	}

	for(i=0; i<N; i++)
	{
		pid = wait(&status);
	}

	sum = 0;
	for (i = 0; i < N; i++)
	{
		res = mq_receive(mq_out, (char *)&ms_type, sizeof(ms_type), &prior);
		if (res < 0) {
			perror("rec");
		}
		sum += ms_type.howMany;
	}


	timeCount = time(NULL) - timeCount;

	printf("\n Czas trwania obliczeń: %ld sekund \n", timeCount);
	printf("\n Znaleziono liczb pierwszych: %d \n", sum);

	mq_close(mq_in);
	mq_close(mq_out);
	return 0;
}
