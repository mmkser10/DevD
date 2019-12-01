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


pthread_mutex_t mutex;

void *thread_motor180x(void *arg);
void *thread_motor180y(void *arg);
void *thread_motor180z(void *arg);
void *thread_matrixColor(void *arg);
void *thread_matrixCount(void *arg);
void *thread_sensor(void *arg);



int d_Main;
int count[6] = {0, };


int main(int argc, char **argv)
{
    int fd, data, motx, moty, motz, sensor, matColor, matCount;
    int check=1;
    int Xangle, Yangle;

    pthread_t motor180x_id;
    pthread_t motor180y_id;
    pthread_t motor180z_id;
    pthread_t sensor_id;
    pthread_t matrixColor_id;
    pthread_t matrixCount_id;
    void *t_return;



    pthread_mutex_init(&mutex, NULL);


    while(check) {

        printf("Sensor start..\n");
        Yangle = 1;
        moty=pthread_create(&motor180y_id, NULL, thread_motor180y, (void *)&Yangle);
        if(moty < 0){
            printf("setting motor degree create error");
            exit(1);
        }
        moty=pthread_join(motor180y_id, &t_return);




        sleep(0.5);
        sensor=pthread_create(&sensor_id, NULL, thread_sensor, NULL);
        if(sensor < 0){
            printf("sensor create error");
            exit(1);
        }
        sensor=pthread_join(sensor_id, &t_return);

        sleep(1);
        printf("d_main %d\n", d_Main);

/*
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
        }*/

        printf("Enter \n1. Red\n2. Green\n3. Blue\n4. Yellow\n5. Purple\n6. Orange\n");
        scanf("%d", &d_Main);

        //color sensor code...
        switch (d_Main) {
            case 1 :
                matColor=pthread_create(&matrixColor_id, NULL, thread_matrixColor, (void *)&d_Main);
                if(matColor < 0){
                    printf("Matrix color create error");
                    exit(1);
                }
                matColor=pthread_join(matrixColor_id, &t_return);
                count[d_Main-1]++;

                matCount=pthread_create(&matrixCount_id, NULL, thread_matrixCount, (void *)&count[d_Main-1]);
                if(matColor < 0){
                    printf("Matrix count create error");
                    exit(1);
                }
                matCount=pthread_join(matrixCount_id, &t_return);
                break;
            case 2 :
                matColor=pthread_create(&matrixColor_id, NULL, thread_matrixColor, (void *)&d_Main);
                if(matColor < 0){
                    printf("Matrix color create error");
                    exit(1);
                }
                matColor=pthread_join(matrixColor_id, &t_return);
                count[d_Main-1]++;

                matCount=pthread_create(&matrixCount_id, NULL, thread_matrixCount, (void *)&count[d_Main-1]);
                if(matColor < 0){
                    printf("Matrix count create error");
                    exit(1);
                }
                matCount=pthread_join(matrixCount_id, &t_return);
                break;
            case 3 :
                matColor=pthread_create(&matrixColor_id, NULL, thread_matrixColor, (void *)&d_Main);
                if(matColor < 0){
                    printf("Matrix color create error");
                    exit(1);
                }
                matColor=pthread_join(matrixColor_id, &t_return);
                count[d_Main-1]++;

                matCount=pthread_create(&matrixCount_id, NULL, thread_matrixCount, (void *)&count[d_Main-1]);
                if(matColor < 0){
                    printf("Matrix count create error");
                    exit(1);
                }
                matCount=pthread_join(matrixCount_id, &t_return);
                break;
            case 4 :
                matColor=pthread_create(&matrixColor_id, NULL, thread_matrixColor, (void *)&d_Main);
                if(matColor < 0){
                    printf("Matrix color create error");
                    exit(1);
                }
                matColor=pthread_join(matrixColor_id, &t_return);
                count[d_Main-1]++;

                matCount=pthread_create(&matrixCount_id, NULL, thread_matrixCount, (void *)&count[d_Main-1]);
                if(matColor < 0){
                    printf("Matrix count create error");
                    exit(1);
                }
                matCount=pthread_join(matrixCount_id, &t_return);
                break;
            case 5 :
                matColor=pthread_create(&matrixColor_id, NULL, thread_matrixColor, (void *)&d_Main);
                if(matColor < 0){
                    printf("Matrix color create error");
                    exit(1);
                }
                matColor=pthread_join(matrixColor_id, &t_return);
                count[d_Main-1]++;

                matCount=pthread_create(&matrixCount_id, NULL, thread_matrixCount, (void *)&count[d_Main-1]);
                if(matColor < 0){
                    printf("Matrix count create error");
                    exit(1);
                }
                matCount=pthread_join(matrixCount_id, &t_return);
                break;
            case 6 :
                matColor=pthread_create(&matrixColor_id, NULL, thread_matrixColor, (void *)&d_Main);
                if(matColor < 0){
                    printf("Matrix color create error");
                    exit(1);
                }
                matColor=pthread_join(matrixColor_id, &t_return);
                count[d_Main-1]++;

                matCount=pthread_create(&matrixCount_id, NULL, thread_matrixCount, (void *)&count[d_Main-1]);
                if(matColor < 0){
                    printf("Matrix count create error");
                    exit(1);
                }
                matCount=pthread_join(matrixCount_id, &t_return);
                break;
            default:
                check = 0;
                close(fd);
                return 0;
        }

        motz=pthread_create(&motor180z_id, NULL, thread_motor180z, (void *)&d_Main);
        if(motz < 0){
            printf("motorz degree create error");
            exit(1);
        }

        motx=pthread_create(&motor180x_id, NULL, thread_motor180x,  (void *)&d_Main);
        if(motx < 0){
            printf("motorx degree create error");
            exit(1);
        }

        motz=pthread_join(motor180z_id, &t_return);
        motx=pthread_join(motor180x_id, &t_return);

        Yangle = 0;
        moty=pthread_create(&motor180y_id, NULL, thread_motor180y, (void *)&Yangle);
        if(moty < 0){
            printf("motory degree create error");
            exit(1);
        }
        moty=pthread_join(motor180y_id, &t_return);

    }

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

    sleep(2);
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


    int		sensor_fd[3];
    char	x;
    char	flag;
    struct pollfd events[1];
    int		retval;
    int 	i = 0;
    int j = 0;


    double durationA = 0;
    double durationB = 0;
    double durationC = 0;


    double	r_value=0;
    double	b_value=0;
    double	g_value=0;



    while(j < 10){
        printf("j = %d\n", j);
        for(x=1;x<=3;x++) {
            sensor_fd[x-1] = open(SENSOR_FILE_NAME, O_RDWR | O_NONBLOCK);
            if (sensor_fd[x-1] < 0)
            {
                fprintf(stderr, "Can't open %s\n", SENSOR_FILE_NAME);
                return -1;
            }

            puts("program start\n");
            printf("x = %d\n", x);
            write(sensor_fd[x-1], &x, 1);
            gettimeofday(&start_time,NULL);

            while (i < 10) {
                events[0].fd = sensor_fd[x-1];
                events[0].events = POLLIN;    // waiting read

                retval = poll(events, 1, 100);        // event waiting
                if (retval < 0) {
                    fprintf(stderr, "Poll error\n");
                    exit(0);
                }

                if (events[0].revents & POLLIN) {
                    read(sensor_fd[x-1], &flag, 1);
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

            if(j != 9){
                close(sensor_fd[x-1]);
                printf("Close activate\n");
            }
            else{

                r_value=(r_value/10);
                b_value=(b_value/10);
                g_value=(g_value/10);

                printf("Red..... %f\t", r_value);
                printf("Blue..... %f\t", b_value);
                printf("Green..... %f\n", g_value);


                d_Main += 1;

                close(sensor_fd[0]);
                close(sensor_fd[1]);
                close(sensor_fd[2]);
                pthread_exit(0);
            }
        }
        j++;
        sleep(0.1);


    }
}


void *thread_matrixColor(void *arg) {
    int fdM_Color;
    char data;

    char color[6][5] = {
            0x1, 0x2, 0x1, 0x3, 0x2, // 빨
            0x4, 0x3, 0x2, 0x2, 0x4, // 초
            0x1, 0x2, 0x1, 0x2, 0x1, // 파
            0x2, 0x2, 0x5, 0x5, 0x5, // 노
            0x1, 0x2, 0x1, 0x3, 0x3, // 보
            0x0, 0x2, 0x2, 0x2, 0x0, // 주
    };


    fdM_Color = open(MATRIX_FILE_NAME, O_RDWR);
    if(fdM_Color < 0){
        fprintf(stderr, "Can't open %s\n",MATRIX_FILE_NAME);
        return -1;
    }


    data = *(char *)arg;
    printf("fdM_Color write = %d\n", data);
    write(fdM_Color, color[data - 1], sizeof(char));

    close(fdM_Color);
    return 0;

}


void *thread_matrixCount(void *arg) {
    int fdM_Count;
    char data;

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


    fdM_Count = open(MATRIX_FILE_NAME, O_RDWR);
    if(fdM_Count < 0){
        fprintf(stderr, "Can't open %s\n",MATRIX_FILE_NAME);
        return -1;
    }


    data = *(char *)arg;
    printf("fdM_Count write = %d\n", data);
    write(fdM_Count, num[data - 1], sizeof(char));

    close(fdM_Count);
    return 0;

}
