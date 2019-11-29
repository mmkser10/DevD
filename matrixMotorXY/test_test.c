#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/kdev_t.h>
#include <unistd.h>
#include <sys/time.h>

#define MOTOR180x_FILE_NAME "/dev/motx_driver"
#define MOTOR180y_FILE_NAME "/dev/moty_driver"
#define MATRIX_FILE_NAME "/dev/mat_driver"


void *thread_motor180x(void *arg);
void *thread_motor180y(void *arg);

int main(int argc, char **argv)
{
    int fd, data, motx, moty;
    int check=1;
    int Xangle, Yangle;

    pthread_t motor180x_id;
    pthread_t motor180y_id;
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
	Yangle = 1;
        moty=pthread_create(&motor180y_id, NULL, thread_motor180y, (void *)&Yangle);
        if(moty < 0){
            printf("motor180 degree create error");
            exit(1);
        }
        moty=pthread_join(motor180y_id, &t_return);
	/* color sensor code...
	 *
	 * root motor code...*/
        motx=pthread_create(&motor180x_id, NULL, thread_motor180x, NULL);
        if(motx < 0){
            printf("motor90 degree create error");
            exit(1);
        }
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

    data = 2;
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
	
    close(fdY);
    return 0;
}
