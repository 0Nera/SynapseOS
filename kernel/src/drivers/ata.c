#include <kernel.h>


pci_dev_t ata_device;

ata_dev_t primary_master = {.slave = 0};
ata_dev_t primary_slave = {.slave = 1};
ata_dev_t secondary_master = {.slave = 0};
ata_dev_t secondary_slave = {.slave = 1};



void ata_io_wait(ata_dev_t * dev) {
    inb(dev->alt_status);
    inb(dev->alt_status);
    inb(dev->alt_status);
    inb(dev->alt_status);
}

void software_reset(ata_dev_t * dev) {
    outb(dev->control, CONTROL_SOFTWARE_RESET);
    ata_io_wait(dev);
    outb(dev->control, CONTROL_ZERO);
}


void ata_device_init(ata_dev_t * dev, int primary) {

    // Setup DMA
    // Prdt must not cross 64kb boundary / contiguous in physical memory. So simply allocate a 4kb aligned page satisfy both conditions
    dev->prdt = (void*)kheap_malloc(sizeof(prdt_t));
    memset(dev->prdt, 0, sizeof(prdt_t));
    //dev->prdt_phys = virtual2phys(kpage_dir, dev->prdt);
    dev->mem_buffer = (void*)kheap_malloc(4096);
    memset(dev->mem_buffer, 0, 4096);

    //dev->prdt[0].buffer_phys = (uint32_t)virtual2phys(kpage_dir, dev->mem_buffer);
    dev->prdt[0].transfer_size = SECTOR_SIZE;
    dev->prdt[0].mark_end = MARK_END;

    // Setup register address
    uint16_t base_addr = primary ? (0x1F0) : (0x170);
    uint16_t alt_status = primary ? (0x3F6) : (0x376);

    dev->data = base_addr;
    dev->error = base_addr + 1;
    dev->sector_count = base_addr + 2;
    dev->lba_lo = base_addr + 3;
    dev->lba_mid = base_addr + 4;
    dev->lba_high = base_addr + 5;
    dev->drive = base_addr + 6;
    dev->command = base_addr + 7;
    dev->alt_status = alt_status;


    dev->bar4 = pci_read(ata_device, PCI_BAR4);
    if(dev->bar4 & 0x1) {
        dev->bar4 = dev->bar4 & 0xfffffffc;
    }
    dev->BMR_COMMAND = dev->bar4;
    dev->BMR_STATUS = dev->bar4 + 2;
    dev->BMR_prdt = dev->bar4 + 4;

    // Set device's mountpoint(like /dev/hda)
    memset(dev->mountpoint, 0, 32);
    strcpy(dev->mountpoint, "/dev/hd");
    // Primary master(hda, 00), primary slave(hdb, 01), secondary master(hdc, 10), secondary slave(hdd, 11)
    dev->mountpoint[strlen(dev->mountpoint)] = 'a' + (((!primary) << 1) | dev->slave);
}


void ata_device_detect(ata_dev_t * dev, int primary) {
    qemu_printf("ata_device_init\n");
    // Must init some register address before detection
    ata_device_init(dev, primary);

    qemu_printf("software_reset\n");
    software_reset(dev);
    io_wait(dev);
    // Select drive, send 0xA0 to master device, 0xB0 to slave device
    outb(dev->drive, (0xA + dev->slave) << 4);
    // Set sector counts and LBAs to 0
    outb(dev->sector_count, 0);
    outb(dev->lba_lo, 0);
    outb(dev->lba_mid, 0);
    outb(dev->lba_high, 0);
    // Send identify command to command port

    qemu_printf("outb5\n");
    outb(dev->command, COMMAND_IDENTIFY);
    qemu_printf("inb\n");
    if(!inb(dev->status)) {
        qemu_printf("ata_detect_device: device does not exist\n");
        return;
    }

    uint8_t lba_lo = inb(dev->lba_lo);
    uint8_t lba_hi = inb(dev->lba_high);
    if(lba_lo != 0 || lba_hi != 0) {
        qemu_printf("ata_detect_device: not ata device\n");
        return;
    }
    // Polling
    uint8_t drq = 0, err = 0;
    // If either drq or err is set, stop the while loop
    while(!drq && !err) {
        drq = inb(dev->status) & STATUS_DRQ;
        err = inb(dev->status) & STATUS_ERR;
    }
    if(err) {
        qemu_printf("ata_detect_device: err when polling\n");
        return;
    }

    // Read 256 words(don't care the return value)
    for(int i = 0; i < 256; i++) ins(dev->data);

    uint32_t pci_command_reg = pci_read(ata_device, PCI_COMMAND);
    if(!(pci_command_reg & (1 << 2))) {
        pci_command_reg |= (1 << 2);
        qemu_printf("pci_write\n");
        pci_write(ata_device, PCI_COMMAND, pci_command_reg);
    }

    //vfs_storage_dev_t *dev = kheap_malloc(sizeof(vfs_storage_dev_t));
    //dev->type = 4;
    //strcpy(dev->name, "ata");

    /*vfs_filesystem_handles_t *fs_handles = kheap_malloc(sizeof(vfs_filesystem_handles_t));
    fs_handles->read = 0; //&void
    fs_handles->exists = 0;
    fs_handles->get_size = 0;
    fs_handles->is_dir = 0;
    fs_handles->write = 0;
    fs_handles->readdir = 0;
    fs_handles->mkfile = 0;
    fs_handles->mkdir = 0;
    fs_handles->rm = 0;
    //vfs_mount(dev, fs_handles, 1, "/ata/", 0);
    //vfs_mount(dev->mountpoint, create_ata_device(dev));*/
}


void ata_handler(struct regs *r) {
    tty_printf("ATA interrupt\n");
    qemu_printf("ATA interrupt\n");

    inb(primary_master.status);
    inb(primary_master.BMR_STATUS);
    outb(primary_master.BMR_COMMAND, BMR_COMMAND_DMA_STOP);
    //irq_ack(14);
}

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

    register_interrupt_handler(46, ata_handler);

	tty_printf("primary_master\n");
    ata_device_detect(&primary_master, 1);
	tty_printf("primary_slave\n");
    ata_device_detect(&primary_slave, 1);
	tty_printf("secondary_master\n");
    ata_device_detect(&secondary_master, 0);
	tty_printf("secondary_slave\n");
    ata_device_detect(&secondary_slave, 0);

    tty_printf("ATA installed\n");
    qemu_printf("ATA installed\n");
    //asm volatile("int $0x2E");
}