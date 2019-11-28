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
#include <linux/jiffies.h>

#define motorROUTE_MAJOR	223
#define motorROUTE_NAME	"MOTORROUTE_DRIVER"
#define GPIO_SIZE	256
#define GPIO        5


char motorROUTE_usage=0;
static void *motorROUTE_map;
volatile unsigned *motorROUTE;


static int motorROUTE_init(void);
static void motorROUTE_exit(void);
static int motorROUTE_release(struct inode*, struct file*);
static int motorROUTE_open(struct inode*, struct file*);
static int motorROUTE_write(struct file*, const char*, size_t, loff_t*);



static struct file_operations motorROUTE_fops=
        {
                .owner	=THIS_MODULE,
                .open	=motorROUTE_open,
                .release=motorROUTE_release,
                .write	=motorROUTE_write,
        };

static int motorROUTE_init(void){
    int result;
    result=register_chrdev(motorROUTE_MAJOR, motorROUTE_NAME, &motorROUTE_fops);
    if(result<0){
        printk(KERN_WARNING"Can't get any major!\n");
        return result;
    }
}




static void motorROUTE_exit(void){
    unregister_chrdev(motorROUTE_MAJOR, motorROUTE_NAME);
    printk("motorROUTE module removed.\n");
}



module_init(motorROUTE_init);
module_exit(motorROUTE_exit);
MODULE_LICENSE("GPL");


static int motorROUTE_open(struct inode *inode, struct file *mfile){
    if(motorROUTE_usage != 0)
        return -EBUSY;
    motorROUTE_usage=1;

    motorROUTE_map=ioremap(GPIO_BASE, GPIO_SIZE);

    if(!motorROUTE_map){
        printk("error:mapping gpio memory");
        iounmap(motorROUTE_map);
        return -EBUSY;
    }
    motorROUTE=(volatile unsigned int*)motorROUTE_map;

    *(motorROUTE+(GPIO/10))&=~(0x07<<(3*(GPIO%10)));
    *(motorROUTE+(GPIO/10))|=(0x01<<(3*(GPIO%10)));

    return 0;
}

static int motorROUTE_write(struct file *minode, const char *gdata, size_t length, loff_t *off_what){
    char tmp_buf;
    int result;
    int frequency=20000;
    int minAngle=544;
    int maxAngle=2440;
    int x;
    unsigned int setClock;
    unsigned int clrClock;
    unsigned long delay = jiffies + 3*HZ;


    result=copy_from_user(&tmp_buf, gdata, length);
    if(result<0){
        printk("Error:copy from user");
        return result;
    }
    printk("data from user: %d\n", tmp_buf);


    if(tmp_buf==1){
        for(x=0;x<=30;x++){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motorROUTE+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motorROUTE+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
        while(time_before(jiffies, delay)){}
        for(x=30;x>=0;x--){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motorROUTE+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motorROUTE+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
    }

    if(tmp_buf==2){
        for(x=0;x<=60;x++){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motorROUTE+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motorROUTE+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
        while(time_before(jiffies, delay)){}
        for(x=60;x>=0;x--){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motorROUTE+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motorROUTE+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
    }

    if(tmp_buf==3){
        for(x=0;x<=90;x++){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motorROUTE+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motorROUTE+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
        while(time_before(jiffies, delay)){}
        for(x=90;x>=0;x--){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motorROUTE+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motorROUTE+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
    }

    if(tmp_buf==4){
        for(x=0;x<=120;x++){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motorROUTE+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motorROUTE+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
        while(time_before(jiffies, delay)){}
        for(x=120;x>=0;x--){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motorROUTE+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motorROUTE+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
    }

    if(tmp_buf==5){
        for(x=0;x<=150;x++){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motorROUTE+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motorROUTE+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
        while(time_before(jiffies, delay)){}
        for(x=150;x>=0;x--){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motorROUTE+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motorROUTE+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
    }

    if(tmp_buf==6){
        for(x=0;x<=180;x++){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motorROUTE+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motorROUTE+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
        while(time_before(jiffies, delay)){}
        for(x=180;x>=0;x--){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motorROUTE+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motorROUTE+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
    }
    return length;
}


static int motorROUTE_release(struct inode *minode, struct file *mfile){
    motorROUTE_usage=0;
    if(motorROUTE) iounmap(motorROUTE);
    return 0;
}
