// ==============================================================================
// SETIN - net.h (Network Stack Definitions)
// ==============================================================================

#ifndef NET_H
#define NET_H

#include "setin.h"

// Standard 48-bit MAC Address structure
struct mac_address {
    unsigned char addr[6];
} __attribute__((packed));

// Standard Ethernet Frame Header (Layer 2)
struct ethernet_header {
    struct mac_address dest_mac;
    struct mac_address src_mac;
    unsigned short type;          // Type of packet (e.g., 0x0800 for IPv4, 0x0806 for ARP)
} __attribute__((packed));

// Standard IPv4 Header (Layer 3)
struct ipv4_header {
    unsigned char  version_ihl;   // Version (4 bits) + Internet Header Length (4 bits)
    unsigned char  tos;           // Type of Service
    unsigned short total_length;  // Total length of the packet
    unsigned short id;            // Identification
    unsigned short flags_fragment;// Flags (3 bits) + Fragment Offset (13 bits)
    unsigned char  ttl;           // Time to Live
    unsigned char  protocol;      // Protocol (e.g., 1=ICMP/Ping, 6=TCP, 17=UDP)
    unsigned short checksum;      // Header Checksum
    unsigned int   src_ip;        // Source IP Address
    unsigned int   dest_ip;       // Destination IP Address
} __attribute__((packed));

// Function prototypes for the Setin network stack
void net_handle_packet(unsigned char *packet_buffer, unsigned short length);

#endif
