
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/kdev_t.h>
#include <unistd.h>
#include <sys/time.h>

#define MATRIX_FILE_NAME "/dev/matrix_driver"

int main(int argc, char **argv)
{
    int fd, data;
    int check=1;
    
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
    }

    close(fd);
    return 0;
}
