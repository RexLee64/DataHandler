/*
 * @Author: Rex 
 * @Date: 2018-09-25 10:58:51 
 * @Last Modified by:  
 * @Last Modified time: 2018-09-28 16:42:05
 */
#include "test.h"
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
#include <iostream>
#include "OrderBook.h"

#define BUFF_SIZE 2048
static const char *g_szIfName = "ens3f1np1"; // 网卡接口

typedef struct XdpPacketHeader
{
    unsigned short mPktSize;
    unsigned char mMsgCount;
    unsigned char mFiller;
    unsigned int mSeqNum;
    unsigned long long mSendTime;
} PacketHeader;

typedef struct XdpMessageHeader
{
    unsigned short mMsgSize;
    unsigned short mMsgType;
} MessageHeader; 

    // 增加/修改订单
    //     typedef struct XdpAddModOrder
    // {
    //     unsigned int mOrderbookID;
    //     unsigned long long mOrderID;
    //     int mPrice;
    //     unsigned int mQuantity;
    //     unsigned char mSide;
    //     unsigned char mLotType;
    //     unsigned short mOrderType;
    //     unsigned int mOrderbookPosition;
    // } AddModOrder;
 

void ProcessMessageHeader(char *buf, int msgCount);
void AddOrder(char *buf, uint16_t offset, uint16_t len);

