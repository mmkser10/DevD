#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <mach/platform.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/time.h>
#include <linux/jiffies.h>

#define	MATRIX_MAJOR	221
#define	MATRIX_NAME	"MATRIX_DRIVER"
#define	GPIO_SIZE	256

char matrix_usage = 0;
static void *matrix_map;
volatile unsigned *matrix;

static int matrix_open(struct inode *minode, struct file *mfile)
{
    unsigned char index;

    if (matrix_usage != 0)
        return -EBUSY;
    matrix_usage = 1;



    matrix_map = ioremap(GPIO_BASE, GPIO_SIZE);
    if (!matrix_map)
    {
        printk("error: mapping gpio memory");
        iounmap(matrix_map);
        return -EBUSY;
    }
    matrix = (volatile unsigned int *)matrix_map;
    for (index = 2; index <= 9; ++index)
    {
        *(matrix) &= ~(0x07 << (3 * index));
        *(matrix) |= (0x01 << (3 * index));
    }
    return 0;
}

static int matrix_release(struct inode *minode, struct file *mfile)
{
    matrix_usage = 0;
    if (matrix)
        iounmap(matrix);
    return 0;
}

static int matrix_write(struct file *mfile, const char *gdata, size_t length, loff_t *off_what) {
    char tmp_buf[5];
    char val[5];
    int result, i;
    unsigned long delay = jiffies + 1*HZ;

    result = copy_from_user(&tmp_buf, gdata, length);
    if (result < 0) {
        printk("Error: copy from user");
        return result;
    }

    for (i = 0; i < 5; i++)
        val[i] = gdata[i];

    printk("matrix values = %s\n", tmp_buf);

    while(time_before(jiffies, delay)) {
        for (i = 0; i < 5; i++) {
            *(matrix + 7) = (val[i] << 2);
            *(matrix + 7) = (0x1 << (i + 5));
            udelay(1);
            *(matrix + 10) = (0xFF << 2);
        }
    }
    return length;
}

static struct file_operations matrix_fops =
        {
                .owner		= THIS_MODULE,
                .open		= matrix_open,
                .release	= matrix_release,
                .write		= matrix_write,
        };

static int matrix_init(void)
{
    int result;

    result = register_chrdev(MATRIX_MAJOR, MATRIX_NAME, &matrix_fops);
    if (result < 0)
    {
        printk(KERN_WARNING "Can't get any major number\n");
        return result;
    }
    return 0;
}

static void matrix_exit(void)
{
    *(matrix + 10) = (0xFF << 2);	// clear matrix
    unregister_chrdev(MATRIX_MAJOR, MATRIX_NAME);
    printk("MATRIX module removed.\n");
}

module_init(matrix_init);
module_exit(matrix_exit);

MODULE_LICENSE("GPL");
