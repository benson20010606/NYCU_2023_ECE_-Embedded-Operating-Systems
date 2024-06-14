
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
MODULE_LICENSE("GPL");

unsigned char temp[100]={0};
uint8_t input_count=0;
uint8_t output_count=0;



char* seg_for_c[27] = {
    "1111001100010001", //A
    "0000011100000101", //B
    "1100111100000000", //C
    "0000011001000101", //D
    "1000011100000001", //E
    "1000001100000001", //F
    "1001111100010000", //G
    "0011001100010001", //H
    "1100110001000100", //I
    "1100010001000100", //J
    "0000000001101100", //K
    "0000111100000000", //L
    "0011001110100000", //M
    "0011001110001000", //N
    "1111111100000000", //O
    "1000001101000001", //P
    "0111000001010000", //Q
    "1110001100011001", //R
    "1101110100010001", //S
    "1100000001000100", //T
    "0011111100000000", //U
    "0000001100100010", //V
    "0011001100001010", //W
    "0000000010101010", //X
    "0000000010100100", //Y
    "1100110000100010", //Z
    "0000000000000000",
};





// File Operations
static ssize_t my_read(struct file *fp, char *buf, size_t count, loff_t *fpos) {
    if(input_count>output_count){
        char * name=seg_for_c[temp[output_count]-'A'];
        if(copy_to_user(buf,name,count)>0){
        pr_err("ERROR: Not all the bytes have been copied from user\n");
        }
        pr_info("read Function : buf[%d]= %c\n ", output_count,temp[output_count]);
        output_count++;
    }else{
    
        copy_to_user(buf,seg_for_c[26],count);
        
        if(input_count==output_count){
        input_count=0;
        output_count=0;
 
        }
        pr_err("data not update yet \n");
    }

    printk("call read\n");
    return count;
}
static ssize_t my_write(struct file *fp,const char *buf, size_t count, loff_t *fpos) {
     char rec_buf[1] = {0};
     
     if( copy_from_user( rec_buf, buf, count ) > 0) {
        pr_err("ERROR: Not all the bytes have been copied from user\n");
      }else if (rec_buf[0]<'A'||rec_buf[0]>'Z'){
        pr_err("ERROR: input format error\n");
      }else{
      	temp[input_count]=rec_buf[0];
      	pr_info("Write Function : buf[%d]= %c\n ", input_count,temp[input_count]);
      	input_count++;	
      }
      

    
    
    printk("call write\n");
    return count;
}
static int my_open(struct inode *inode, struct file *fp) {

    
    printk("call open\n");
    return 0;
}
struct file_operations my_fops = {
    read: my_read,
    write: my_write,
    open: my_open
};

#define MAJOR_NUM 236
#define DEVICE_NAME "my_dev"
static int my_init(void) {

    printk("call init\n");
    if(register_chrdev(MAJOR_NUM, DEVICE_NAME, &my_fops) < 0) {
        printk("Can not get major %d\n", MAJOR_NUM);
        return (-EBUSY);
    }
    printk("My device is started and the major is %d\n", MAJOR_NUM);
    return 0;
}
static void my_exit(void) {
    unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
    printk("call exit\n");
}
module_init(my_init);
module_exit(my_exit);
