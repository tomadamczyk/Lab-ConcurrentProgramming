#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <mqueue.h>

struct ms_type {
	int start;
	int end;
	int howMany;
} ms_type;

int isPrime(int n)
{
	int i, j = 0;
	for (i = 2; i * i <= n; i++)
	{
		if (n % i == 0) return (0) ;
	}
	return (1);
}

int main(int argc, char *argv[])
{
	int primary;
	int i, j, start, end, res, counter = 0;

	mqd_t mq_in, mq_out;
	struct mq_attr attr;
	unsigned int prior = 10;

	mq_in = mq_open("/Wejsciowa", O_RDWR, 0660, NULL);
	mq_out = mq_open("/Wyjsciowa", O_RDWR, 0660, NULL);

	if (mq_in < 0 || mq_out < 0) {
		perror("Kolejka nie została utworzona");
	}

	struct ms_type part;

	res = mq_receive(mq_in, (char *)&ms_type, sizeof(ms_type), &prior);
	if (res < 0) {
		perror("Błąd!");
	}
	for (i = ms_type.start; i <= ms_type.end; i++)
	{
		primary = isPrime(i);

		if (primary == 1)
		{
			counter++;
		}
	}
	printf("Pocz %d kon %d wynik %d\n", ms_type.start, ms_type.end, counter);

	part.start = ms_type.start;
	part.end = ms_type.end;
	part.howMany = counter;
	mq_send(mq_out, (const char *)&part, sizeof(part), 10);
	
	return 0;
}