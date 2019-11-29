#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/kdev_t.h>
#include <unistd.h>
#include <sys/time.h>

#define MOTORSENSOR_FILE_NAME   "/dev/motS_driver"
#define MOTORGATE_FILE_NAME     "/dev/motG_driver"
#define MOTORROUTE_FILE_NAME    "/dev/motR_driver"
#define MATRIX_FILE_NAME        "/dev/mat_driver"


void *thread_motS(void *arg);
void *thread_motG(void *arg);
void *thread_motR(void *arg);

int main(int argc, char **argv)
{

    pthread_t motS_id;
    pthread_t motG_id;
    pthread_t motR_id;

    int fd, data, motS, motG, motR;
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



    fd = open(MATRIX_FILE_NAME, O_RDWR);
    if(fd < 0){
        fprintf(stderr, "Can't open %s\n",MATRIX_FILE_NAME);
        return -1;
    }


    while(check) {
        printf("Enter \n1. Red\n2. Green\n3. Blue\n4. Yellow\n5. Purple\n6. Orange\n");
        scanf("%d", &data);

        switch (data) {
            case 1 :
                write(fd, color[data - 1], sizeof(char));
                break;
            case 2 :
                write(fd, color[data - 1], sizeof(char));
                break;
            case 3 :
                write(fd, color[data - 1], sizeof(char));
                break;
            case 4 :
                write(fd, color[data - 1], sizeof(char));
                break;
            case 5 :
                write(fd, color[data - 1], sizeof(char));
                break;
            case 6 :
                write(fd, color[data - 1], sizeof(char));
                break;
            default:
                check = 0;
        }



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



    }

    close(fd);
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
