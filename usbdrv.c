#include <linux/init.h>
#include <linux/module.h>
#include <linux/usb.h>
#include "proc/proc.h"
#include <linux/fs.h>
#include <linux/types.h>
MODULE_LICENSE("GPL");
#define USB_VENDOR_ID 0x488
#define USB_PRODUCT_ID 0x5740
#define DRIVER_NAME "STMDriver"
#define DEVICE_NAME "STMDevice"


const struct usb_device_id etx_usb_table[] = {
    { USB_DEVICE( USB_VENDOR_ID, USB_PRODUCT_ID ) },    //Put your USB device's Vendor and Product ID
    { } /* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, etx_usb_table);


struct usb_stm_dev {
	struct usb_device *	udev;			/* the usb device for this device */
	struct usb_interface *	interface;		/* the interface for this device */
	unsigned char *		bulk_in_buffer;		/* the buffer to receive data */
	size_t			bulk_in_size;		/* the size of the receive buffer */
	__u8			bulk_in_endpointAddr;	/* the address of the bulk in endpoint */
	__u8			bulk_out_endpointAddr;	/* the address of the bulk out endpoint */
	struct kref		kref;
};



static int etx_usb_probe(struct usb_interface *interface,const struct usb_device_id *id);

static void etx_usb_disconnect(struct usb_interface *interface);

static struct usb_driver etx_usb_driver = {
    .name       = "STMProgrammer USB Driver",
    .probe      = etx_usb_probe,
    .disconnect = etx_usb_disconnect,
    .id_table   = etx_usb_table,
    
};

static ssize_t write(struct file* f,const char* data,size_t len,loff_t* offset);

static ssize_t read(struct file* f,char* data,size_t len,loff_t* offset);

static int release(struct inode* node,struct file* f);

static int open(struct inode* inod,struct file* f);

static struct file_operations stm_fops = {
	.owner =	THIS_MODULE,
	.read =		read,
	.write =	write,
	.open =		open,
	.release =	release,
};
static struct usb_class_driver stm_class = {
	.name = "usb/stm%d",
	.fops = &stm_fops,
	.minor_base = 1,
};


static int etx_usb_probe(struct usb_interface *interface,
                        const struct usb_device_id *id)
{
    //interface->
    dev_info(&interface->dev, "USB Driver Probed: Vendor ID : 0x%02x,\t"
             "Product ID : 0x%02x\n", id->idVendor, id->idProduct);
    

    usb_register_dev(interface,&stm_class);
            
    struct usb_stm_dev* stm_dev=kmalloc(sizeof(struct usb_stm_dev),GFP_KERNEL);
    stm_dev->udev=interface_to_usbdev(interface);
    stm_dev->interface=interface;
    
    struct usb_host_interface* current_interface= interface->cur_altsetting;

    printk("Endpoinst num: %d",current_interface->desc.bNumEndpoints);

    for(int i=0;i<current_interface->desc.bNumEndpoints;i++)
    {
        printk("Endpoint : %d , %d",current_interface->endpoint[i].desc.bEndpointAddress,current_interface->endpoint[i].desc.bmAttributes);

        if((current_interface->endpoint[i].desc.bEndpointAddress & USB_DIR_IN) &&
        ((current_interface->endpoint[i].desc.bmAttributes & USB_ENDPOINT_XFERTYPE_MASK)==USB_ENDPOINT_XFER_BULK)
        )
        {
            stm_dev->bulk_in_endpointAddr=current_interface->endpoint[i].desc.bEndpointAddress;
            stm_dev->bulk_in_size=current_interface->endpoint[i].desc.wMaxPacketSize;
            printk("Bulk in size%d",stm_dev->bulk_in_size);
            stm_dev->bulk_in_buffer=kmalloc(stm_dev->bulk_in_size,GFP_KERNEL);
        }
        else //if((current_interface->endpoint[i].desc.bmAttributes & USB_ENDPOINT_XFERTYPE_MASK)==USB_ENDPOINT_XFER_BULK)
        
        {
            printk("Bulk out ");
            stm_dev->bulk_out_endpointAddr=current_interface->endpoint[i].desc.bEndpointAddress;  
        }
    }

   
    usb_set_intfdata(interface,stm_dev);        
    return 0;  //return 0 indicates we are managing this device
}

static void etx_usb_disconnect(struct usb_interface *interface)
{
    
    dev_info(&interface->dev, "USB Driver Disconnected\n");
    struct usb_stm_dev* stm_dev=usb_get_intfdata(interface);
    kfree(stm_dev);
    usb_set_intfdata(interface,NULL);
    usb_deregister_dev(interface,&stm_class);
}


static ssize_t write(struct file* f,const char* data,size_t len,loff_t* offset)
{
    printk("usb write is called!%d",len);
    struct usb_stm_dev* dv=f->private_data;
    if(dv==NULL)
        printk("dv null");
    const int actualSize=min(dv->bulk_in_size,len);
    copy_from_user(dv->bulk_in_buffer,data,actualSize);
    int actual=0;
    usb_bulk_msg(dv->udev,usb_sndbulkpipe(dv->udev,dv->bulk_out_endpointAddr),dv->bulk_in_buffer,actualSize,&actual,HZ*10);    
     printk("write has been returned %d",actual);
    return actual;
}

static ssize_t read(struct file* f,char* data,size_t len,loff_t* offset)
{

    printk("usb read is called!");
    struct usb_stm_dev* dv=f->private_data;
    int actual=0;
    int retval=usb_bulk_msg(dv->udev,usb_rcvbulkpipe(dv->udev,dv->bulk_in_endpointAddr),dv->bulk_in_buffer,min(dv->bulk_in_size,len),&actual,HZ*10);
    copy_to_user(data,dv->bulk_in_buffer,len);
     printk("usb read is returned %d!",actual);
    return len;

}
static int release(struct inode* node,struct file* f)
{
    return 0;
}   

static int open(struct inode* inod,struct file* f)
{
    int major;
	int minor;
	major = imajor(inod);
	minor = iminor(inod);
	printk("\n*****Some body is opening me at major %d  minor %d*****\n",major, minor);

   struct usb_interface* interface = usb_find_interface(&etx_usb_driver, minor);
   struct usb_stm_dev* dev = usb_get_intfdata(interface);
   f->private_data=dev;

	return 0;
}





// struct usb_driver 
// {
//   const char * name;
//   int (* probe) (struct usb_interface *intf,const struct usb_device_id *id);
//   void (* disconnect) (struct usb_interface *intf);
//   int (* ioctl) (struct usb_interface *intf, unsigned int code,void *buf);
//   int (* suspend) (struct usb_interface *intf, pm_message_t message);
//   int (* resume) (struct usb_interface *intf);
//   int (* reset_resume) (struct usb_interface *intf);
//   int (* pre_reset) (struct usb_interface *intf);
//   int (* post_reset) (struct usb_interface *intf);
//   const struct usb_device_id * id_table;
//   struct usb_dynids dynids;
//   struct usbdrv_wrap drvwrap;
//   unsigned int no_dynamic_id:1;
//   unsigned int supports_autosuspend:1;
//   unsigned int soft_unbind:1;
// };

static int __init startup(void)
{
    printk("Module startup usbdrv module i mean!");
    //  if(!init_proc_entry())
    // {
    //     return -ENOMEM;
    // }
    usb_register(&etx_usb_driver);
    return 0;
}

static void __exit cleanup(void)
{
   printk("Module cleanup usbdrv module i mean!");
   

   usb_deregister(&etx_usb_driver);
 //  deinit_proc_entry();
}

module_init(startup);
module_exit(cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ali");
