#include <kernel.h>

pci_dev_t dev_zero = {0};
uint32_t pci_size_map[100];


uint16_t pci_read_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset) {
	uint64_t address;
    uint64_t lbus = (uint64_t)bus;
    uint64_t lslot = (uint64_t)slot;
    uint64_t lfunc = (uint64_t)func;
    uint16_t tmp = 0;
    address = (uint64_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outl(0xCF8, address);
    tmp = (uint16_t)(
        (inl(0xCFC) >> ((offset & 2) * 8)) //& 0xffff
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


uint32_t pci_read(pci_dev_t dev, uint32_t field) {
	// Only most significant 6 bits of the field
	dev.field_num = (field & 0xFC) >> 2;
	dev.enable = 1;
	outl(PCI_CONFIG_ADDRESS, dev.bits);

	// What size is this field supposed to be ?
	uint32_t size = pci_size_map[field];
	if(size == 1) {
		// Get the first byte only, since it's in little endian, it's actually the 3rd byte
		uint8_t t = inb(PCI_CONFIG_DATA + (field & 3));
		return t;
	}
	else if(size == 2) {
		uint16_t t = ins(PCI_CONFIG_DATA + (field & 2));
		return t;
	}
	else if(size == 4){
		// Read entire 4 bytes
		uint32_t t = inl(PCI_CONFIG_DATA);
		return t;
	}
	return 0xffff;
}


uint32_t get_secondary_bus(pci_dev_t dev) {
	return pci_read(dev, PCI_SECONDARY_BUS);
}

// FIXME: PCI не определяет AC'97 (class = 0x04, subclass = 0x01)
void checkAllBuses() {
    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            for (uint8_t function = 1; function < 8; function++) {
				uint16_t id = getDeviceID(bus, device, function);
                if (id != 0 & id != 65535 ) {    
                    tty_printf("\t%d->", bus);
                    qemu_log("%d->", bus);

					switch (id) {
						case 0x7010:
							tty_printf("82371SB PIIX3 IDE [Natoma/Triton II]\n");
							qemu_log("82371SB PIIX3 IDE [Natoma/Triton II] ");
							break;
						case 0x7111:
							tty_printf("82371AB/EB/MB PIIX4 IDE\n");
							qemu_log("82371AB/EB/MB PIIX4 IDE ");
							break;
						case 0x7113:
							tty_printf("82371AB/EB/MB PIIX4 ACPI \n");
							qemu_log("82371AB/EB/MB PIIX4 ACPI ");
							break;
						case 0x5513:
							tty_printf("5513 IDE Controller \n");
							qemu_log("5513 IDE Controller ");
							break;
						case 0x7001:
							tty_printf("USB 1.1 Controller \n");
							qemu_log("USB 1.1 Controller ");
							break;
						case 0x7002:
							tty_printf("USB 2.0 Controller \n");
							qemu_log("USB 2.0 Controller ");
							break;
						case 0xAA28:
							tty_printf("RV620 HDMI Audio [Radeon HD 3450/3470/3550/3570] \n");
							qemu_log("RV620 HDMI Audio [Radeon HD 3450/3470/3550/3570] ");
							break;
						case 0xA012:
							tty_printf("Atom Processor D4xx/D5xx/N4xx/N5xx Integrated Graphics Controller \n");
							qemu_log("Atom Processor D4xx/D5xx/N4xx/N5xx Integrated Graphics Controller ");
							break;
						case 0x27D2:
							tty_printf("NM10/ICH7 Family PCI Express Port 2 \n");
							qemu_log("NM10/ICH7 Family PCI Express Port 2 ");
							break;
						case 0x27D6:
							tty_printf("NM10/ICH7 Family PCI Express Port 4 \n");
							qemu_log("NM10/ICH7 Family PCI Express Port 4 ");
							break;
						case 0x27C9:
							tty_printf("NM10/ICH7 Family USB UHCI Controller #2 \n");
							qemu_log("NM10/ICH7 Family USB UHCI Controller #2 ");
							break;
						case 0x27CA:
							tty_printf("NM10/ICH7 Family USB UHCI Controller #3 \n");
							qemu_log("NM10/ICH7 Family USB UHCI Controller #3 ");
							break;
						case 0x27CB:
							tty_printf("NM10/ICH7 Family USB UHCI Controller #4 \n");
							qemu_log("NM10/ICH7 Family USB UHCI Controller #4 ");
							break;
						case 0x27CC:
							tty_printf("NM10/ICH7 Family USB2 EHCI Controller \n");
							qemu_log("NM10/ICH7 Family USB2 EHCI Controller ");
							break;
						case 0x27C1:
							tty_printf("NM10/ICH7 Family SATA Controller [AHCI mode] \n");
							qemu_log("NM10/ICH7 Family SATA Controller [AHCI mode] \n");
							break;
						case 0x27DA:
							tty_printf("NM10/ICH7 Family SMBus Controller \n");
							qemu_log("NM10/ICH7 Family SMBus Controller \n");
							break;
						case 0x1360:
							tty_printf("RTL8139 Ethernet \n");
							qemu_log("RTL8139 Ethernet ");
							break;
						case 0x1300:
							tty_printf("RTL8139 Ethernet \n");
							qemu_log("RTL8139 Ethernet ");
							break;
						case 0x24D5:
							tty_printf("ICH5 AC'97 \n");
							qemu_log("ICH5 AC'97 ");
						case 0x24C5:
							tty_printf("Realtek AC'97 \n");
							qemu_log("Realtek AC'97 ");
						case 0x2668:
							tty_printf("Intel High Definition Audio \n");
							qemu_log("Intel High Definition Audio ");
						case 0x27D8:
							tty_printf("Intel High Definition Audio \n");
							qemu_log("Intel High Definition Audio ");
						default:
							tty_printf("device %x \n", getDeviceID(bus, device, function));
							qemu_log("device %x ", getDeviceID(bus, device, function));
							break;
					}
					switch (getVendorID(bus, device, function)) {
						case 0x8086:
							tty_printf("Intel Corporation\n");
							qemu_log("Intel Corporation");
							break;
						case 0x1002:
							tty_printf("Advanced Micro Devices, Inc. [AMD/ATI]\n");
							qemu_log("Advanced Micro Devices, Inc. [AMD/ATI]");
							break;
						case 0x1039:
							tty_printf("Silicon Integrated Systems [SiS]\n");
							qemu_log("Silicon Integrated Systems [SiS]");
							break;
						case 0x0B05:
							tty_printf("ASUSTek Computer, Inc.\n");
							qemu_log("ASUSTek Computer, Inc.");
							break;
						case 0x80EE:
							tty_printf("VirtualBox internal");
							qemu_log("VirtualBox internal");
							break;
						case 0x1234:
							tty_printf("QEMU internal");
							qemu_log("QEMU internal");
							break;
						default:
							tty_printf("vendor %x\n", getVendorID(bus, device, function));
							qemu_log("vendor %x", getVendorID(bus, device, function));
							break;
					}
                }
            }
        }
    }
}


uint32_t get_device_type(pci_dev_t dev) {
	uint32_t t = pci_read(dev, PCI_CLASS) << 8;
	return t | pci_read(dev, PCI_SUBCLASS);
}


pci_dev_t pci_scan_function(uint16_t vendor_id, uint16_t device_id, uint32_t bus, uint32_t device, uint32_t function, int32_t device_type) {
	pci_dev_t dev = {0};

	dev.bus_num = bus;
	dev.device_num = device;
	dev.function_num = function;
	// If it's a PCI Bridge device, get the bus it's connected to and keep searching
	if(get_device_type(dev) == PCI_TYPE_BRIDGE) {
		pci_scan_bus(vendor_id, device_id, get_secondary_bus(dev), device_type);
	}
	// If type matches, we've found the device, just return it
	if(device_type == -1 || device_type == get_device_type(dev)) {
		uint32_t devid  = pci_read(dev, PCI_DEVICE_ID);
		uint32_t vendid = pci_read(dev, PCI_VENDOR_ID);
		if(devid == device_id && vendor_id == vendid)
			return dev;
	}
	return dev_zero;
}


uint32_t pci_reach_end(pci_dev_t dev) {
	uint32_t t = pci_read(dev, PCI_HEADER_TYPE);
	return !t;
}


pci_dev_t pci_scan_device(uint16_t vendor_id, uint16_t device_id, uint32_t bus, uint32_t device, int32_t device_type) {
	pci_dev_t dev = {0};
	dev.bus_num = bus;
	dev.device_num = device;

	if(pci_read(dev,PCI_VENDOR_ID) == PCI_NONE)
		return dev_zero;

	pci_dev_t t = pci_scan_function(vendor_id, device_id, bus, device, 0, device_type);
	if(t.bits)
		return t;

	if(pci_reach_end(dev))
		return dev_zero;

	for(int32_t function = 1; function < FUNCTION_PER_DEVICE; function++) {
		if(pci_read(dev,PCI_VENDOR_ID) != PCI_NONE) {
			t = pci_scan_function(vendor_id, device_id, bus, device, function, device_type);
			if(t.bits)
				return t;
		}
	}
	return dev_zero;
}

pci_dev_t pci_scan_bus(uint16_t vendor_id, uint16_t device_id, uint32_t bus, int32_t device_type) {
	for(int32_t device = 0; device < DEVICE_PER_BUS; device++) {
		pci_dev_t t = pci_scan_device(vendor_id, device_id, bus, device, device_type);
		if(t.bits)
			return t;
	}
	return dev_zero;
}


pci_dev_t pci_get_device(uint16_t vendor_id, uint16_t device_id, int32_t device_type) {
	pci_dev_t t = pci_scan_bus(vendor_id, device_id, 0, device_type);

	if(t.bits){
		return t;
	}

	// Handle multiple pci host controllers

	if(pci_reach_end(dev_zero)) {
		qemu_log("PCI Get device failed");
	}

	for(int32_t function = 1; function < FUNCTION_PER_DEVICE; function++) {
		pci_dev_t dev = {0};
		dev.function_num = function;

		if(pci_read(dev, PCI_VENDOR_ID) == PCI_NONE)
			break;
		
		t = pci_scan_bus(vendor_id, device_id, function, device_type);
		
		if(t.bits){
			return t;
		}
	}
	qemu_log("dev zero!");
	return dev_zero;
}

void pci_write(pci_dev_t dev, uint32_t field, uint32_t value) {
	dev.field_num = (field & 0xFC) >> 2;
	dev.enable = 1;
	// Tell where we want to write
	outl(PCI_CONFIG_ADDRESS, dev.bits);
	// Value to write
	outl(PCI_CONFIG_DATA, value);
}

void pci_init() {
	// Init size map
	pci_size_map[PCI_VENDOR_ID] =	2;
	pci_size_map[PCI_DEVICE_ID] =	2;
	pci_size_map[PCI_COMMAND]	=	2;
	pci_size_map[PCI_STATUS]	=	2;
	pci_size_map[PCI_SUBCLASS]	=	1;
	pci_size_map[PCI_CLASS]		=	1;
	pci_size_map[PCI_CACHE_LINE_SIZE]	= 1;
	pci_size_map[PCI_LATENCY_TIMER]		= 1;
	pci_size_map[PCI_HEADER_TYPE] = 1;
	pci_size_map[PCI_BIST] = 1;
	pci_size_map[PCI_BAR0] = 4;
	pci_size_map[PCI_BAR1] = 4;
	pci_size_map[PCI_BAR2] = 4;
	pci_size_map[PCI_BAR3] = 4;
	pci_size_map[PCI_BAR4] = 4;
	pci_size_map[PCI_BAR5] = 4;
	pci_size_map[PCI_INTERRUPT_LINE]	= 1;
	pci_size_map[PCI_SECONDARY_BUS]		= 1;
	
	qemu_log("PCI installed");
}
