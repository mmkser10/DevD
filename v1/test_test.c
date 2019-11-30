#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/kdev_t.h>
#include <unistd.h>
#include <sys/time.h>
#include <linux/poll.h>
#include <signal.h>


#define MOTOR180y_FILE_NAME "/dev/moty_driver"
#define SENSOR_FILE_NAME "/dev/sensor_driver"

void *thread_motor180y(void *arg);

int main(int argc, char **argv)
{

    struct timeval start_time;
    struct timeval end_time;
    double durationA = 0;
    double durationB = 0;
    double durationC = 0;
    int		sensor_fd, moty;
    char	x;
    char	flag;
    struct pollfd events[2];
    int		retval, Yangle;
    int 	i = 0;
    int data_Main;
    double	r_value;
    double	b_value;
    double	g_value;
    int j = 0;



    pthread_t motor180y_id;
    void *t_return;


    
    printf("Program Start..\n");

    Yangle = 1;
    moty=pthread_create(&motor180y_id, NULL, thread_motor180y, (void *)&Yangle);
    if(moty < 0){
        printf("setting motor degree create error");
        exit(1);
    }
    moty=pthread_join(motor180y_id, &t_return);



    sensor_fd = open(SENSOR_FILE_NAME, O_RDWR | O_NONBLOCK);
    if (sensor_fd < 0)
    {
        fprintf(stderr, "Can't open %s\n", SENSOR_FILE_NAME);
        return -1;
    }

    puts("program start\n");




    while(j < 10){
        for(x=1;x<=3;x++) {
            sleep(0.1);
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

    printf("Red..... %f\t", r_value);
    printf("Blue..... %f\t", b_value);
    printf("Green..... %f\t", g_value);

    if(11000 < r_value && 14000 > r_value && 7000 < b_value && 9000 > b_value && 6000 < g_value && 8000 > g_value){
        printf("Red\n");
        data_Main=1;
    }
    else if(10000 < r_value && 12000 > r_value && 10000 < b_value && 14000 > b_value && 7000 < g_value && 9000 > g_value){
        printf("Purple\n");
        data_Main=2;
    }
    else if(5000 < r_value && 7000 > r_value && 9000 < b_value && 13000 > b_value && 5500 < g_value && 8500 > g_value){
        printf("Blue\n");
        data_Main=3;
    }
    else if(30000 < r_value && 35000 > r_value && 16000 < b_value && 20000 > b_value && 19000 < g_value && 23000 > g_value){
        printf("Yellow\n");
        data_Main=4;
    }
    else if(18000 < r_value && 22000 > r_value && 9500 < b_value && 11500 > b_value && 9000 < g_value && 11000 > g_value){
        printf("Orange\n");
        data_Main=5;
    }
    else if(8000 < r_value && 10000 > r_value && 11000 < b_value && 14000 > b_value && 9000 < g_value && 12000 > g_value){
        printf("Green\n");
        data_Main=6;
    }

    close(sensor_fd);
    return 0;
}

void *thread_motor180y(void *arg) {

    int fdY;
    char data;

    fdY=open(MOTOR180y_FILE_NAME, O_RDWR);
    if(fdY<0){
        fprintf(stderr, "Can't open %s\n", MOTOR180y_FILE_NAME);
        return -1;
    }

    data = *(char *)arg;
    write(fdY, &data, sizeof(char));

    sleep(2);
    close(fdY);
    return 0;
}

