#include <kernel.h>


int ethernet_send_packet(uint8_t * dst_mac_addr, uint8_t * data, int len, uint16_t protocol) {
    return len;
}

void ethernet_handle_packet(ethernet_frame_t * packet, int len) {
}

void ethernet_init() {
}
