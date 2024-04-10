
#include "proc.h"

#define PROC_BASE "STMProgrammer"
#define PROC_CONNECTION_R "connection"


static ssize_t read_proc(struct file *filp,char *buf,size_t count,loff_t *offp ) 
{
    
printk("count: %d",count);
if(*offp>=15)
{
    return 0;
}
const unsigned int len=count>15?15:count;
const unsigned long res=copy_to_user(buf,"hi from kernel", len);

*offp=len;
return len;

}

static struct proc_ops proc_fops = {

.proc_read=   read_proc
};


bool init_proc_entry(void)
{
    
    const struct proc_dir_entry* dir_entry=proc_create(PROC_BASE,0666,NULL,&proc_fops);
    if(!dir_entry)
    {
        printk("init_proc_entry failed!");
        return false;
    }
    return true;
}

void deinit_proc_entry(void)
{
   
        remove_proc_entry(PROC_BASE,NULL);
    
}

