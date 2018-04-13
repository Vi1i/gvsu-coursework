#include <stdio.h>              // printf
#include <stdlib.h>             // malloc
#include <unistd.h>             // close
//#include <sys/types.h>        // 
#include <sys/socket.h>         // socket
#include <arpa/inet.h>          // htons
#include <netinet/if_ether.h>   // ETH_P_ALL
#include <netinet/ip.h>         // iphdr struct
#include <net/ethernet.h>       // ether_header

#include <test.h>

void process_packet(unsigned char * buffer, size_t size);
void print_tcp_packet(unsigned char * buffer, size_t size);
void print_udp_packet(unsigned char * buffer, size_t size);
void print_other_packet(unsigned char * buffer, size_t size);

int main(int argc, char * argv[]) {
    printf("%d", TEST);
    size_t saddr_size;
    size_t data_size;

    struct sockaddr saddr;

    unsigned char * buffer = (unsigned char *)malloc(65536);

    int sock_raw = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));

    if(sock_raw < 0) {
        perror("Socket Error\n");
        return -1;
    }

    while(1) {
        saddr_size = sizeof(saddr);

        data_size = recvfrom(sock_raw, buffer, 65536, 0, &saddr, (socklen_t *)&saddr_size);

        if(data_size < 0) {
            perror("recvfrom error, failed to get packets\n");
            return -1;
        }

        process_packet(buffer, data_size);
    }

    close(sock_raw);
    return 0;
}

void process_packet(unsigned char * buffer, size_t size) {
    struct iphdr * iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));

    switch(iph->protocol) {
        case 6:
            print_tcp_packet(buffer, size);
            break;
        case 17:
            print_udp_packet(buffer, size);
            break;
        default:
            print_other_packet(buffer, size);
            break;
    }
}

void print_tcp_packet(unsigned char * buffer, size_t size) {
    printf("TCP\n");
}

void print_udp_packet(unsigned char * buffer, size_t size) {
    printf("UDP\n");
}

void print_other_packet(unsigned char * buffer, size_t size) {
    struct iphdr * iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    printf("OTHER: %d\n", iph->protocol);
}
