#include <kernel.h>


RTL8139_dev_t RTL8139_device;
pci_dev_t pci_RTL8139_device;
int32_t RTL8139_io_addres = 0; 
uint32_t RTL8139_ret;
uint32_t RTL8139_raw_mac[2];


// Четыре регистра TXAD, вы должны каждый раз использовать другой для отправки пакета (например, использовать первый, второй... четвертый и обратно к первому)
uint8_t TSAD_array[4] = {0x20, 0x24, 0x28, 0x2C};
uint8_t TSD_array[4] = {0x10, 0x14, 0x18, 0x1C};
/*
static struct board board_tbl[] = {
    {"RealTek", "RealTek RTL8139+", BUSTYPE_PCI, PCI_UNITCODE(0x10ec, 0x8139), 0xffffffff, 0, 0, 0x20, 0xff, RTL8139D_CAPS},
    {"RealTek", "RealTek RTL8139C Fast Ethernet", BUSTYPE_PCI, PCI_UNITCODE(0x10ec, 0x8139), 0xffffffff, 0, 0, 0x10, 0xff, RTL8139_CAPS},
    {"RealTek", "RealTek RTL8129 Fast Ethernet", BUSTYPE_PCI, PCI_UNITCODE(0x10ec, 0x8129), 0xffffffff, 0, 0, 0, 0, RTL8129_CAPS},
    {"RealTek", "RealTek RTL8139 Fast Ethernet", BUSTYPE_PCI, PCI_UNITCODE(0x10ec, 0x8139), 0xffffffff, 0, 0, 0, 0, RTL8139_CAPS},
    {"RealTek", "RealTek RTL8139B PCI",  BUSTYPE_PCI, PCI_UNITCODE(0x10ec, 0x8138), 0xffffffff, 0, 0, 0, 0, RTL8139_CAPS},
    {"Accton", "Accton EN-1207D Fast Ethernet Adapter", BUSTYPE_PCI, PCI_UNITCODE(0x1113, 0x1211), 0xffffffff, PCI_UNITCODE(0x1113, 0x9211), 0xffffffff, 0, 0, RTL8139_CAPS},
    {"SMC", "SMC1211TX EZCard 10/100 (RealTek RTL8139)", BUSTYPE_PCI, PCI_UNITCODE(0x1113, 0x1211), 0xffffffff, 0, 0, 0, 0, RTL8139_CAPS},
    {"D-Link", "D-Link DFE-538TX (RTL8139)", BUSTYPE_PCI, PCI_UNITCODE(0x1186, 0x1300), 0xffffffff, 0, 0, 0, 0, RTL8139_CAPS},
    {"LevelOne", "LevelOne FPC-0106Tx (RTL8139)", BUSTYPE_PCI, PCI_UNITCODE(0x018a, 0x0106), 0xffffffff, 0, 0, 0, 0, RTL8139_CAPS},
    {"Compaq", "Compaq HNE-300 (RTL8139c)", BUSTYPE_PCI, PCI_UNITCODE(0x021b, 0x8139), 0xffffffff, 0, 0, 0, 0, RTL8139_CAPS},
    {"Generic", "Generic RTL8139", BUSTYPE_PCI, 0, 0, 0, 0, 0, 0, RTL8139_CAPS},
    {NULL,},
};*/

// Получение MAC адреса
void RTL8139_read_mac_addr() {

    RTL8139_raw_mac[0] = inl(RTL8139_device.io_base + 0x00);
    RTL8139_raw_mac[1] = ins(RTL8139_device.io_base + 0x04);

    RTL8139_device.mac_addr[0] = RTL8139_raw_mac[0] >> 0;
    RTL8139_device.mac_addr[1] = RTL8139_raw_mac[0] >> 8;
    RTL8139_device.mac_addr[2] = RTL8139_raw_mac[0] >> 16;
    RTL8139_device.mac_addr[3] = RTL8139_raw_mac[0] >> 24;

    RTL8139_device.mac_addr[4] = RTL8139_raw_mac[1] >> 0;
    RTL8139_device.mac_addr[5] = RTL8139_raw_mac[1] >> 8;

    tty_printf("MAC Address: %v:%v:%v:%v:%v:%v\n", 
        RTL8139_device.mac_addr[0], RTL8139_device.mac_addr[1], RTL8139_device.mac_addr[2], 
        RTL8139_device.mac_addr[3], RTL8139_device.mac_addr[4], RTL8139_device.mac_addr[5]
        );
    punch();
}


