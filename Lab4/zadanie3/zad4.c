#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

struct {
	int start;
	int end;
	int howMany;
} result;

int main(int argc, char *argv[])
{
	int pid, status, i, start, end, N, last;
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

	int file = open("wynik.bin", O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (file < 0) 
	{
		perror("Blad otwarcia");
		exit(0);
	}

	timeCount = time(NULL);
	for(i=0; i<N; i++)
	{
		if(fork() == 0)
		{
			sprintf(buffer1, "%d", i+1);		//Nr procesu
			sprintf(buffer2, "%d", (i*partition)+start);
			last = ((i+1)*partition)+start;
			if(last>end)
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
		printf("Proces %d zakończony status: %d\n", pid, status);
	}

	timeCount = time(NULL) - timeCount;

	int sum = 0;
	for (int i = 0; i < N; i++) {
		read(file, &result, sizeof(result));
		sum += result.howMany;
	}

	printf("\n Czas trwania obliczeń: %ld sekund \n", timeCount);
	printf("\n Wynik zsumowany z pliku: %d \n", sum);
	close(file);
	return 0;
}
