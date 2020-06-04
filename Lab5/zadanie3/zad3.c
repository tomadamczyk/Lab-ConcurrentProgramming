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

struct section {
	int start;
	int end;
	int number;
};

int main(int argc, char *argv[])
{
	int pid, status, i, start, end, interval, N, last, startPart, endPart, res, counter = 0;
	start = atoi(argv[1]);
	end = atoi(argv[2]);
	N = atoi(argv[3]);
	interval = atoi(argv[4]);
	double partition2 = (double)(end - start)/N;
	int partition = (end - start)/N;
	time_t timeCount;
	
	if(partition2 > partition)	//sprawdzenie czy przedzial dzieli sie calkowicie
	{
		partition++;
	}

	startPart = start;
	endPart = end;

	if (mkfifo("fifo", 0666) < 0) 
	{
		perror("fifo");
		exit(0);
	}

	if (mkfifo("inputfile", 0666) < 0) 
	{
		perror("inputfile");
		exit(0);
	}

	int fd = open("fifo", O_RDWR);
	int fdInput = open("inputfile", O_RDWR);

	int quantity = endPart / interval;


	struct section s;
	for(i=0; i<quantity; i++)
	{
		start = (i*partition) + startPart;
		end = ((i+1)*partition) + startPart - 1; 
		if(end == endPart - 1)
		{
			end = endPart;
		}
		
		s.start = start;
		s.end = end;
		s.number = counter++;
		status = write(fdInput, &s, sizeof(s));
		
		if (status < 0) {
			perror("Blad zapisu");
			exit(0);
		} 	
	}

	for (i=0; i<N; i++){
		s.start = 0;
		s.end = 0;
		s.number = -1;
		write(fdInput, &s, sizeof(s));
	}

	timeCount = time(NULL);

	for(i=0; i<N; i++)
	{
		if(fork() == 0)
		{
			execl("./licz", "licz", NULL);
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
	close(fdInput);
	return 0;
}