#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h> 
#include <unistd.h> 
#include <ctype.h>

int main(int argc, char *argv[]) {

    int fd;
    char buf[7] = {0};

    fd = open("/dev/mydev", O_RDWR);
    if(fd < 0)
        perror("open");


    sprintf(buf, "%s",argv[1]);
    
    for(int i=0;i<6; i++){
        buf[i]=toupper(buf[i]);
    	write(fd, &buf[i], 1);
    	sleep(1);
    }
    

 

    if(fd > 0)
        close(fd);

    return 0;
}
