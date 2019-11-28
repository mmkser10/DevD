#include <linux/init.h>
#include <linux/module.h>
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
#include <linux/delay.h>

#define motorGATE_MAJOR	222
#define motorGATE_NAME	"MOTORGATE_DRIVER"
#define GPIO_SIZE	256
#define GPIO        4

char motorGATE_usage=0;
static void *motorGATE_map;
volatile unsigned *motorGATE;


static int motorGATE_init(void);
static void motorGATE_exit(void);
static int motorGATE_release(struct inode*, struct file*);
static int motorGATE_open(struct inode*, struct file*);
static int motorGATE_write(struct file*, const char*, size_t, loff_t*);



static struct file_operations motorGATE_fops=
        {
                .owner	=THIS_MODULE,
                .open	=motorGATE_open,
                .release=motorGATE_release,
                .write	=motorGATE_write,
        };

static int motorGATE_init(void){
    int result;
    result=register_chrdev(motorGATE_MAJOR, motorGATE_NAME, &motorGATE_fops);
    if(result<0){
        printk(KERN_WARNING"Can't get any major!\n");
        return result;
    }
}




static void motorGATE_exit(void){
    unregister_chrdev(motorGATE_MAJOR, motorGATE_NAME);
    printk("motorGATE module removed.\n");
}



module_init(motorGATE_init);
module_exit(motorGATE_exit);
MODULE_LICENSE("GPL");


static int motorGATE_open(struct inode *inode, struct file *mfile){
    if(motorGATE_usage != 0)
        return -EBUSY;
    motorGATE_usage=1;

    motorGATE_map=ioremap(GPIO_BASE, GPIO_SIZE);

    if(!motorGATE_map){
        printk("error:mapping gpio memory");
        iounmap(motorGATE_map);
        return -EBUSY;
    }
    motorGATE=(volatile unsigned int*)motorGATE_map;

    *(motorGATE+(GPIO/10))&=~(0x07<<(3*(GPIO%10)));
    *(motorGATE+(GPIO/10))|=(0x01<<(3*(GPIO%10)));

    return 0;
}

static int motorGATE_write(struct file *minode, const char *gdata, size_t length, loff_t *off_what){
    char tmp_buf;
    int result;
    int frequency=20000;
    int minAngle=544;
    int maxAngle=2440;
    int x;
    unsigned int setClock;
    unsigned int clrClock;


    result=copy_from_user(&tmp_buf, gdata, length);
    if(result<0){
        printk("Error:copy from user");
        return result;
    }
    printk("data from user: %d\n", tmp_buf);


    if(tmp_buf==0){
        for(x=0;x<=30;x++){
            setClock=(minAngle+((maxAngle-minAngle)/60*x));
            clrClock=frequency-setClock;
            *(motorGATE+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motorGATE+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
    }
    else{
        for(x=30;x>=0;x--){
            setClock=(minAngle+((maxAngle-minAngle)/60*x));
            clrClock=frequency-setClock;
            *(motorGATE+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motorGATE+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
    }
    return length;
}


static int motorGATE_release(struct inode *minode, struct file *mfile){
    motorGATE_usage=0;
    if(motorGATE) iounmap(motorGATE);
    return 0;
}
