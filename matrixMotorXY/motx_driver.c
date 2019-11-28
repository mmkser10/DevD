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

#define motor180x_MAJOR	222
#define motor180x_NAME	"MOTOR180X_DRIVER"
#define GPIO_SIZE	256
#define GPIO        	10


char motor180x_usage=0;
static void *motor180x_map;
volatile unsigned *motor180x;


static int motor180x_init(void);
static void motor180x_exit(void);
static int motor180x_release(struct inode*, struct file*);
static int motor180x_open(struct inode*, struct file*);
static int motor180x_write(struct file*, const char*, size_t, loff_t*);



static struct file_operations motor180x_fops=
        {
                .owner	=THIS_MODULE,
                .open	=motor180x_open,
                .release=motor180x_release,
                .write	=motor180x_write,
        };

static int motor180x_init(void){
    int result;
    result=register_chrdev(motor180x_MAJOR, motor180x_NAME, &motor180x_fops);
    if(result<0){
        printk(KERN_WARNING"Can't get any major!\n");
        return result;
    }
}




static void motor180x_exit(void){
    unregister_chrdev(motor180x_MAJOR, motor180x_NAME);
    printk("motor180x module removed.\n");
}



module_init(motor180x_init);
module_exit(motor180x_exit);
MODULE_LICENSE("GPL");


static int motor180x_open(struct inode *inode, struct file *mfile){
    if(motor180x_usage != 0)
        return -EBUSY;
    motor180x_usage=1;

    motor180x_map=ioremap(GPIO_BASE, GPIO_SIZE);

    if(!motor180x_map){
        printk("error:mapping gpio memory");
        iounmap(motor180x_map);
        return -EBUSY;
    }
    motor180x=(volatile unsigned int*)motor180x_map;

    *(motor180x+(GPIO/10))&=~(0x07<<(3*(GPIO%10)));
    *(motor180x+(GPIO/10))|=(0x01<<(3*(GPIO%10)));

    return 0;
}

static int motor180x_write(struct file *minode, const char *gdata, size_t length, loff_t *off_what){
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
            setClock=(minAngle+((maxAngle-minAngle)/90*x));
            clrClock=frequency-setClock;
            *(motor180x+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motor180x+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
        while(time_before(jiffies, delay)){}
        for(x=30;x>=0;x--){
            setClock=(minAngle+((maxAngle-minAngle)/90*x));
            clrClock=frequency-setClock;
            *(motor180x+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motor180x+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
    }

    if(tmp_buf==2){
        for(x=90;x<=45;x--){
            setClock=(minAngle+((maxAngle-minAngle)/90*x));
            clrClock=frequency-setClock;
            *(motor180x+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motor180x+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
        while(time_before(jiffies, delay)){}
        for(x=45;x>=90;x++){
            setClock=(minAngle+((maxAngle-minAngle)/90*x));
            clrClock=frequency-setClock;
            *(motor180x+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motor180x+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
    }

    if(tmp_buf==3){
        for(x=0;x<=30;x++){
            setClock=(minAngle+((maxAngle-minAngle)/30*x));
            clrClock=frequency-setClock;
            *(motor180x+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motor180x+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
        while(time_before(jiffies, delay)){}
        for(x=30;x>=0;x--){
            setClock=(minAngle+((maxAngle-minAngle)/30*x));
            clrClock=frequency-setClock;
            *(motor180x+7)=(0x01<<GPIO);
            udelay(setClock);
            *(motor180x+10)=(0x01<<GPIO);
            udelay(clrClock);
        }
    }

    if(tmp_buf==4){
        printk("Activate 4");
    }

    if(tmp_buf==5){
        printk("Activate 5");
    }

    if(tmp_buf==6){
        printk("Activate 6");
    }
    return length;
}


static int motor180x_release(struct inode *minode, struct file *mfile){
    motor180x_usage=0;
    if(motor180x) iounmap(motor180x);
    return 0;
}
