#include <kernel.h>


uint16_t pci_read_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset) {
	uint64_t address;
    uint64_t lbus = (uint64_t)bus;
    uint64_t lslot = (uint64_t)slot;
    uint64_t lfunc = (uint64_t)func;
    uint16_t tmp = 0;
    address = (uint64_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outb(address, 0xCF8);
    tmp = (uint16_t)(
        (inb(0xCFC) >> ((offset & 2) * 8)) //& 0xffff
        );
    return (tmp);
}


uint16_t getVendorID(uint16_t bus, uint16_t device, uint16_t function) {
        uint32_t r0 = pci_read_word(bus,device,function,0);
        return r0;
}


uint16_t getDeviceID(uint16_t bus, uint16_t device, uint16_t function) {
        uint32_t r0 = pci_read_word(bus,device,function,2);
        return r0;
}


void checkAllBuses(void) {
    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            for (uint8_t function = 1; function < 8; function++) {
                if (getDeviceID(bus, device, function) != 0){
                    tty_printf("%d %d %d ->%d: %d\n", bus, device, function, getDeviceID(bus, device, function), getVendorID(bus, device, function));
                    qemu_printf("%d %d %d ->%d: %d \n", bus, device, function, getDeviceID(bus, device, function), getVendorID(bus, device, function));
                }
            }
        }
    }


    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            for (uint8_t function = 1; function < 8; function++) {
                for (int i = 0; i!= 48; i++){
                    if (pci_read_word(bus, device, function, i) != 0 & pci_read_word(bus, device, function, i) != 17 & pci_read_word(bus, device, function, i) != 255){
                        qemu_printf("%d %d %d %d->pci_read_word: %d \n", bus, device, function, i, pci_read_word(bus, device, function, i));
                    }
                }                    
            }
        }
        qemu_printf("\t\tbus->%d\n", bus);

    }
}


void pci_init() {
	checkAllBuses();
	tty_printf("PCI driver support loaded.\n");
}