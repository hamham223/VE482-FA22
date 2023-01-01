//
// Created by citrate on 11/23/21.
//

#include <linux/module.h>
#include <linux/printk.h>
#include <linux/fs.h>                 /* struct file */
#include <linux/cdev.h>               /* cdev */
#include <linux/slab.h>
//TODO: Add necessary header files
#include "dice.h"

static int dice_major;                /* Major number of dice devices */
static int dice_devs = DICE_DEVS;     /* Number of dice devices */
static int dice_num = DICE_NUM;       /* Initial number of dice in a device */
//static int gen_sides = GEN_SIDES;     /* Initial number of sides of `arbitrary` dice game */
static struct dice_dev *dice_devices; /* An array of dice device */
static struct class *dice_class;      /* Used for class */


static int __init dice_init(void);
static void __exit dice_exit(void);
static int dice_open(struct inode *inode, struct file *filp);
static int dice_release(struct inode *inode, struct file *filp);
static ssize_t dice_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
static ssize_t dice_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);

MODULE_LICENSE("GPL");
module_init(dice_init);
module_exit(dice_exit);
//TODO: Make gen_sides a module parameter

static struct file_operations fops = {
        .read = dice_read,
        .write = dice_write,
        .open = dice_open,
        .release = dice_release,
};

static int __init dice_init(void) {
    //TODO: Find Major number dynamically
    // Hint: alloc_chrdev_region

    //TODO: Allocate memory for dices

    //TODO: create a class, loop through registering cdev and creating device
    // Hint: class_create, cdev_init, cdev_add, device_create
    dev_t dev_tmp;
    int res;
    int i;
    dev_t dev_cur;
    struct device *dev_ret;
    res = alloc_chrdev_region(&dev_tmp,0,dice_devs,"Hamster's Dice");
    if ( res < 0 ){
        printk(KERN_WARNING,"Error: Hamster ate the dice and it disappered.!\n");
        return res;
    } 
    dice_major = MAJOR(dev_tmp);

    //  * %GFP_KERNEL is typical for kernel-internal allocations. The caller requires
    //  * %ZONE_NORMAL or a lower zone for direct access but can direct reclaim.
    dice_devices = kmalloc(sizeof(struct dice_dev) * dice_devs,GFP_KERNEL);

    dice_class = class_create(THIS_MODULE,"Hamster");
    
    for (i = 0; i < dice_devs; i++) {
        cdev_init(&(dice_devices[i].dice_cdev),&fops);
        dice_devices[i].num = dice_num;
        dice_devices[i].dice_type = 1;
        dev_cur = MKDEV(dice_major, i);;
        cdev_add(&(dice_devices[i].dice_cdev),dev_cur,1);
        dev_ret = device_create(dice_class,NULL,dev_cur,NULL,"hamster_dice_%d",i+1);
    }
    printk(KERN_INFO,"Hamster's dice module inited!\n");
    return 0;
}

static void __exit dice_exit(void) {
    //TODO: release all resources
    // Hint: Pay attention to the order!
    // Hint: device_destroy, cdev_del, class_destroy, kfree, unregister_chrdev_region
    int i = 0;
    for (i = 0; i < dice_devs; i++) {
        device_destroy(dice_class,MKDEV(dice_major, i));
    }
    for (i = 0; i < dice_devs; i++) {
        cdev_del(&(dice_devices[i].dice_cdev));
    }
    class_destroy(dice_class);
    kfree(dice_devices);
    unregister_chrdev_region(MKDEV(dice_major,0),dice_devs);
}

static int dice_open(struct inode *inode, struct file *filp) {
    //TODO: Find which dice is opened
    // Hint: container_of, filp->private_data
    filp->private_data = container_of(inode->i_cdev, struct dice_dev, dice_cdev);
    printk(KERN_INFO,"Almost there grandpa!\n");
    //printk(KERN_DEBUG,"Here is dice information: ");
    //printk(KERN_SOH,"%d\n",dice_devices[0].num);
    return 0;   // what should be returned?
}

static int dice_release(struct inode *inode, struct file *filp) {
    //Do nothing here
    //If you allocate any resource in dice_open(), then you must release them here
    return 0;   // what should be returned?
}

static ssize_t dice_read(struct file *filp, char __user *buff, size_t count, loff_t *offp) {
    //TODO: Generate dice patterns, generate random number
    // Attention: handle count and offp carefully
    // Hint: copy_to_user

    //printk(KERN_INFO,"Almost there grandpa!\n");
    const char message[40];
    struct dice_dev *dice;
    int res;
    int offset;

    offset = 0;
    offset += snprintf(message+offset,40,"Almost there grandpa!\n");
    
    dice =  (struct dice_dev *)filp->private_data;

    if (*offp + count > offset) {
        count = offset - *offp;
    }
    res = copy_to_user(buff,message + *offp ,count);
    if ( res != 0) {
        printk(KERN_ERR,"Hamster: copy to user failed!\n");
        return res;
    }
    *offp += count;
    return count;   // what should be returned?
}
static ssize_t dice_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp) {
    //TODO: Read in number of dice
    // Attention: handle count and offp carefully
    // Hint: copy_from_user
    return 0;   // what should be returned?
}
