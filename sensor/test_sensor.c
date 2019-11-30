#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/kdev_t.h>
#include <linux/poll.h>
#include <signal.h>
#include <sys/time.h>

#define	KEY_FILE_NAME	"/dev/sensor_driver"

int main()
{
    struct timeval start_time;
    struct timeval end_time;
    double durationA = 0;
    double durationB = 0;
    double durationC = 0;
    int		sensor_fd;
    char	x;
    char	flag;
    struct pollfd events[1];
    int		retval;
    int 	i = 0;
    double	r_value;
    double	b_value;
    double	g_value;
    int j = 0;

    sensor_fd = open(KEY_FILE_NAME, O_RDWR | O_NONBLOCK);
    if (sensor_fd < 0)
    {
        fprintf(stderr, "Can't open %s\n", KEY_FILE_NAME);
        return -1;
    }

    puts("program start\n");



    while(j < 10){
        for(x=1;x<=3;x++) {
            write(sensor_fd, &x, 1);
            gettimeofday(&start_time,NULL);

            while (i < 10) {

                events[0].fd = sensor_fd;
                events[0].events = POLLIN;    // waiting read

                retval = poll(events, 1, 100);        // event waiting
                if (retval < 0) {
                    fprintf(stderr, "Poll error\n");
                    exit(0);
                }

                if (events[0].revents & POLLIN) {
                    read(sensor_fd, &flag, 1);
                    i++;
                }
            }
            if(x == 1){
                gettimeofday(&end_time,NULL);

                durationA = (double)(end_time.tv_usec) - (double)(start_time.tv_usec);
                r_value += 10 / (durationA/1000000);

            }
            if(x == 2){
                gettimeofday(&end_time,NULL);

                durationB = (double)(end_time.tv_usec) - (double)(start_time.tv_usec);
                b_value += 10 / (durationB/1000000);

            }
            if(x == 3){
                gettimeofday(&end_time,NULL);

                durationC = (double)(end_time.tv_usec) - (double)(start_time.tv_usec);
                g_value += 10 / (durationC/1000000);
            }
            i = 0;

        }

        j++;

        sleep(0.1);
    }

    r_value=(r_value/10);
    b_value=(b_value/10);
    g_value=(g_value/10);

    if(11000 < r_value && 14000 > r_value && 7000 < b_value && 9000 > b_value && 6000 < g_value && 8000 > g_value){
        printf("Red");
    }
    if(10000 < r_value && 12000 > r_value && 10000 < b_value && 14000 > b_value && 7000 < g_value && 9000 > g_value){
        printf("Purple");
    }
    if(5000 < r_value && 7000 > r_value && 9000 < b_value && 13000 > b_value && 5500 < g_value && 8500 > g_value){
        printf("Blue");
    }
    if(30000 < r_value && 35000 > r_value && 16000 < b_value && 20000 > b_value && 19000 < g_value && 23000 > g_value){
        printf("Yellow");
    }
    if(18000 < r_value && 22000 > r_value && 9500 < b_value && 11500 > b_value && 9000 < g_value && 11000 > g_value){
        printf("Purple");
    }
    if(8000 < r_value && 10000 > r_value && 11000 < b_value && 14000 > b_value && 9000 < g_value && 12000 > g_value){
        printf("Purple");
    }

    close(sensor_fd);
    return 0;
}
