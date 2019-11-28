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

#define motorSENSOR_MAJOR	21
#define motorSENSOR_NAME	"MOTORSENSOR_DRIVER"
#define GPIO_SIZE	256
#define GPIO        3

char motorSENSOR_usage=0;
static void *motorSENSOR_map;
volatile unsigned *motorSENSOR;


static int motorSENSOR_init(void);
static void motorSENSOR_exit(void);
static int motorSENSOR_release(struct inode*, struct file*);
static int motorSENSOR_open(struct inode*, struct file*);
static int motorSENSOR_write(struct file*, const char*, size_t, loff_t*);



static struct file_operations motorSENSOR_fops=
        {
                .owner	=THIS_MODULE,
                .open	=motorSENSOR_open,
                .release=motorSENSOR_release,
                .write	=motorSENSOR_write,
        };

static int motorSENSOR_init(void){
    int result;
    result=register_chrdev(motorSENSOR_MAJOR, motorSENSOR_NAME, &motorSENSOR_fops);
    if(result<0){
        printk(KERN_WARNING"Can't get any major!\n");
        return result;
    }
}




static void motorSENSOR_exit(void){
    unregister_chrdev(motorSENSOR_MAJOR, motorSENSOR_NAME);
    printk("motorSENSOR module removed.\n");
}



module_init(motorSENSOR_init);
module_exit(motorSENSOR_exit);
MODULE_LICENSE("GPL");


static int motorSENSOR_open(struct inode *inode, struct file *mfile){
    if(motorSENSOR_usage != 0)
        return -EBUSY;
    motorSENSOR_usage=1;

    motorSENSOR_map=ioremap(GPIO_BASE, GPIO_SIZE);

    if(!motorSENSOR_map){
        printk("error:mapping gpio memory");
        iounmap(motorSENSOR_map);
        return -EBUSY;
    }
    motorSENSOR=(volatile unsigned int*)motorSENSOR_map;

    *(motorSENSOR+(GPIO/10))&=~(0x07<<(3*(GPIO%10)));
    *(motorSENSOR+(GPIO/10))|=(0x01<<(3*(GPIO%10)));

    return 0;
}

static int motorSENSOR_write(struct file *minode, const char *gdata, size_t length, loff_t *off_what){
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
        for(x=30;x<=60;x++){
            setClock=(minAngle+((maxAngle-minAngle)/60*x));
            clrClock=frequency-setClock;
            *(motorSENSOR+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motorSENSOR+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
    }
    else{
        for(x=60;x>=30;x--){
            setClock=(minAngle+((maxAngle-minAngle)/60*x));
            clrClock=frequency-setClock;
            *(motorSENSOR+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motorSENSOR+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
    }
    return length;
}


static int motorSENSOR_release(struct inode *minode, struct file *mfile){
    motorSENSOR_usage=0;
    if(motorSENSOR) iounmap(motorSENSOR);
    return 0;
}
