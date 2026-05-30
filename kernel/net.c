// ==============================================================================
// SETIN - net.c (Network Packet Processing Engine)
// ==============================================================================

#include "net.h"
#include "system.h"

// Big-endian to Little-endian macro for 16-bit integers (Network Byte Order)
#define swap_uint16(x) (((x) >> 8) | ((x) << 8))

void net_handle_packet(unsigned char *packet_buffer, unsigned short length) {
    // Check if the packet is too small to even contain an Ethernet header
    if (length < sizeof(struct ethernet_header)) {
        safe_print("[NET ERROR] Packet dropped: Too small for Ethernet.\n");
        return;
    }

    // Cast the raw buffer into an Ethernet structure
    struct ethernet_header *eth = (struct ethernet_header *)packet_buffer;

    // Convert type from Network Byte Order (Big Endian) to Host Byte Order (Little Endian)
    unsigned short eth_type = swap_uint16(eth->type);

    if (eth_type == 0x0806) {
        // ARP Packet detected
        safe_print("[NET] Received ARP packet (Address Resolution).\n");
    } 
    else if (eth_type == 0x0800) {
        // IPv4 Packet detected
        safe_print("[NET] Received IPv4 packet. Parsing Layer 3...\n");

        // Advanced pointer arithmetic to skip the Ethernet header and find the IP header
        struct ipv4_header *ip = (struct ipv4_header *)(packet_buffer + sizeof(struct ethernet_header));

        // Check protocol inside IP header
        if (ip->protocol == 1) {
            safe_print("[NET] Ping request (ICMP) detected!\n");
        } 
        else if (ip->protocol == 17) {
            safe_print("[NET] UDP packet detected!\n");
        } 
        else if (ip->protocol == 6) {
            safe_print("[NET] TCP packet detected!\n");
        }
    } 
    else {
        // Unknown network protocol
        safe_print("[NET] Unknown protocol type received.\n");
    }
}
