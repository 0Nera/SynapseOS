#include <kernel.h>


arp_table_entry_t arp_table[512];
int arp_table_size;
int arp_table_curr;

uint8_t broadcast_mac_address[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

void arp_handle_packet(arp_packet_t * arp_packet, int len) {
    char dst_hardware_addr[6];
    char dst_protocol_addr[4];

    // Сохранеяем некоторые данные пакета
    memcpy(dst_hardware_addr, arp_packet->src_hardware_addr, 6);
    memcpy(dst_protocol_addr, arp_packet->src_protocol_addr, 4);
}

void arp_send_packet(uint8_t * dst_hardware_addr, uint8_t * dst_protocol_addr) {
}

void arp_lookup_add(uint8_t * ret_hardware_addr, uint8_t * ip_addr) {
}

int arp_lookup(uint8_t * ret_hardware_addr, uint8_t * ip_addr) {
    return -1;
}

void arp_init() {
    uint8_t broadcast_ip[4];
    uint8_t broadcast_mac[6];

    memset(broadcast_ip, 0xff, 4);
    memset(broadcast_mac, 0xff, 6);
}
