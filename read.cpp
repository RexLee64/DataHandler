#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <linux/if_packet.h>
#include <netinet/if_ether.h>
#define BUFF_SIZE 2048
typedef struct XdpPacketHeader
{
    unsigned short mPktSize;
    unsigned char mMsgCount;
    unsigned char mFiller;
    unsigned long mSeqNum;
    unsigned long long mSendTime;
} PacketHeader;


int main(int argc, char *argv[])
{
    int sockfd;
    int n;
    char read_buf[BUFF_SIZE];
    struct sockaddr_ll sll;
    struct ifreq ifr;
    struct ethhdr *eth;
    struct iphdr *iph;
    struct udphdr *udph;

    FILE *fp;
    fp = fopen(argv[1], "rb+");
    fseek(fp, 0, SEEK_SET);
    // while(!feof(fp))
    while (fread(read_buf, sizeof(read_buf), 1, fp))
    {
        // 接收到的数据帧头6字节是目的MAC地址，紧接着6字节是源MAC地址。
        eth = (struct ethhdr *)read_buf;
        printf("Dest MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
        printf("Source MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
        iph = (struct iphdr *)(read_buf + sizeof(struct ethhdr));
        // if (iph->version == 4 && iph->ihl == 5)
        // {
        //     printf("version:%d,ihl:%d\n",iph->version,iph->ihl);
        //     printf("Source:%s\n", inet_ntoa(*(struct in_addr *) &iph->saddr));
        //     printf("Dest:%s\n", inet_ntoa(*(struct in_addr *) &iph->daddr));
        // }
        if (iph->protocol == IPPROTO_UDP)
        {
            udph = (struct udphdr *)(buf + sizeof(struct ethhdr) + sizeof(struct iphdr));
            printf("Dest MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
            printf("Source MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
            printf("Source:%s\n", inet_ntoa(*(struct in_addr *)&iph->saddr));
            printf("Dest:%s\n", inet_ntoa(*(struct in_addr *)&iph->daddr));
            printf("Source port: %u\n", ntohs(udph->source));
            printf("Dest port: %u\n", ntohs(udph->dest));
            printf("udp-len:%d\n", ntohs(udph->len));

            char *packetPtr = buf + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr);
            switch (ntohs(udph->dest))
            {
            case 51000:
                PacketHeader *hdr = static_cast<PacketHeader *>(packetPtr);
                printf("PktSize:%d,", hdr->mPktSize);
                printf("MsgCount:%d,", hdr->mMsgCount);
                printf("filler:%c,", hdr->mFiller);
                printf("SeqNum:%d,", hdr->mSeqNum);
                printf("sendTime:%lu\n", hdr->mSendTime);
                break;
            }
        }
    }
    fclose(fp);
    return 0;
}