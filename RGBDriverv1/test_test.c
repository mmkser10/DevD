#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/kdev_t.h>
#include <linux/poll.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#define MOTORSENSOR_FILE_NAME   "/dev/motS_driver"
#define MOTORGATE_FILE_NAME     "/dev/motG_driver"
#define MOTORROUTE_FILE_NAME    "/dev/motR_driver"
#define MATRIX_FILE_NAME        "/dev/mat_driver"
#define SENSOR_FILE_NAME        "/dev/sensor_driver"


void *thread_motS(void *arg);
void *thread_motG(void *arg);
void *thread_motR(void *arg);
void *thread_sensor(void *arg);
static int colorInt;
static char count[6] ={0, };

int main(int argc, char **argv)
{

    pthread_t motS_id;
    pthread_t motG_id;
    pthread_t motR_id;
    pthread_t sensor_id;

    int fd, data, motS, motG, motR, sensor;
    int check=1;
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


    motS=pthread_create(&motS_id, NULL, thread_motS, NULL);
    if(motS < 0){
        printf("motorSENSOR create error");
        exit(1);
    }
    
    motS=pthread_join(motS_id, &t_return);


    sensor=pthread_create(&sensor_id, NULL, thread_sensor, NULL);
    if(sensor < 0){
        printf("motorSENSOR create error");
        exit(1);
    }

    sensor=pthread_join(sensor_id, &t_return);


    fd = open(MATRIX_FILE_NAME, O_RDWR);
    if(fd < 0){
        fprintf(stderr, "Can't open %s\n",MATRIX_FILE_NAME);
        return -1;
    }


    while(check) {
//        printf("Enter \n1. Red\n2. Green\n3. Blue\n4. Yellow\n5. Purple\n6. Orange\n");
//        scanf("%d", &data);

        switch (colorInt) {
            case 1 :
                write(fd, color[colorInt - 1], sizeof(char));
                write(fd, num[colorInt - 1], sizeof(char));
                break;
            case 2 :
                write(fd, color[colorInt - 1], sizeof(char));
                write(fd, num[colorInt - 1], sizeof(char));
                break;
            case 3 :
                write(fd, color[colorInt - 1], sizeof(char));
                write(fd, num[colorInt - 1], sizeof(char));
                break;
            case 4 :
                write(fd, color[colorInt - 1], sizeof(char));
                write(fd, num[colorInt - 1], sizeof(char));
                break;
            case 5 :
                write(fd, color[colorInt - 1], sizeof(char));
                write(fd, num[colorInt - 1], sizeof(char));
                break;
            case 6 :
                write(fd, color[colorInt - 1], sizeof(char));
                write(fd, num[colorInt - 1], sizeof(char));
                break;
            default:
                motG=pthread_create(&motG_id, NULL, thread_motG, NULL );
                if(motG < 0){
                    printf("motor180 degree create error");
                    exit(1);
                }

                motR=pthread_create(&motR_id, NULL, thread_motR, (void *)&data);
                if(motR < 0){
                    printf("motor90 degree create error");
                    exit(1);
                }
                motG=pthread_join(motG_id, &t_return);
                motR=pthread_join(motR_id, &t_return);
                close(fd);
                return 0;
        }




        motR=pthread_create(&motR_id, NULL, thread_motR, (void *)&data);
        if(motR < 0){
            printf("motor90 degree create error");
            exit(1);
        }

        motG=pthread_create(&motG_id, NULL, thread_motG, NULL );
        if(motG < 0){
            printf("motor180 degree create error");
            exit(1);
        }


        motR=pthread_join(motR_id, &t_return);
        motG=pthread_join(motG_id, &t_return);



    }

    close(fd);
    return 0;
}



