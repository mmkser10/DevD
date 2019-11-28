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

#define motor180y_MAJOR	221
#define motor180y_NAME	"MOTOR180Y_DRIVER"
#define GPIO_SIZE	256

char motor180y_usage=0;
static void *motor180y_map;
volatile unsigned *motor180y;


static int motor180y_init(void);
static void motor180y_exit(void);
static int motor180y_release(struct inode*, struct file*);
static int motor180y_open(struct inode*, struct file*);
static int motor180y_write(struct file*, const char*, size_t, loff_t*);



static struct file_operations motor180y_fops=
        {
                .owner	=THIS_MODULE,
                .open	=motor180y_open,
                .release=motor180y_release,
                .write	=motor180y_write,
        };

static int motor180y_init(void){
    int result;
    result=register_chrdev(motor180y_MAJOR, motor180y_NAME, &motor180y_fops);
    if(result<0){
        printk(KERN_WARNING"Can't get any major!\n");
        return result;
    }
}




static void motor180y_exit(void){
    unregister_chrdev(motor180y_MAJOR, motor180y_NAME);
    printk("motor180y module removed.\n");
}



module_init(motor180y_init);
module_exit(motor180y_exit);
MODULE_LICENSE("GPL");


static int motor180y_open(struct inode *inode, struct file *mfile){
    if(motor180y_usage != 0)
        return -EBUSY;
    motor180y_usage=1;

    motor180y_map=ioremap(GPIO_BASE, GPIO_SIZE);

    if(!motor180y_map){
        printk("error:mapping gpio memory");
        iounmap(motor180y_map);
        return -EBUSY;
    }
    motor180y=(volatile unsigned int*)motor180y_map;

    *(motor180y+1)&=~(0x07<<(3*7));
    *(motor180y+1)|=(0x01<<(3*7));

    return 0;
}

static int motor180y_write(struct file *minode, const char *gdata, size_t length, loff_t *off_what){
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
        for(x=0;x<=20;x++){
            setClock=(minAngle+((maxAngle-minAngle)/40*x));
            clrClock=frequency-setClock;
            *(motor180y+7)=(0x01<<17);
            udelay(setClock);
            *(motor180y+10)=(0x01<<17);
            udelay(clrClock);
        }
    }
    else{
        for(x=20;x>=0;x--){
            setClock=(minAngle+((maxAngle-minAngle)/40*x));
            clrClock=frequency-setClock;
            *(motor180y+7)=(0x01<<17);
            udelay(setClock);
            *(motor180y+10)=(0x01<<17);
            udelay(clrClock);
        }
    }
    return length;
}


static int motor180y_release(struct inode *minode, struct file *mfile){
    motor180y_usage=0;
    if(motor180y) iounmap(motor180y);
    return 0;
}
