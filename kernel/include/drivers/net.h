#pragma once

#include <stdint.h>



#define ARP_REQUEST 1
#define ARP_REPLY 2


#define DHCP_REQUEST 1
#define DHCP_REPLY 2
#define DHCP_TRANSACTION_IDENTIFIER 0x55555555


#define ETHERNET_TYPE_ARP 0x0806
#define ETHERNET_TYPE_IP 0x0800
#define HARDWARE_TYPE_ETHERNET 0x01

#define IP_IPV4 4
#define IP_PACKET_NO_FRAGMENT 2
#define IP_IS_LAST_FRAGMENT 4
#define PROTOCOL_UDP 17
#define PROTOCOL_TCP 6


#define RTL8139_VENDOR_ID 0x10EC
#define RTL8139_ret_ID 0x8139


typedef struct ip_packet {
    char version_ihl_ptr[0];
    uint8_t version:4;
    uint8_t ihl:4;
    uint8_t tos;
    uint16_t length;
    uint16_t id;
    char flags_fragment_ptr[0];
    uint8_t flags:3;
    uint8_t fragment_offset_high:5;
    uint8_t fragment_offset_low;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t header_checksum;
    uint8_t src_ip[4];
    uint8_t dst_ip[4];
    uint8_t data[];
} __attribute__((packed)) ip_packet_t;

typedef struct dhcp_packet {
    uint8_t op;
    uint8_t hardware_type;
    uint8_t hardware_addr_len;
    uint8_t hops;
    uint32_t xid;
    uint16_t seconds;
    uint16_t flags; // may be broken
    uint32_t client_ip; //
    uint32_t your_ip;
    uint32_t server_ip;
    uint32_t gateway_ip;
    uint8_t client_hardware_addr[16];
    uint8_t server_name[64];
    uint8_t file[128];
    uint8_t options[64];
} __attribute__ ((packed)) dhcp_packet_t;


/**
 * @brief Структура пакета ARP
 * 
 */
typedef struct arp_packet {
    uint16_t hardware_type;
    uint16_t protocol;
    uint8_t hardware_addr_len;
    uint8_t protocol_addr_len;
    uint16_t opcode;
    uint8_t src_hardware_addr[6];
    uint8_t src_protocol_addr[4];
    uint8_t dst_hardware_addr[6];
    uint8_t dst_protocol_addr[4];
} __attribute__((packed)) arp_packet_t;


/**
 * @brief Структура точки входа ARP
 * 
 */
typedef struct arp_table_entry {
    uint32_t ip_addr;
    uint64_t mac_addr;
} arp_table_entry_t;


typedef struct ethernet_frame {
  uint8_t dst_mac_addr[6];    // MAC адрес получателя
  uint8_t src_mac_addr[6];    // MAC адрес отправителя
  uint16_t type;              // Протокол
  uint8_t data[];             // Данные
} __attribute__((packed)) ethernet_frame_t;


/**
 * @brief Структура для работы с RTL8139
 * 
 */
typedef struct RTL8139_dev {
    uint8_t bar_type;
    uint16_t io_base;
    uint32_t mem_base;
    int eeprom_exist;
    uint8_t mac_addr[6];
    char * rx_buffer;
    int tx_cur;
}RTL8139_dev_t;


/**
 * @brief Структура UDP пакета
 * 
 */
typedef struct udp_packet {
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t length;
    uint16_t checksum;
    uint8_t data[];
} __attribute__((packed)) udp_packet_t;



uint16_t udp_calculate_checksum(udp_packet_t * packet);
void udp_send_packet(uint8_t * dst_ip, uint16_t src_port, uint16_t dst_port, void * data, int len);
void udp_handle_packet();


int gethostaddr(char * addr);
void dhcp_discover();
void dhcp_request(uint8_t * request_ip);
void dhcp_handle_packet(dhcp_packet_t * packet);
void * get_dhcp_options(dhcp_packet_t * packet, uint8_t type);
void make_dhcp_packet(dhcp_packet_t * packet, uint8_t msg_type, uint8_t * request_ip);