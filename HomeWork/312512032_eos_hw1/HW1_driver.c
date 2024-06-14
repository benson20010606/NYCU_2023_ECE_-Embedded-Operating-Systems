/***************************************************************************//**
*  \file       led_driver.c
*
*  \details    Simple GPIO driver explanation
*
*  \author     EmbeTronicX
*
*  \Tested with Linux raspberrypi 5.4.51-v7l+
*
*******************************************************************************/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/uaccess.h>  //copy_to/from_user()
#include <linux/gpio.h>     //GPIO
#include <linux/err.h>

//LED is connected to this GPIO
#define GPIO_25 (25)
#define GPIO_8 (8)
#define GPIO_7 (7)
#define GPIO_1 (1)
#define GPIO_12 (12)
#define GPIO_16 (16)
#define GPIO_20 (20)
#define GPIO_21 (21)
//seven-segment display
#define GPIO_11 (11)
#define GPIO_0 (0)
#define GPIO_5 (5)
#define GPIO_6 (6)
#define GPIO_13 (13)
#define GPIO_19 (19)
#define GPIO_26 (26)
//
uint8_t GPIO[8]={GPIO_25, GPIO_8,GPIO_7,GPIO_1,GPIO_12,GPIO_16,GPIO_20,GPIO_21};
char* GPIO_lable[8]={"GPIO_25", "GPIO_8","GPIO_7","GPIO_1","GPIO_12","GPIO_16","GPIO_20","GPIO_21"};

uint8_t GPIO_dispaly[7]={GPIO_11, GPIO_0,GPIO_5,GPIO_6,GPIO_13,GPIO_19,GPIO_26};
char* GPIO_dispaly_lable[7]={"GPIO_11", "GPIO_0","GPIO_5","GPIO_6","GPIO_13","GPIO_19","GPIO_26"};

static uint8_t show[10][7]={{0,0,0,0,0,0,1},
                                {1,0,0,1,1,1,1},
                                {0,0,1,0,0,1,0},
                                {0,0,0,0,1,1,0},
                                {1,0,0,1,1,0,0},
                                {0,1,0,0,1,0,0},
                                {0,1,0,0,0,0,0},
                                {0,0,0,1,1,0,1},
                                {0,0,0,0,0,0,0},
                                {0,0,0,1,1,0,0}};


uint8_t order_arrive_flag=0;
dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
 
static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);
 
 
/*************** Driver functions **********************/
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, 
                char __user *buf, size_t len,loff_t * off);
static ssize_t etx_write(struct file *filp, 
                const char *buf, size_t len, loff_t * off);
/******************************************************/
void Delivery_distance(uint8_t index){

    for(uint8_t i=0;i<(index-'0');i++){
        gpio_set_value(GPIO[i], 1);
    }
    for(int i=(index-'0')-1;i>=0;i--){
        mdelay(1000);
        gpio_set_value(GPIO[i], 0);
       
    }
         
}
void display(uint8_t num){
    for(uint8_t i=0;i<7;i++){
        gpio_set_value(GPIO_dispaly[i],1);
    }
    mdelay(20);
    for(uint8_t i=0;i<7;i++){
        gpio_set_value(GPIO_dispaly[i], show[num-'0'][i]);
    }
    mdelay(480);
}


//File operation structure 
static struct file_operations fops =
{
  .owner          = THIS_MODULE,
  .read           = etx_read,
  .write          = etx_write,
  .open           = etx_open,
  .release        = etx_release,
};

/*
** This function will be called when we open the Device file
*/ 
static int etx_open(struct inode *inode, struct file *file)
{
  pr_info("Device File Opened...!!!\n");
  return 0;
}

/*
** This function will be called when we close the Device file
*/
static int etx_release(struct inode *inode, struct file *file)
{
      for(uint8_t i=0;i<7;i++){
        gpio_set_value(GPIO_dispaly[i],1);
    }
  pr_info("Device File Closed...!!!\n");
  return 0;
}

/*
** This function will be called when we read the Device file
*/ 
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{ 
    unsigned char state[1]={0};
    if (order_arrive_flag==0){
        state[0]='0';
    }else{
        state[0]='1';
    }
    if(copy_to_user(buf,state,1)>0){
        pr_err("ERROR: Not all the bytes have been copied from user\n");
        }
  
  return 0;
}

