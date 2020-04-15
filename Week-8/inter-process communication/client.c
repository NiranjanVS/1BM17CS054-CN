#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#define FIFO1 "fifo1"
#define FIFO2 "fifo2"
#define PERMS 0666
char fname[256];
int main( )
 {
	int readfd, writefd, fd;
 	ssize_t n;
	char buff[512];
	writefd=open(FIFO1, O_WRONLY, 0);
    readfd =open(FIFO2, O_RDONLY, 0);
	printf("Connected..\n");
    printf("Enter the file name : ");
	scanf("%s",fname);
    write(writefd,fname,strlen(fname));
    printf("Waiting for reply from server ......\n");
    while((n=read(readfd,buff,512))>0)
    {
        write(1,buff,n);
    }   
	close(readfd); unlink(FIFO1);
	close(writefd); unlink(FIFO2);
}
