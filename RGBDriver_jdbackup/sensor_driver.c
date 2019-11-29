#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <mach/platform.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/time.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/irq.h>

#define s2 17
#define s3 18
#define out 27

#define GPIO_SIZE 256
#define SENSOR_MAJOR 241
#define SENSOR_NAME "Color_Sensor"


char sensor_usage = 0;
static void *sensor_map;
char value;
volatile unsigned *color_sensor;
static int event_flag;
int d_flag;

DECLARE_WAIT_QUEUE_HEAD(waitqueue);


static irqreturn_t ind_interrupt_handler(int irq, void *pdata)
{
	value = 1;
	wake_up_interruptible(&waitqueue);
    	++event_flag;
    	return IRQ_HANDLED;
}
static unsigned sensor_poll(struct file *inode, struct poll_table_struct *pt)
{
    int mask = 0;
    poll_wait(inode, &waitqueue, pt);
    if (event_flag > 0)
        mask |= (POLLIN | POLLRDNORM);
    event_flag = 0;
    return mask;
}


//change the map name an change the usage name
static int sensor_open(struct inode *minode, struct file *mfile)
{
    if (sensor_usage != 0)
        return -EBUSY;
    sensor_usage = 1;
    sensor_map = ioremap(GPIO_BASE, GPIO_SIZE);
    if (!sensor_map)
    {
        printk("error: mapping gpio memory");
        iounmap(sensor_map);
        return -EBUSY;
    }

    color_sensor = (volatile unsigned int *)sensor_map;

    *(color_sensor + (out/10)) &= ~(0x7 << (3 * (out%10)));
    *(color_sensor + (out/10)) |= (0x0 << (3 * (out%10)));  // (sensor out pin) Input mode [read]
    *(color_sensor + 22) |= (0x1 << out);

    *(color_sensor + (s3/10)) &= ~(0x7 << (3 * (s3%10)));
    *(color_sensor + (s3/10)) |= (0x1 << (3 * (s3%10))); // (sensor s3 pin)  output mode [write]

    *(color_sensor + (s2/10)) &= ~(0x7 << (3 * (s2%10)));
    *(color_sensor + (s2/10)) |= (0x1 << (3 * (s2%10))); // (sensor s2 pin) output mode [write]
    request_irq(gpio_to_irq(27), ind_interrupt_handler, IRQF_TRIGGER_FALLING, "irq_key", NULL);

    return 0;
}


static int sensor_read(struct file *inode, char *gdata, size_t length, loff_t *off_what) {
    int result;
    //printk("sensor read\n");
    result = copy_to_user(gdata, &value, length);
    if (result < 0) {
        printk("error : copy_to_user()");
        return result;
    }
    return length;
}


static int sensor_write(struct file* mfile, const char* gdata, size_t length, loff_t* off_what) {
    int result;
    char tmp_buf;

    result = copy_from_user(&tmp_buf, gdata, length);
    if (result < 0) {
        printk("Error : copy from user");
        return result;
    }

    if (tmp_buf == 1) {
        *(color_sensor + 10) |= (0x1 << s2); // s2 L
        *(color_sensor + 10) |= (0x1 << s3); // s3 L mode red
    } 
    else if (tmp_buf == 2) {
//        *(color_sensor + 10) |= (0x1 << s2); // s2 L
        *(color_sensor + 7) |= (0x1 << s3); // s3 H mode blue
    } 
    else if (tmp_buf == 3) {
        *(color_sensor + 7) |= (0x1 << s2); // s2 H
//        *(color_sensor + 7) |= (0x1 << s3); // s3 H mode green
    }

    return length;
}

static int sensor_release(struct inode *minode, struct file *mfile)
{
    //change the usage name
    sensor_usage = 0;
    if (color_sensor)
        iounmap(color_sensor);
    free_irq(gpio_to_irq(27), NULL);
    return 0;
}

static struct file_operations sensor_fops =
{
                .owner	    = THIS_MODULE,
                .open	    = sensor_open,
                .release    = sensor_release,
                .write      = sensor_write,
                .read       = sensor_read,
                .poll       = sensor_poll,
};

static int sensor_init(void)
{
    int result;
    //change ctrcut name address
    result = register_chrdev(SENSOR_MAJOR, SENSOR_NAME, &sensor_fops);
    if (result < 0)
    {
        printk(KERN_WARNING "Can't get any major!\n");
        return result;
    }
    return 0;
}

static void sensor_exit(void)
{
    unregister_chrdev(SENSOR_MAJOR, SENSOR_NAME);
    printk("sensor module removed.\n");
}

module_init(sensor_init);
module_exit(sensor_exit);
MODULE_LICENSE("GPL");
