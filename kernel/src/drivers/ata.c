#include <kernel.h>


pci_dev_t ata_device;

ata_dev_t primary_master = {.slave = 0};
ata_dev_t primary_slave = {.slave = 1};
ata_dev_t secondary_master = {.slave = 0};
ata_dev_t secondary_slave = {.slave = 1};


void ata_init() {
    ata_device = pci_get_device(ATA_VENDOR_ID, ATA_DEVICE_ID, -1);
    tty_printf("ATA info:\n"            \
                "\tbus %d\n"            \
                "\tenable %d\n"         \
                "\tdevice number %d\n"  \
                "\treversed %d\n"       \
                "\n", 
                ata_device.bus_num,
                ata_device.enable,
                ata_device.device_num,
                ata_device.reserved
                ); 
    tty_printf("ATA installed\n");
    qemu_printf("ATA installed\n");
    
}