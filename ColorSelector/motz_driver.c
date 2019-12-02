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

#define motor180z_MAJOR	224
#define motor180z_NAME	"MOTOR180Z_DRIVER"
#define GPIO_SIZE	256
#define GPIO        12


char motor180z_usage=0;
static void *motor180z_map;
volatile unsigned *motor180z;


static int motor180z_init(void);
static void motor180z_exit(void);
static int motor180z_release(struct inode*, struct file*);
static int motor180z_open(struct inode*, struct file*);
static int motor180z_write(struct file*, const char*, size_t, loff_t*);



static struct file_operations motor180z_fops=
        {
                .owner	=THIS_MODULE,
                .open	=motor180z_open,
                .release=motor180z_release,
                .write	=motor180z_write,
        };

static int motor180z_init(void){
    int result;
    result=register_chrdev(motor180z_MAJOR, motor180z_NAME, &motor180z_fops);
    if(result<0){
        printk(KERN_WARNING"Can't get any major!\n");
        return result;
    }
}


static void motor180z_exit(void){
    unregister_chrdev(motor180z_MAJOR, motor180z_NAME);
    printk("motor180z module removed.\n");
}

module_init(motor180z_init);
module_exit(motor180z_exit);
MODULE_LICENSE("GPL");


static int motor180z_open(struct inode *inode, struct file *mfile){
    if(motor180z_usage != 0)
        return -EBUSY;
    motor180z_usage=1;

    motor180z_map=ioremap(GPIO_BASE, GPIO_SIZE);

    if(!motor180z_map){
        printk("error:mapping gpio memory");
        iounmap(motor180z_map);
        return -EBUSY;
    }
    motor180z=(volatile unsigned int*)motor180z_map;

    *(motor180z+(GPIO/10))&=~(0x07<<(3*(GPIO%10)));
    *(motor180z+(GPIO/10))|=(0x01<<(3*(GPIO%10)));

    return 0;
}

static int motor180z_write(struct file *minode, const char *gdata, size_t length, loff_t *off_what){
    char tmp_buf;
    int result;
    int frequency=20000;
    int minAngle=544;
    int maxAngle=2440;
    int x;
    unsigned int setClock;
    unsigned int clrClock;
    unsigned long delay;


    result=copy_from_user(&tmp_buf, gdata, length);
    if(result<0){
        printk("Error:copy from user");
        return result;
    }
    printk("[motz]data from user: %d\n", result);

    if(tmp_buf==1){
        printk("[motz] Active 1\n");
        for(x=0;x<=30;x++){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motor180z+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motor180z+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
        delay = jiffies + 3*HZ;
        while(time_before(jiffies, delay)){}
        for(x=30;x>=0;x--){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motor180z+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motor180z+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
    }

    else if(tmp_buf==2){
        printk("[motz] Active 2\n");

        for(x=0;x<=60;x++){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motor180z+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motor180z+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
        delay = jiffies + 3*HZ;
        while(time_before(jiffies, delay)){}
        for(x=60;x>=0;x--){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motor180z+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motor180z+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
    }

    else if(tmp_buf==3){
        printk("[motz] Active 3\n");

        for(x=0;x<=90;x++){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motor180z+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motor180z+10)=(0x01<<GPIO);
            udelay(clrClock);
        }

        delay = jiffies + 3*HZ;
        while(time_before(jiffies, delay)){}
        for(x=90;x>=0;x--){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motor180z+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motor180z+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
    }

    else if(tmp_buf==4){
        printk("[motz] Active 4\n");

        for(x=0;x<=120;x++){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motor180z+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motor180z+10)=(0x01<<GPIO);
            udelay(clrClock);
        }

        delay = jiffies + 3*HZ;
        while(time_before(jiffies, delay)){}
        for(x=120;x>=0;x--){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motor180z+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motor180z+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
    }

    else if(tmp_buf==5){
        printk("[motz] Active 5\n");

        for(x=0;x<=150;x++){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motor180z+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motor180z+10)=(0x01<<GPIO);
            udelay(clrClock);
        }

        delay = jiffies + 3*HZ;
        while(time_before(jiffies, delay)){}
        for(x=150;x>=0;x--){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motor180z+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motor180z+10)=(0x01<<GPIO);
            udelay(clrClock);
        }

    }

    else if(tmp_buf==6){
        printk("[motz] Active 6\n");
        for(x=0;x<=180;x++){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motor180z+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motor180z+10)=(0x01<<GPIO);
            udelay(clrClock);
        }

        delay = jiffies + 3*HZ;
        while(time_before(jiffies, delay)){}
        for(x=180;x>=0;x--){
            setClock=(minAngle+((maxAngle-minAngle)/180*x));
            clrClock=frequency-setClock;
            *(motor180z+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motor180z+10)=(0x01<<GPIO);
            udelay(clrClock);
        }

    }

    else {
        printk("[motz](else)Not Active\n");
    }

    return length;
}


static int motor180z_release(struct inode *minode, struct file *mfile){
    motor180z_usage=0;
    if(motor180z) iounmap(motor180z);
    return 0;
}
