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


struct section {
	int start;
	int end;
	int number;
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
	int i, j, start, end, counter=0, res;
	struct section s;

	int fd = open("fifo", O_RDWR);
	int fdInput = open("inputfile", O_RDWR);

	if (fd < 0 || fdInput < 0) {
		perror("Blad otwarcia.");
		exit(0);
	}

	do {
		res = read(fdInput, &s, sizeof(s));
		if (res < 0) {
			exit(0);
		}

		if (s.number == -1) {
			exit(0);
		}

		for(i=start; i<=end; i++) {
			primary = isPrime(i);

			if(primary == 1) {
				counter++;
			}
		}

		struct result r;
		r.start = start;
		r.end = end;
		r.howMany = counter;

		int status = write(fd, &r, sizeof(r));
		if (status < 0) {
			perror("Blad zapisu.");
			exit(0);
		}

	} while(s.number!=-1);

	close(fd);
	close(fdInput);	
	return counter;
}