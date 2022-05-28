#include <kernel.h>

#define RTL8139_VENDOR_ID 0x10EC
#define RTL8139_DEVICE_ID 0x8139


void RTL8139_init() {
    //pci_dev_t pci_rtl8139_device;
    //pci_rtl8139_device = pci_get_device(RTL8139_VENDOR_ID, RTL8139_DEVICE_ID, -1);

    uint32_t ret = pci_read(pci_get_device(RTL8139_VENDOR_ID, RTL8139_DEVICE_ID, -1), PCI_BAR0);
    tty_printf("ret = %d\n", ret);
}