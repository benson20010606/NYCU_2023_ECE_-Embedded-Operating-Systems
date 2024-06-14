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
#define GPIO_12 (12)
#define GPIO_16 (16)
#define GPIO_20 (20)
#define GPIO_21 (21)
uint8_t GPIO[4]={GPIO_12,GPIO_16,GPIO_20,GPIO_21};
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

  
    gpio_set_value(GPIO_12, 0);
    gpio_set_value(GPIO_16, 0);
    gpio_set_value(GPIO_20, 0);
    gpio_set_value(GPIO_21, 0);
  pr_info("Device File Closed...!!!\n");
  return 0;
}

/*
** This function will be called when we read the Device file
*/ 
static ssize_t etx_read(struct file *filp, 
                char __user *buf, size_t len, loff_t *off)
{
  pr_info("No function!!");
  
  return 0;
}

/*
** This function will be called when we write the Device file
*/ 
static ssize_t etx_write(struct file *filp, 
                const char __user *buf, size_t len, loff_t *off)
{
  uint8_t rec_buf[1] = {0};
  
    if( copy_from_user( rec_buf, buf, len ) > 0) {
        pr_err("ERROR: Not all the bytes have been copied from user\n");
      }

      pr_info("Write Function : %c \n", rec_buf[0]);

    gpio_set_value(GPIO_12, 0);
    gpio_set_value(GPIO_16, 0);
    gpio_set_value(GPIO_20, 0);
    gpio_set_value(GPIO_21, 0);

     if (rec_buf[0]=='1'||rec_buf[0]=='3'||rec_buf[0]=='5'||rec_buf[0]=='7'||rec_buf[0]=='9'){
	 gpio_set_value(GPIO_12, 1);
	 }
     if (rec_buf[0]=='2'||rec_buf[0]=='3'||rec_buf[0]=='6'||rec_buf[0]=='7'){
	 gpio_set_value(GPIO_16, 1);
         }
     if (rec_buf[0]=='4'||rec_buf[0]=='5'||rec_buf[0]=='6'||rec_buf[0]=='7'){
         gpio_set_value(GPIO_20, 1);
          }
     if (rec_buf[0]=='8'||rec_buf[0]=='9'){
         gpio_set_value(GPIO_21, 1);
         }


  		
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
  if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"etx_device"))){
    pr_err( "Cannot create the Device \n");
    goto r_device;
  }
  
  //Checking the GPIO is valid or not
  if(gpio_is_valid(GPIO_21) == false){
    pr_err("GPIO %d is not valid\n", GPIO_21);
    goto r_device;
  }
  //Requesting the GPIO
  if(gpio_request(GPIO_21,"GPIO_21") < 0){
    pr_err("ERROR: GPIO %d request\n", GPIO_21);
    goto r_gpio;
  }
  
  //Checking the GPIO is valid or not
  if(gpio_is_valid(GPIO_20) == false){
    pr_err("GPIO %d is not valid\n", GPIO_20);
    goto r_device;
  }
  //Requesting the GPIO
  if(gpio_request(GPIO_20,"GPIO_20") < 0){
    pr_err("ERROR: GPIO %d request\n", GPIO_20);
    goto r_gpio;
  }
  //Checking the GPIO is valid or not
  if(gpio_is_valid(GPIO_16) == false){
    pr_err("GPIO %d is not valid\n", GPIO_16);
    goto r_device;
  }
  //Requesting the GPIO
  if(gpio_request(GPIO_16,"GPIO_16") < 0){
    pr_err("ERROR: GPIO %d request\n", GPIO_16);
    goto r_gpio;
  }
  //Checking the GPIO is valid or not
  if(gpio_is_valid(GPIO_12) == false){
    pr_err("GPIO %d is not valid\n", GPIO_12);
    goto r_device;
  }
  //Requesting the GPIO
  if(gpio_request(GPIO_12,"GPIO_12") < 0){
    pr_err("ERROR: GPIO %d request\n", GPIO_12);
    goto r_gpio;
  }
  
  
  //configure the GPIO as output
   for(uint8_t i=0; i<4;i++){
       gpio_direction_output(GPIO[i], 0);
       gpio_export(GPIO[i], false);
     }


  
  pr_info("Device Driver Insert...Done!!!\n");
  return 0;
 
r_gpio:
  gpio_free(GPIO_21);
  gpio_free(GPIO_20);
  gpio_free(GPIO_16);
  gpio_free(GPIO_12);
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



    for(uint8_t i=0 ;i<4;i++){
        gpio_unexport(GPIO[i]);
        gpio_free(GPIO[i]);
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