int main(int argc, char *argv[])
{
    int sockfd;
    int n;
    char buf[BUFF_SIZE];
    char read_buf[BUFF_SIZE];
    struct sockaddr_ll sll;
    struct ifreq ifr;
    struct ethhdr *eth;
    struct iphdr *iph;
    struct udphdr *udph;
    printf("%d,%d,%d,%d,%d\n", sizeof(unsigned short), sizeof(unsigned char), sizeof(unsigned int), sizeof(unsigned long), sizeof(unsigned long long));
    sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    if (sockfd == -1)
    {
        printf("sockt error!\n");
        return -1;
    }
    strncpy(ifr.ifr_name, g_szIfName, IFNAMSIZ);
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1) // 获取网卡信息
    {
        perror("ioctl()");
        close(sockfd);
        return -1;
    }

    // ifr.ifr_flags |= IFF_PROMISC;
    // if(ioctl(sockfd, SIOCSIFFLAGS, &ifr) == -1) // 网卡设置混杂模式
    // {
    //     perror("ioctl()");
    //     close(sockfd);
    //     return -1;
    // }
    memset(&sll, 0, sizeof(sll));
    sll.sll_family = PF_PACKET;
    sll.sll_ifindex = ifr.ifr_ifindex;
    sll.sll_protocol = htons(ETH_P_IP);
    if (bind(sockfd, (struct sockaddr *)&sll, sizeof(sll)) == -1)
    {
        perror("bind()");
        close(sockfd);
        return -1;
    }
    int i = 0;
    while (1)
    {
        if (i++ == 10)
            break;
        n = recvfrom(sockfd, buf, sizeof(buf), 0, NULL, NULL);
        if (n == -1)
        {
            perror("recvfrom()");
            break;
        }

        // buf[strlen(buf)] = '\0';
        // 接收到的数据帧头6字节是目的MAC地址，紧接着6字节是源MAC地址。
        eth = (struct ethhdr *)buf;
        // printf("Dest MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
        // printf("Source MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
        iph = (struct iphdr *)(buf + sizeof(struct ethhdr));
        // if (iph->version == 4 && iph->ihl == 5)
        // {
        //     printf("version:%d,ihl:%d\n",iph->version,iph->ihl);
        //     printf("Source:%s\n", inet_ntoa(*(struct in_addr *) &iph->saddr));
        //     printf("Dest:%s\n", inet_ntoa(*(struct in_addr *) &iph->daddr));
        // }
        if (iph->protocol == IPPROTO_UDP)
        {
            udph = (struct udphdr *)(buf + sizeof(struct ethhdr) + sizeof(struct iphdr));
            char *packetPtr = buf + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr);
            PacketHeader *hdr = (PacketHeader *)(packetPtr);
            char *msgPtr = packetPtr + 16; // packet header len = 16bytes
            char *ip = inet_ntoa(*(struct in_addr *)&iph->daddr);
            int port = ntohs(udph->dest);
            if (hdr->mMsgCount > 0)
            {
                // if (strcmp(ip, "239.1.127.130") == 0 && port == 51001)
                // {
                //     printf("\n==================================================channel id:121 239.1.127.130:51001======================================================\n");
                //     printf("PktSize:%hu,", hdr->mPktSize);
                //     printf("MsgCount:%hu,", hdr->mMsgCount);
                //     printf("SeqNum:%lu\n", hdr->mSeqNum);
                //     ProcessMessageHeader(msgPtr, hdr->mMsgCount);
                // }
                // if (strcmp(ip, "239.1.127.130") == 0 && port == 51002)
                // {
                //     printf("\n==================================================channel id:221 239.1.127.130:51002======================================================\n");
                //     printf("PktSize:%hu,", hdr->mPktSize);
                //     printf("MsgCount:%hu,", hdr->mMsgCount);
                //     printf("SeqNum:%lu\n", hdr->mSeqNum);
                //     ProcessMessageHeader(msgPtr, hdr->mMsgCount);
                // }
                // if(strcmp(ip, "239.1.127.139") == 0 && port == 51000)
                // {
                //     printf("\n==================================================239.1.127.139:51000======================================================\n");
                //     ProcessMessageHeader(msgPtr, hdr->mMsgCount);
                // }
            }

            // switch (ntohs(udph->dest))
            // {
            // case 51000:
            //     printf("\n==================================================51000======================================================\n");
            //     printf("Source:%s\n", inet_ntoa(*(struct in_addr *)&iph->saddr));
            //     printf("Dest:%s\n", inet_ntoa(*(struct in_addr *)&iph->daddr));
            //     printf("PktSize:%hu,", hdr->mPktSize);
            //     printf("MsgCount:%hu,", hdr->mMsgCount);
            //     printf("filler:%c,", hdr->mFiller);
            //     printf("SeqNum:%lu,", hdr->mSeqNum);
            //     printf("sendTime:%llu\n", hdr->mSendTime);
            //     ProcessMessageHeader(msgPtr, hdr->mMsgCount);
            //     break;
            // case 51001:
            //     printf("\n==================================================51001======================================================\n");
            //     printf("Source:%s\n", inet_ntoa(*(struct in_addr *)&iph->saddr));
            //     printf("Dest:%s\n", inet_ntoa(*(struct in_addr *)&iph->daddr));
            //     printf("PktSize:%hu,", hdr->mPktSize);
            //     printf("MsgCount:%hu,", hdr->mMsgCount);
            //     printf("filler:%c,", hdr->mFiller);
            //     printf("SeqNum:%lu,", hdr->mSeqNum);
            //     printf("sendTime:%llu\n", hdr->mSendTime);
            //     ProcessMessageHeader(msgPtr, hdr->mMsgCount);
            //     break;
            // case 51002:
            //     printf("\n==================================================51002======================================================\n");
            //     printf("Source:%s\n", inet_ntoa(*(struct in_addr *)&iph->saddr));
            //     printf("Dest:%s\n", inet_ntoa(*(struct in_addr *)&iph->daddr));
            //     printf("PktSize:%hu,", hdr->mPktSize);
            //     printf("MsgCount:%hu,", hdr->mMsgCount);
            //     printf("filler:%c,", hdr->mFiller);
            //     printf("SeqNum:%lu,", hdr->mSeqNum);
            //     printf("sendTime:%llu\n", hdr->mSendTime);
            //     ProcessMessageHeader(msgPtr, hdr->mMsgCount);
            //     break;
            // case 51003:
            //     printf("\n==================================================51003======================================================\n");
            //     printf("Source:%s\n", inet_ntoa(*(struct in_addr *)&iph->saddr));
            //     printf("Dest:%s\n", inet_ntoa(*(struct in_addr *)&iph->daddr));
            //     printf("PktSize:%hu,", hdr->mPktSize);
            //     printf("MsgCount:%hu,", hdr->mMsgCount);
            //     printf("filler:%c,", hdr->mFiller);
            //     printf("SeqNum:%lu,", hdr->mSeqNum);
            //     printf("sendTime:%llu\n", hdr->mSendTime);
            //     ProcessMessageHeader(msgPtr, hdr->mMsgCount);
            //     break;
            // case 51004:
            //     printf("\n==================================================51004======================================================\n");
            //     printf("Source:%s\n", inet_ntoa(*(struct in_addr *)&iph->saddr));
            //     printf("Dest:%s\n", inet_ntoa(*(struct in_addr *)&iph->daddr));
            //     printf("PktSize:%hu,", hdr->mPktSize);
            //     printf("MsgCount:%hu,", hdr->mMsgCount);
            //     printf("filler:%c,", hdr->mFiller);
            //     printf("SeqNum:%lu,", hdr->mSeqNum);
            //     printf("sendTime:%llu\n", hdr->mSendTime);
            //     ProcessMessageHeader(msgPtr, hdr->mMsgCount);
            //     break;
            // case 51005:
            //     printf("\n==================================================51005======================================================\n");
            //     printf("Source:%s\n", inet_ntoa(*(struct in_addr *)&iph->saddr));
            //     printf("Dest:%s\n", inet_ntoa(*(struct in_addr *)&iph->daddr));
            //     printf("PktSize:%hu,", hdr->mPktSize);
            //     printf("MsgCount:%hu,", hdr->mMsgCount);
            //     printf("filler:%c,", hdr->mFiller);
            //     printf("SeqNum:%lu,", hdr->mSeqNum);
            //     printf("sendTime:%llu\n", hdr->mSendTime);
            //     ProcessMessageHeader(msgPtr, hdr->mMsgCount);
            //     break;
            // case 51006:
            //     printf("\n==================================================51006======================================================\n");
            //     printf("Source:%s\n", inet_ntoa(*(struct in_addr *)&iph->saddr));
            //     printf("Dest:%s\n", inet_ntoa(*(struct in_addr *)&iph->daddr));
            //     printf("PktSize:%hu,", hdr->mPktSize);
            //     printf("MsgCount:%hu,", hdr->mMsgCount);
            //     printf("filler:%c,", hdr->mFiller);
            //     printf("SeqNum:%lu,", hdr->mSeqNum);
            //     printf("sendTime:%llu\n", hdr->mSendTime);
            //     ProcessMessageHeader(msgPtr, hdr->mMsgCount);
            //     break;
            // }

            // printf("Dest MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
            // printf("Source MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
            // // printf("version:%d,ihl:%d\n", iph->version, iph->ihl);
            // printf("Source:%s\n", inet_ntoa(*(struct in_addr *)&iph->saddr));
            // printf("Dest:%s\n", inet_ntoa(*(struct in_addr *)&iph->daddr));
            // udph = (struct udphdr *)(buf + sizeof(struct ethhdr) + sizeof(struct iphdr));
            // printf("Source port: %u\n", ntohs(udph->source));
            // printf("Dest port: %u\n", ntohs(udph->dest));
            // printf("udp-len:%d\n", ntohs(udph->len));
            // udph = (UDP_HEADER *)(iph + sizeof(UDP_HEADER));
            // printf("Source port: %u\n", ntohs(udph->uh_sport));
            // printf("Dest port: %u\n", ntohs(udph->uh_dport));
            // printf("len: %u\n", ntohs(udph->uh_len));
            // printf("====================================================\n");
        }
    }
    close(sockfd);
    return 0;
}

void ProcessMessageHeader(char *buf, int msgCount)
{
    int n = 0;
    int size = 0;
    while (n < msgCount)
    {
        MessageHeader *msghdr = (MessageHeader *)buf;
        printf("msg %d: msgsize=%d, msgtype=%d\n", n, msghdr->mMsgSize, msghdr->mMsgType);
        if (msghdr->mMsgType == ADDORDER)
        {
            AddOrder(buf, 4, msghdr->mMsgSize);
        }
        n++;
        buf = buf + msghdr->mMsgSize;
        size += msghdr->mMsgSize;
    }
    printf("total size=%d\n", size + 16);
}

void AddOrder(char *buf, uint16_t offset, uint16_t len)
{
    XdpAddModOrder addOrder(buf, len, offset);
    printf("OrderbookID:%u\n", addOrder.orderBookId());
    printf("OrderID:%llu\n", addOrder.orderId());
    printf("price:%d\n", addOrder.price());
    printf("quantity:%u\n", addOrder.quantity());
    printf("side:%hu\n", addOrder.side());
    printf("lotType:%d\n", addOrder.lotType());
    printf("OrderType:%d\n", addOrder.orderType());
    printf("orderBookPosition:%u\n\n", addOrder.orderBookPosition());

}