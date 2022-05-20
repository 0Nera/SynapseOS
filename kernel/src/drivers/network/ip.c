#include <kernel.h>


uint8_t my_ip[] = {10, 0, 2, 14};
uint8_t test_target_ip[] = {10, 0, 2, 15};
uint8_t zero_hardware_addr[] = {0,0,0,0,0,0};


void get_ip_str(char * ip_str, uint8_t * ip) {
    tty_printf("%s %d.%d.%d.%d", ip_str, ip[0], ip[1], ip[2], ip[3]);
}


uint16_t ip_calculate_checksum(ip_packet_t * packet) {
    int array_size = sizeof(ip_packet_t) / 2;
    uint16_t * array = (uint16_t*)packet;
    uint8_t * array2 = (uint8_t*)packet;
    uint32_t sum = 0;
    for(int i = 0; i < array_size; i++) {
        sum += flip_short(array[i]);
    }
    uint32_t carry = sum >> 16;
    sum = sum & 0x0000ffff;
    sum = sum + carry;
    uint16_t ret = ~sum;
    return ret;
}

void ip_send_packet(uint8_t * dst_ip, void * data, int len) {
}


void ip_handle_packet(ip_packet_t * packet) {
    
}
