#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

struct result {
	int start;
	int end;
	int howMany;
} ;

int isPrime(int n)
{
	int i,j=0;
  	for(i=2;i*i<=n;i++)
	{
    		if(n%i == 0) return(0) ;
 	}
 	return(1);
}

int main(int argc, char *argv[])
{
	int pid, status, i, start, end, N, last, sum, startPart, endPart, wr, res;
	int fd[2];
	pipe(fd);
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
	
	startPart = start;
	endPart = end;

	timeCount = time(NULL);
	for(i=0; i<N; i++)
	{	
		start = (i*partition) + startPart; 
		end = ((i+1)*partition) + startPart - 1;

		if (end == endPart - 1) {
			end = endPart;
		}

		if(fork() == 0)
		{
			close(fd[0]);
			int counter = 0, primary, j;
			struct result r;

			for (j = start; j <= end; j++) {
				
				primary = isPrime(j);

				if (primary == 1) {
					counter++;
				}
			}

			r.start = start;
			r.end = end;
			r.howMany = counter;
			wr = write(fd[1], &r, sizeof(r));
			close(fd[1]);
			exit(i);

		}
	}

	close(fd[1]);
	struct result finalResult;

	for (i=0; i<N; i++) {
		res = read(fd[0], &finalResult, sizeof(finalResult));
		if (res < 0) {
			perror("Blad odczytu!");
			exit(1);
		}
		sum = sum +finalResult.howMany;
		printf("Poczatek: %d Koniec: %d Ile liczb pierwszych: %d\n", finalResult.start, finalResult.end, finalResult.howMany);

	}

	timeCount = time(NULL) - timeCount;

	printf("\n Czas trwania obliczeń: %ld sekund \n", timeCount);
	printf("ZNaleziono %d liczb pierwszych. \n", sum);
	close(fd[0]);

	return 0;
}