// Перезагружает RTL8139
void RTL8139_reset() {
    outb(RTL8139_device.io_base + 0x37, 0x10);
    while((inb(RTL8139_device.io_base + 0x37) & 0x10) != 0);
}


// Сохраняет MAC адресс в src_mac_addr
void RTL8139_get_mac_addr(uint8_t src_mac_addr[6]){
    memcpy(src_mac_addr, RTL8139_device.mac_addr, 6);
}


// Отправка пакета
void RTL8139_send_packet() {

}


// Прием прерываний
void RTL8139_handler(struct regs *r){
    tty_printf("RTL8139 interupt %d\n", r->idt_index);

    uint16_t status = ins(RTL8139_device.io_base + 0x3e);   // Получение статуса

    if(status & (1<<2)) {                                   // Успешно(?) отправлено
        tty_printf("RTL8139: Sent\n");
    }
    if (status & (1<<0)) {                                  // Принято отправлено
        tty_printf("RTL8139: Received\n");
        //receive_packet();
    }
    
    outs(RTL8139_device.io_base + 0x3E, 0x5);               // Говорим сетевухе чтоб дальше работала
}


// Настраивает RTL8139
int RTL8139_init() {
    outb(RTL8139_io_addres + 0x52, 0x0);                // Включаем RTL

    RTL8139_ret = pci_read(                           // Ищем среди устройств
            pci_get_device(RTL8139_VENDOR_ID, RTL8139_ret_ID, -1), 
            PCI_BAR0
            );

    if(RTL8139_ret == 0) {
        qemu_log("RTL8139 can't install");
        return -1;
    }

    pci_RTL8139_device = pci_get_device(RTL8139_VENDOR_ID, RTL8139_ret_ID, -1);

    RTL8139_device.bar_type = RTL8139_ret & 0x1;
    
    RTL8139_device.io_base = RTL8139_ret & (~0x3);    // Получаем базу ввода-вывода или базу памяти, извлекая старшие 28/30 бит
    RTL8139_device.mem_base = RTL8139_ret & (~0xf);
    tty_printf("RTL8139 use %s, base: %x\n", (RTL8139_device.bar_type == 0)? "mem based":"port based", (RTL8139_device.bar_type != 0)?RTL8139_device.io_base:RTL8139_device.mem_base);
    
    RTL8139_device.tx_cur = 0;                        // Устанавливаем текущий TSAD

    // Включаем для PCI
    uint32_t pci_command_reg = pci_read(pci_RTL8139_device, PCI_COMMAND);
    if(!(pci_command_reg & (1 << 2))) {
        pci_command_reg |= (1 << 2);
        pci_write(pci_RTL8139_device, PCI_COMMAND, pci_command_reg);
    }
    // Отправляем 0x00 в регистр CONFIG_1 (0x52), чтобы установить LWAKE + LWPTN как активный. (Включаем)
    outb(RTL8139_device.io_base + 0x52, 0x0);

    RTL8139_reset();    // Перезапуск

    // Выделяем буффер
    RTL8139_device.rx_buffer = kheap_malloc(8192 + 16 + 1500);
    memset(RTL8139_device.rx_buffer, 0x0, 8192 + 16 + 1500);
    outl(RTL8139_device.io_base + 0x30, (uint32_t)kv2p(RTL8139_device.rx_buffer));

    // Устанавливает биты TOK и ROK в высокий уровень
    outs(RTL8139_device.io_base + 0x3C, 0x0005);

    // (1 << 7) — это бит WRAP, 0xf — это AB+AM+APM+AAP. Ps. я ничего не понял тут
    outl(RTL8139_device.io_base + 0x44, 0xf | (1 << 7));

    // Устанавливает биты RE и TE в высокий уровень
    outb(RTL8139_device.io_base + 0x37, 0x0C);
    RTL8139_read_mac_addr();

    // Регистрация прерываний
    uint32_t irq_num = pci_read(pci_RTL8139_device, PCI_INTERRUPT_LINE);
    register_interrupt_handler(32 + irq_num, &RTL8139_handler);
    IRQ_clear_mask(irq_num);
    qemu_log("RTL8139 installed, idt: %d, irq: %d", 32 + irq_num, irq_num);
    

    return 1;
}
