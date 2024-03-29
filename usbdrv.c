#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
#define USB_VENDOR_ID 
#define USB_PRODUCT_ID
static void __exit cleanup(void)
{
   printk("Module cleanup usbdrv module i mean!");
}

const struct usb_device_id etx_usb_table[] = {
    { USB_DEVICE( USB_VENDOR_ID, USB_PRODUCT_ID ) },    //Put your USB device's Vendor and Product ID
    { } /* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, etx_usb_table);

static int __init startup(void)
{
    printk("Module startup usbdrv module i mean!");
    return 0;
}

static int etx_usb_probe(struct usb_interface *interface,
                        const struct usb_device_id *id)
{
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
module_init(startup);
module_exit(cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Linux");