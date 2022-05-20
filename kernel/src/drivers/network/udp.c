#include <kernel.h>


uint16_t udp_calculate_checksum(udp_packet_t * packet) {
    return 0;
}

void udp_send_packet(uint8_t * dst_ip, uint16_t src_port, uint16_t dst_port, void * data, int len) {
}

void udp_handle_packet(udp_packet_t * packet) {
}
