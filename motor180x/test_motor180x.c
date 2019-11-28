#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#define MOTOR180x_FILE_NAME "/dev/motor180x_driver"

int main (int argc, char **argv)
{
    int motor_fd;
    char data;

    motor_fd=open(MOTOR180x_FILE_NAME, O_RDWR);
    if(motor_fd<0){
        fprintf(stderr, "Can't open %s\n", MOTOR180x_FILE_NAME);
        return -1;
    }


    while(1){
        data=0;
        write(motor_fd, &data, sizeof(char));
	sleep(1);


	data=1;
	write(motor_fd, &data, sizeof(char));
	sleep(1);
    }
    close(motor_fd);
    return 0;
}
