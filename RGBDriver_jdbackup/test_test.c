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


#define MOTOR180x_FILE_NAME "/dev/motx_driver"
#define MOTOR180y_FILE_NAME "/dev/moty_driver"
#define MOTOR180z_FILE_NAME "/dev/motz_driver"
#define MATRIX_FILE_NAME "/dev/mat_driver"
#define SENSOR_FILE_NAME "/dev/sensor_driver"

void *thread_motor180x(void *arg);
void *thread_motor180y(void *arg);
void *thread_motor180z(void *arg);
void *thread_sensor(void *arg);
int data_Main;

int main(int argc, char **argv)
{
    int fd, data, motx, moty, motz, sensor;
    int check=1;
    int Xangle, Yangle;

    pthread_t motor180x_id;
    pthread_t motor180y_id;
    pthread_t motor180z_id;
    pthread_t sensor_id;

    void *t_return;

    char num[9][5] = {
            0x6, 0x6, 0x6, 0x6, 0x6, //1
            0x0, 0x6, 0x0, 0x3, 0x0, //2
            0x0, 0x6, 0x0, 0x6, 0x0, //3
            0x2, 0x2, 0x0, 0x6, 0x6, //4
            0x0, 0x3, 0x0, 0x6, 0x0, //5
            0x0, 0x3, 0x0, 0x2, 0x0, //6
            0x0, 0x6, 0x6, 0x6, 0x6, //7
            0x0, 0x2, 0x0, 0x2, 0x0, //8
            0x0, 0x2, 0x0, 0x6, 0x0, //9
    };

    char color[6][5] = {
            0x1, 0x2, 0x1, 0x3, 0x2, // 빨
            0x4, 0x3, 0x2, 0x2, 0x4, // 초
            0x1, 0x2, 0x1, 0x2, 0x1, // 파
            0x2, 0x2, 0x5, 0x5, 0x5, // 노
            0x1, 0x2, 0x1, 0x3, 0x3, // 보
            0x0, 0x2, 0x2, 0x2, 0x0, // 주
    };

    fd = open(MATRIX_FILE_NAME, O_RDWR);
    if(fd < 0){
        fprintf(stderr, "Can't open %s\n",MATRIX_FILE_NAME);
        return -1;
    }

    while(check) {
        printf("Program Start..\n");

        Yangle = 1;
        moty=pthread_create(&motor180y_id, NULL, thread_motor180y, (void *)&Yangle);
        if(moty < 0){
            printf("setting motor degree create error");
            exit(1);
        }
        moty=pthread_join(motor180y_id, &t_return);
	


	sleep(3);
        sensor=pthread_create(&sensor_id, NULL, thread_sensor, NULL);
        if(sensor < 0){
            printf("sensor create error");
            exit(1);
        }
        sensor=pthread_join(sensor_id, &t_return);

	sleep(5);
        //printf("Enter \n1. Red\n2. Green\n3. Blue\n4. Yellow\n5. Purple\n6. Orange\n");
        //scanf("%d", &data_Main);

        //color sensor code...
        switch (data_Main) {

            case 1 :
                //write(fd, color[data - 1], sizeof(char));
                break;
            case 2 :
                //write(fd, color[data - 1], sizeof(char));
                break;
            case 3 :
                //write(fd, color[data - 1], sizeof(char));
                break;
            case 4 :
                //write(fd, color[data - 1], sizeof(char));
                break;
            case 5 :
                //write(fd, color[data - 1], sizeof(char));
                break;
            case 6 :
                //write(fd, color[data - 1], sizeof(char));
                break;
            default:
                check = 0;
                close(fd);
                return 0;
        }

        /* root motor code...*/
        motz=pthread_create(&motor180z_id, NULL, thread_motor180z, (void *)&data_Main);
        if(motz < 0){
            printf("motor90 degree create error");
            exit(1);
        }

        motx=pthread_create(&motor180x_id, NULL, thread_motor180x,  (void *)&data_Main);
        if(motx < 0){
            printf("motor90 degree create error");
            exit(1);
        }

        motz=pthread_join(motor180z_id, &t_return);
        motx=pthread_join(motor180x_id, &t_return);

        Yangle = 0;
        moty=pthread_create(&motor180y_id, NULL, thread_motor180y, (void *)&Yangle);
        if(moty < 0){
            printf("motor180 degree create error");
            exit(1);
        }
        moty=pthread_join(motor180y_id, &t_return);
    }

    close(fd);
    return 0;
}


void *thread_motor180x(void *arg){

    int fdX;
    char data;

    fdX=open(MOTOR180x_FILE_NAME, O_RDWR);
    if(fdX<0){
        fprintf(stderr, "Can't open %s\n", MOTOR180x_FILE_NAME);
        return -1;
    }

    data = *(char *)arg;
    write(fdX, &data, sizeof(char));

    close(fdX);
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

    sleep(4);
    close(fdY);
    return 0;
}

void *thread_motor180z(void *arg) {

    int fdZ;
    char data;

    fdZ=open(MOTOR180z_FILE_NAME, O_RDWR);
    if(fdZ<0){
        fprintf(stderr, "Can't open %s\n", MOTOR180z_FILE_NAME);
        return -1;
    }

    data = *(char *)arg;
    write(fdZ, &data, sizeof(char));

    close(fdZ);
    return 0;
}


void *thread_sensor(void *arg) {

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

    sensor_fd = open(SENSOR_FILE_NAME, O_RDWR | O_NONBLOCK);
    if (sensor_fd < 0)
    {
        fprintf(stderr, "Can't open %s\n", SENSOR_FILE_NAME);
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