void *thread_sensor(void *arg){

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



    while(j < 3){
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
                r_value = 10 / (durationA/1000000);

                if(r_value <= 0.0){
                    printf("- error : Wrong Value \n");
                    break;
                }

                printf("Red = %f ----- ", r_value);
            }
            if(x == 2){
                gettimeofday(&end_time,NULL);

                durationB = (double)(end_time.tv_usec) - (double)(start_time.tv_usec);
                b_value = 10 / (durationB/1000000);

                if(b_value <= 0.0){
                    printf("- error : Wrong Value \n");
                    break;
                }

                printf("Blue = %f ----- ", b_value);
            }
            if(x == 3){
                gettimeofday(&end_time,NULL);

                durationC = (double)(end_time.tv_usec) - (double)(start_time.tv_usec);
                g_value = 10 / (durationC/1000000);

                if(g_value <= 0.0){
                    printf("- error : Wrong Value \n");
                    break;
                }

                printf("Green = %f\n", g_value);
            }
            i = 0;
        }

        j++;

        sleep(1);
    }


    if(18000.0<=r_value<=21000.0 && 9000.0<=b_value<=11000.0 && 5000.0<=g_value<=7000.0){
        printf("IT'S 'RED'!\n");
        colorInt = 1;
        count[0] += 1;
    }
    else if(12000.0<=r_value<=15000.0 && 16000.0<=b_value<=19000.0 && 7000.0<=g_value<=9000.0){
        printf("IT'S 'PURPLE'!\n");
        colorInt = 2;
        count[1] += 1;
    }
    else if(30000.0<=r_value<=40000.0 && 13000.0<=b_value<=18000.0 && 19000.0<=g_value<=23000.0){
        printf("IT'S 'YELLOW'!\n");
        colorInt = 3;
        count[2] += 1;
    }
    else if(26000.0<=r_value<=32000.0 && 10000.0<=b_value<=13000.0 && 9000.0<=g_value<=11000.0){
        printf("IT'S 'ORANGE'!\n");
    colorInt = 4;
    count[3] += 1;
}
    else if(7000.0<=r_value<=9000.0 && 11000.0<=b_value<=14000.0 && 10000.0<=g_value<=13000.0){
        printf("IT'S 'GREEN'!\n");
        colorInt = 5;
        count[4] += 1;
    }
    else if(5000.0<=r_value<=7000.0 && 15000.0<=b_value<=17000.0 && 6000.0<=g_value<=9000.0){
        printf("IT'S 'BLUE'!\n");
        colorInt = 6;
        count[5] += 1;
    }

    else{
        printf("--Another Color--\n ");
    }

    close(sensor_fd);
    return 0;


}


//Move to Sensor using SG90 MOTOR
void *thread_motS(void *arg) {

    int fdS;
    char data;

    fdS=open(MOTORSENSOR_FILE_NAME, O_RDWR);
    if(fdS<0){
        fprintf(stderr, "Can't open %s\n", MOTORSENSOR_FILE_NAME);
        return -1;
    }

    data=0;
    write(fdS, &data, sizeof(char));
    sleep(1);


    data=1;
    write(fdS, &data, sizeof(char));
    sleep(1);

    close(fdS);
    return 0;
}


//Open support plate using SG90 MOTOR
void *thread_motG(void *arg){

    int fdG;
    char data;

    fdG=open(MOTORGATE_FILE_NAME, O_RDWR);
    if(fdG<0){
        fprintf(stderr, "Can't open %s\n", MOTORGATE_FILE_NAME);
        return -1;
    }

    data=0;
    write(fdG, &data, sizeof(char));


    data=1;
    write(fdG, &data, sizeof(char));

    close(fdG);
    return 0;
}


//Setting RGB route using SG90 MOTOR
void *thread_motR(void *arg){

    int fdR;
    char data = *(char *)arg;

    fdR=open(MOTORROUTE_FILE_NAME, O_RDWR);
    if(fdR<0){
        fprintf(stderr, "Can't open %s\n", MOTORROUTE_FILE_NAME);
        return -1;
    }

    write(fdR, &data, sizeof(char));

    close(fdR);
    return 0;
}
