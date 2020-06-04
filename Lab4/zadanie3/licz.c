#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

struct {
	int start;
	int end;
	int howMany;
} result;

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
	int primary;
	int i, j, start, end, counter=0;
	int file;
	start = atoi(argv[2]);
	end = atoi(argv[3]);
	for(i=start; i<=end; i++)
	{
		primary = isPrime(i);

		if(primary == 1)
		{
			counter++;
		}
	}

	result.start = start;
	result.end = end;
	result.howMany = counter;

	file = open("wynik.bin", O_RDWR | O_CREAT | O_APPEND, 0666);
	if (file < 0){
		perror("Blad otwarcia");
		exit(0);
	}

	int res = lockf(file, F_LOCK, 0);
	write(file, &result, sizeof(result));
	res = lockf(file, F_ULOCK, 0);
	close(file);

	printf("Proces %d - %d liczb pierwszych\n", atoi(argv[1]), counter);
	
	return counter;


}