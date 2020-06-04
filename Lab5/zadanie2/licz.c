#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

struct result {
	int start;
	int end;
	int howMany;
};

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

	struct result r;
	r.start = start;
	r.end = end;
	r.howMany = counter;

	int fd = open("fifo", O_RDWR);
	if (fd < 0) {
		perror("Blad otwarcia.");
		exit(0);
	}

	int status = write(fd, &r, sizeof(r));
	if (status < 0) {
		perror("Blad zapisu.");
		exit(0);
	}
	close(fd);
	
	return counter;
}