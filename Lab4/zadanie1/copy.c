#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#define SIZE 512

int main(int argc, char *argv[]) {
	int file1, file2, rd, wrt;
	char buf[SIZE];
	
	if(argc < 3) return 0;	

	file1 = open(argv[1],O_RDONLY);
 
 	if(file1<0) {
 		perror("open");
 		exit(0);
 	}

 	file2 = open(argv[2], O_RDWR | O_CREAT, 0666);

 	if(file2<0) {
 		perror("open");
 		exit(0);
 	}
 
 	do {
 		
 		rd = read(file1, buf, SIZE);
		wrt = write(file2, buf, rd);
 		
 		} while(rd == SIZE);
 	
 	close(file1);
 	close(file2);
 	return 0;
}
