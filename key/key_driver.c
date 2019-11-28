#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <mach/platform.h>
#include <linux/io.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/module.h>


#define KEY_MAJOR	233
#define KEY_NAME	"KEY_DRIVER"
#define GPIO_SIZE	256
#define GPIO_OUT	17
#define GPIO_IN		27


char key_usage = 0;
static void *key_map;
volatile unsigned *key;
static char tmp_buf;
static int event_flag;
DECLARE_WAIT_QUEUE_HEAD(waitqueue);

static irqreturn_t ind_interrupt_handler(int irq, void *pdata)
{

	int tmp_key;
	tmp_key = (*(key + 13) & (1 << GPIO_IN)) == 0 ? 0 : 1;
	if (tmp_key == 0)
		++tmp_buf;


	wake_up_interruptible(&waitqueue);
	++event_flag;
	return IRQ_HANDLED;
}

static unsigned key_poll(struct file *inode, struct poll_table_struct *pt)
{
	int mask = 0;
	poll_wait(inode, &waitqueue, pt);
	if (event_flag > 0)
		mask |= (POLLIN | POLLRDNORM);
	event_flag = 0;
	return mask;
}

static int key_open(struct inode *minode, struct file *mfile)
{
	if (key_usage != 0)
		return -EBUSY;
	key_usage = 1;
	key_map = ioremap(GPIO_BASE, GPIO_SIZE);
	if (!key_map) {
		printk("error : mapping gpio memory");
		iounmap(key_map);
		return -EBUSY;
	}
	key = (volatile unsigned int*)key_map;
	*(key + 1) &= ~(0x7 << (3 * (GPIO_OUT % 10)));
	*(key + 1) |= (0x1 << (3 * (GPIO_OUT % 10)));
	*(key + 2) &= ~(0x7 << (3 * (GPIO_IN % 10)));
	*(key + 2) |= (0x0 << (3 * (GPIO_IN % 10)));
	*(key + 22) |= (0x1 << GPIO_IN);
	request_irq(gpio_to_irq(27), ind_interrupt_handler, IRQF_TRIGGER_FALLING, "irq_key", NULL);
	return 0;
}

static int key_release(struct inode *minode, struct file *mfile)
{
	key_usage = 0;
	if (key)
		iounmap(key);
	free_irq(gpio_to_irq(27), NULL);
	return 0;
}

static int key_read(struct file *inode, char *gdata, size_t length, loff_t *off_what) {
	int result;
	printk("key_read = %d\n", tmp_buf);
	result = copy_to_user(gdata, &tmp_buf, length);
	if (result < 0) {
		printk("error : copy_to_user()");
		return result;
	}
	return length;
}

static int key_write(struct file *inode, const char *gdata, size_t length, loff_t *off_what)
{


	char tmp_buf;
	int result;

	result = copy_from_user(&tmp_buf, gdata, length);
	if (result < 0)
	{
		printk("Error : copy from user");
		return result;
	}

	printk("data from user : %d\n", tmp_buf);
	if (tmp_buf == 0)
		*(key + 7) = (0x1 << GPIO_OUT);
	else
		*(key + 10) = (0x1 << GPIO_OUT);

	return length;
}




static struct file_operations key_fops =
{
	.owner = THIS_MODULE,
	.open = key_open,
	.release = key_release,
	.read = key_read,
	.write = key_write,
	.poll = key_poll,
};


static int _key_init(void)
{
	int result;
	result = register_chrdev(KEY_MAJOR, KEY_NAME, &key_fops);
	if (result < 0) {
		printk(KERN_WARNING"Can't get any major!\n");
		return result;
	}
	return 0;
}

static void key_exit(void) {
	unregister_chrdev(KEY_MAJOR, KEY_NAME);
	printk("Key module removed.\n");
}





module_init(_key_init);
module_exit(key_exit);
MODULE_LICENSE("GPL");
