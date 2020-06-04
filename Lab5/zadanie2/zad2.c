#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

struct result{
	int start;
	int end;
	int howMany;
};

int main(int argc, char *argv[])
{
	int pid, status, i, start, end, N, last, fd_read, fd, res;
	start = atoi(argv[1]);
	end = atoi(argv[2]);
	N = atoi(argv[3]);
	double partition2 = (double)(end - start)/N;
	int partition = (end - start)/N;
	time_t timeCount;
	
	if(partition2 > partition)	//sprawdzenie czy przedzial dzieli sie calkowicie
	{
		partition++;
	}
	char buffer1[10], buffer2[10], buffer3[10];

	if (mkfifo("fifo", 0666) < 0) 
	{
		perror("mkfifo");
		exit(0);
	}

	fd = open("fifo", O_RDWR);

	timeCount = time(NULL);
	for(i=0; i<N; i++)
	{
		if(fork() == 0)
		{
			sprintf(buffer1, "%d", i);		//Nr procesu
			sprintf(buffer2, "%d", (i*partition)+start);
			last = ((i+1)*partition)+start - 1;
			if(last == end - 1)
			{
				last=end;
			}
			sprintf(buffer3, "%d", last);
			execl("./licz", "licz", buffer1, buffer2, buffer3, NULL);
			exit(i+1);
		}
	}

	for(i=0; i<N; i++)
	{
		pid = wait(&status);
		//printf("Proces %d zakończony status: %d\n", pid, status);
	}

	timeCount = time(NULL) - timeCount;

	int sum = 0;
	struct result r;
	for (int i = 0; i < N; i++) {
		
		res = read(fd, &r, sizeof(r));

		if(res<0)
		{
			perror("Blad odczytu");
			exit(0);
		}

		sum = sum + r.howMany;
		printf("Poczatek: %d Koniec: %d Ile liczb pierwszych: %d\n", r.start, r.end, r.howMany);
	}

	printf("\n Czas trwania obliczeń: %ld sekund \n", timeCount);
	printf("\n Znaleziono liczb pierwszych: %d \n", sum);
	close(fd);
	return 0;
}