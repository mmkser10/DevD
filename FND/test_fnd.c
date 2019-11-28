#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/kdev_t.h>
#include <unistd.h>
#include <sys/time.h>

#define FND_FILE_NAME	"/dev/fnd_driver"// fnd device name

int main(int argc, char **argv)
{
    int fnd_fd, number;
    char fnd_values[13] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE,
                           0xE4, 0xFE, 0xF6, 0x01, 0xFF, 0x00};
    // 7-Seg LED values

    fnd_fd = open(FND_FILE_NAME, O_RDWR);	   // FND device open
    if (fnd_fd < 0)	// error handling
    {
        fprintf(stderr, "Can’t open %s\n", FND_FILE_NAME);
        return -1;
    }
    while (1)
    {
        printf("Input number : ");
        scanf("%d", &number);

        if  (number < 0 || number > 12)
            printf("Invalid number!!\n");
        else
        write(fnd_fd, &fnd_values[number], sizeof(char));
    }

    close(fnd_fd);
    return 0;
}


