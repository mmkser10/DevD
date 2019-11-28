#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <mach/platform.h>
#include <linux/io.h>

#define FND_MAJOR	221
#define	FND_NAME	"FND_DRIVER"
#define	GPIO_SIZE	256

char fnd_usage = 0;
static void *fnd_map;
volatile unsigned *fnd;

static int fnd_open(struct inode *minode, struct file *mfile)
{
    unsigned char index;
    int i;
    if (fnd_usage != 0)
        return -EBUSY;
    fnd_usage = 1;



    fnd_map = ioremap(GPIO_BASE, GPIO_SIZE);
    if (!fnd_map)
    {
        printk("error: mapping gpio memory");
        iounmap(fnd_map);
        return -EBUSY;
    }
    fnd = (volatile unsigned int *)fnd_map;
    for (i = 0; i < 8; ++i)
    {
        *(fnd + 1) &= ~(0x07 << (3 * i));	//Clear all func from GPIO10 to GPIO 17
    *(fnd + 1) |=  (0x01 << (3 * i));	//Set output mode from GPIO10 to  17
}

    return 0;
}

static int fnd_release(struct inode *minode, struct file *mfile)
{
    fnd_usage = 0;
    if (fnd)
        iounmap(fnd);
    return 0;
}

static int fnd_write(struct file *mfile, const char *gdata, size_t length, loff_t *off_what) {
    char tmp_buf;
    int result;

    result = copy_from_user(&tmp_buf, gdata, length);
    if (result < 0) {
        printk("Error: copy from user");
        return result;
    }


    printk("fnd values = %s\n", tmp_buf);

    *(fnd + 7) = (0xFF << 10); 		// clear 7-Seg
    *(fnd + 10) = (tmp_buf << 10);		// output value to 7-Seg

    return length;
}

static struct file_operations fnd_fops =
        {
                .owner		= THIS_MODULE,
                .open		= fnd_open,
                .release	= fnd_release,
                .write		= fnd_write,
        };

static int fnd_init(void)
{
    int result;

    result = register_chrdev(FND_MAJOR, FND_NAME, &fnd_fops);
    if (result < 0)
    {
        printk(KERN_WARNING "Can't get any major number\n");
        return result;
    }
    return 0;
}

static void fnd_exit(void)
{
    unregister_chrdev(FND_MAJOR, FND_NAME);
    printk("fnd module removed.\n");
}

module_init(fnd_init);
module_exit(fnd_exit);

MODULE_LICENSE("GPL");
