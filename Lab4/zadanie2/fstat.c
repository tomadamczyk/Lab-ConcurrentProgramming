#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
	int file, res;
	if(argc < 2) return 0;	

	file = open(argv[1],O_RDONLY);
 
 	if(file<0) {
 		perror("open");
 		exit(0);
 	}

    struct stat fileStat;
    if(fstat(file,&fileStat) < 0) return 1;
 	
    printf("Plik %s\n",argv[1]);
    printf("Wielkość: %d bytes\n",fileStat.st_size);
    printf("Liczba linków: %d\n",fileStat.st_nlink);
	printf("Pozwolenia: ");
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n\n");

 	return 0;
}