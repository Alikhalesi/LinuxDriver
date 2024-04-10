#include <linux/init.h>
#include <linux/module.h>
#include <linux/usb.h>
#include "proc/proc.h"
MODULE_LICENSE("GPL");
#define USB_VENDOR_ID 0x488
#define USB_PRODUCT_ID 0x5740



const struct usb_device_id etx_usb_table[] = {
    { USB_DEVICE( USB_VENDOR_ID, USB_PRODUCT_ID ) },    //Put your USB device's Vendor and Product ID
    { } /* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, etx_usb_table);



static int etx_usb_probe(struct usb_interface *interface,
                        const struct usb_device_id *id)
{
    //interface->
    dev_info(&interface->dev, "USB Driver Probed: Vendor ID : 0x%02x,\t"
             "Product ID : 0x%02x\n", id->idVendor, id->idProduct);
    return 0;  //return 0 indicates we are managing this device
}

static void etx_usb_disconnect(struct usb_interface *interface)
{
    dev_info(&interface->dev, "USB Driver Disconnected\n");
}

static struct usb_driver etx_usb_driver = {
    .name       = "STMProgrammer USB Driver",
    .probe      = etx_usb_probe,
    .disconnect = etx_usb_disconnect,
    .id_table   = etx_usb_table,
    
};


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
     if(!init_proc_entry())
    {
        return -ENOMEM;
    }
    usb_register(&etx_usb_driver);
    return 0;
}

static void __exit cleanup(void)
{
   printk("Module cleanup usbdrv module i mean!");
   
   usb_deregister(&etx_usb_driver);
   deinit_proc_entry();
}

module_init(startup);
module_exit(cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ali");