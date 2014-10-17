#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/fcntl.h>


//module attributes
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Device driver Demo");
MODULE_AUTHOR("SHAIL SHAH");

static char msg[100]={0};
static short readpos=0;
static int times=0;
static char sub[3]={0};
static char error=0;	


//protorype, else the structure initialization that follows fail
static int dev_open(struct inode *,struct  file *);
static int dev_rls(struct inode *,struct  file *);
static ssize_t dev_read(struct file *,char *, size_t,loff_t *);
static ssize_t dev_write(struct file *,const char *, size_t,loff_t *);

//structure containing callbacks
static struct  file_operations fops=
{
        .read=dev_read,
        .open=dev_open,
        .write=dev_write,
        .release=dev_rls,
};



int init_module(void){

	int t=register_chrdev(119,"bright",&fops);

	if(t<0) printk(KERN_ALERT "Device registration failed..\n");
	else printk(KERN_ALERT "Device registered...\n");

	return t;
}

void cleanup_module(void)
{
	unregister_chrdev(119,"bright");	
}

static int dev_open(struct inode *inod,struct file *fil )
{
	times++;
	printk(KERN_ALERT "Device Opened....\n");
	return 0;
}
static ssize_t dev_read(struct file * filp,char *buff, size_t len,loff_t *off){
    short count=0;
    char message1[]="error";
    char message2[]="success";
    if(error==1){

    	while(len && (message1[readpos]!=0)){
     		put_user(message1[readpos],buff++);
	    	count++;
	    	len--;
			readpos++;
		}
		error=0;
		return count;	
    } 
    else{
    	while(len && (message2[readpos]!=0)){
     			put_user(message2[readpos],buff++);
	    		count++;
	    		len--;
				readpos++;
			}
		return count;		
    }
}
static ssize_t dev_write(struct file *filp,const char *buff, size_t len, loff_t *off){
	short ind=0;
	short count=0;
	char * temp=NULL;
	int i;
	struct file *file;
	loff_t pos = 0;
  	mm_segment_t old_fs;


	memset(msg,0,100);
	readpos=0;
	while(len>0){
		msg[count++]=buff[ind++];
		len--;
	}
		
	for(i=0;i<3;i++)
		sub[i]=msg[i];
	
	if(strcmp(sub,"set")!=0){
		error=1;
		return count;
	}
	temp=msg;
	temp+=4;
	
  printk(KERN_ALERT "%s",temp);
  old_fs=get_fs();
  set_fs(get_ds());

  file = filp_open("/sys/class/backlight/acpi_video0/brightness", O_WRONLY|O_CREAT, 0644);
  if(file){
	
   		vfs_write(file, temp, 2, &pos);
		error=0;
		filp_close(file,NULL);
	}
  set_fs(old_fs);
	return count; 
}
static int dev_rls(struct inode * inod,struct file *fil){
	printk(KERN_ALERT"Device Closed\n");
	return 0;
}


