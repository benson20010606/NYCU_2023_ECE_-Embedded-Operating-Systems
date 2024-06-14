#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h> // for open
#include <unistd.h> // for close


int main(int argc, char *argv[]) {

    int fd;
    char buf[10] = {0};

    fd = open("/dev/etx_device", O_RDWR);
    if(fd < 0)
        perror("open");


    sprintf(buf, argv[1]);
    write(fd, buf, 10);

 

    if(fd > 0)
        close(fd);

    return 0;
}