/*
** This function will be called when we write the Device file
*/ 
static ssize_t etx_write(struct file *filp, 
                const char __user *buf, size_t len, loff_t *off)
{
    uint8_t rec_buf[100] = {0};
    order_arrive_flag=0;
    if( copy_from_user( rec_buf, buf, len ) > 0) {
        pr_err("ERROR: Not all the bytes have been copied from user\n");
    }
    Delivery_distance(rec_buf[0]);
    
    for(uint8_t i=1;i<len;i++){
        display(rec_buf[i]);
        
    }
    
    order_arrive_flag=1;


  		
  return len;
}

/*
** Module Init function
*/ 
static int __init etx_driver_init(void)
{
  /*Allocating Major number*/
    if((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) <0){
        pr_err("Cannot allocate major number\n");
        goto r_unreg;
    }
    pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
  /*Creating cdev structure*/
    cdev_init(&etx_cdev,&fops);
 
  /*Adding character device to the system*/
    if((cdev_add(&etx_cdev,dev,1)) < 0){
        pr_err("Cannot add the device to the system\n");
        goto r_del;
    }
 
  /*Creating struct class*/
    if(IS_ERR(dev_class = class_create(THIS_MODULE,"etx_class"))){
        pr_err("Cannot create the struct class\n");
        goto r_class;
    }
 
  /*Creating device*/
    if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"order"))){
        pr_err( "Cannot create the Device \n");
        goto r_device;
    }


    //Requesting the GPIO
    //Checking the GPIO is valid or not
    for(uint8_t i=0; i<8;i++){
        if(gpio_is_valid(GPIO[i]) == false){
            pr_err("GPIO %d is not valid\n", GPIO[i]);
        goto r_device;
        }
        if(gpio_request(GPIO[i],GPIO_lable[i]) < 0){
            pr_err("ERROR: GPIO %d request\n", GPIO[i]);
            goto r_gpio;
        }
    }
    for(uint8_t i=0; i<7;i++){
        if(gpio_is_valid(GPIO_dispaly[i]) == false){
            pr_err("GPIO %d is not valid\n", GPIO_dispaly[i]);
        goto r_device;
        }
        if(gpio_request(GPIO_dispaly[i],GPIO_dispaly_lable[i]) < 0){
            pr_err("ERROR: GPIO %d request\n", GPIO_dispaly[i]);
            goto r_gpio;
        }
    }
 
  //configure the GPIO as output
   for(uint8_t i=0; i<8;i++){
       gpio_direction_output(GPIO[i], 0);
       gpio_export(GPIO[i], false);
    }
   for(uint8_t i=0; i<7;i++){
       gpio_direction_output(GPIO_dispaly[i], 1);
       gpio_export(GPIO_dispaly[i], false);
    }

  
  pr_info("Device Driver Insert...Done!!!\n");
  return 0;
 
r_gpio:
    for(uint8_t i=0 ;i<8;i++){
        gpio_free(GPIO[i]);
     }
    for(uint8_t i=0 ;i<7;i++){
        gpio_free(GPIO_dispaly[i]);
     }

r_device:
  device_destroy(dev_class,dev);
r_class:
  class_destroy(dev_class);
r_del:
  cdev_del(&etx_cdev);
r_unreg:
  unregister_chrdev_region(dev,1);
  
  return -1;
}

/*
** Module exit function
*/ 
static void __exit etx_driver_exit(void)
{
    for(uint8_t i=0 ;i<8;i++){
        gpio_unexport(GPIO[i]);
        gpio_free(GPIO[i]);
     }
    for(uint8_t i=0 ;i<7;i++){
        gpio_unexport(GPIO_dispaly[i]);
        gpio_free(GPIO_dispaly[i]);
     }

  
  device_destroy(dev_class,dev);
  class_destroy(dev_class);
  cdev_del(&etx_cdev);
  unregister_chrdev_region(dev, 1);
  pr_info("Device Driver Remove...Done!!\n");
}
 
module_init(etx_driver_init);
module_exit(etx_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com>");
MODULE_DESCRIPTION("A simple device driver - GPIO Driver");
MODULE_VERSION("1.32");
