#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <mach/platform.h>
#include <asm/uaccess.h>

#define LED_MAJOR	221
#define LED_NAME	"LED_DRIVER"
#define GPIO_SIZE	256

char led_usage=0;
static void *led_map;
volatile unsigned *led;


static int led_init(void);
static void led_exit(void);
static int led_release(struct inode*, struct file*);
static int led_open(struct inode*, struct file*);
static int led_write(struct file*, const char*, size_t, loff_t*);



static struct file_operations led_fops=
{
	.owner	=THIS_MODULE,
	.open	=led_open,
	.release=led_release,
	.write	=led_write,
};

static int led_init(void){
	int result;
	result=register_chrdev(LED_MAJOR, LED_NAME, &led_fops);
	if(result<0){
		printk(KERN_WARNING"Can't get any major!\n");
		return result;
	}
}



static void led_exit(void){
	unregister_chrdev(LED_MAJOR, LED_NAME);
	printk("LED module removed.\n");
}



module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");


static int led_open(struct inode *inode, struct file *mfile){
	int i;
	if(led_usage != 0)
		return -EBUSY;
	led_usage=1;

	led_map=ioremap(GPIO_BASE, GPIO_SIZE);
	if(!led_map){
		printk("error:mapping gpio memory");
		iounmap(led_map);
		return -EBUSY;
	}
	led=(volatile unsigned int*)led_map;

	for(i=1;i<=10;i++){
		*(led+0)&=~(0x07<<(3*i));
		*(led+0)|=(0x01<<(3*i));
		*(led+1)&=~(0x07<<(3*i));
		*(led+1)|=(0x01<<(3*i));
		*(led+2)&=~(0x07<<(3*i));
		*(led+2)|=(0x01<<(3*i));
	}

	return 0;
}

static int led_write(struct file *minode, const char *gdata, size_t length, loff_t *off_what){
	char tmp_buf;
	int result;
	result=copy_from_user(&tmp_buf, gdata, length);
	if(result<0){
		printk("Error:copy from user");
		return result;
	}
	printk("data from user: %d\n", tmp_buf);
	if(tmp_buf==0){
		*(led+7)|=0x01;
		*(led+7)|=(0x01<<1);
		*(led+7)|=(0x01<<2);
		*(led+7)|=(0x01<<3);
		*(led+7)|=(0x01<<4);
		*(led+7)|=(0x01<<5);
		*(led+7)|=(0x01<<6);
		*(led+7)|=(0x01<<7);
		*(led+7)|=(0x01<<8);
		*(led+7)|=(0x01<<9);
		*(led+7)|=(0x01<<10);
		*(led+7)|=(0x01<<11);
		*(led+7)|=(0x01<<12);
		*(led+7)|=(0x01<<13);
		*(led+7)|=(0x01<<14);
		*(led+7)|=(0x01<<15);
		*(led+7)|=(0x01<<16);
		*(led+7)|=(0x01<<17);
		*(led+7)|=(0x01<<18);
		*(led+7)|=(0x01<<19);
		*(led+7)|=(0x01<<20);
		*(led+7)|=(0x01<<21);
		*(led+7)|=(0x01<<22);
		*(led+7)|=(0x01<<23);
		*(led+7)|=(0x01<<24);
		*(led+7)|=(0x01<<25);
		*(led+7)|=(0x01<<26);
		*(led+7)|=(0x01<<27);
		*(led+7)|=(0x01<<28);
		*(led+7)|=(0x01<<29);
		*(led+7)|=(0x01<<30);
		printk("this led tmp_buf == 0");
	}
	else{
		*(led+10)|=0x01;
		*(led+10)|=(0x01<<1);
		*(led+10)|=(0x01<<2);
		*(led+10)|=(0x01<<3);
		*(led+10)|=(0x01<<4);
		*(led+10)|=(0x01<<5);
		*(led+10)|=(0x01<<6);
		*(led+10)|=(0x01<<7);
		*(led+10)|=(0x01<<8);
		*(led+10)|=(0x01<<9);
		*(led+10)|=(0x01<<10);
		*(led+10)|=(0x01<<11);
		*(led+10)|=(0x01<<12);
		*(led+10)|=(0x01<<13);
		*(led+10)|=(0x01<<14);
		*(led+10)|=(0x01<<15);
		*(led+10)|=(0x01<<16);
		*(led+10)|=(0x01<<17);
		*(led+10)|=(0x01<<18);
		*(led+10)|=(0x01<<19);
		*(led+10)|=(0x01<<20);
		*(led+10)|=(0x01<<21);
		*(led+10)|=(0x01<<22);
		*(led+10)|=(0x01<<23);
		*(led+10)|=(0x01<<24);
		*(led+10)|=(0x01<<25);
		*(led+10)|=(0x01<<26);
		*(led+10)|=(0x01<<27);
		*(led+10)|=(0x01<<28);
		*(led+10)|=(0x01<<29);
		*(led+10)|=(0x01<<30);

		printk("this led tmp_buf == 1");
	}
	return length;
}


static int led_release(struct inode *minode, struct file *mfile){
	led_usage=0;
	if(led) iounmap(led);
	return 0;
}
