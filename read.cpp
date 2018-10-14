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
    int n;
    char buf[BUFF_SIZE];
    struct sockaddr_ll sll;
    struct ifreq ifr;
    struct ethhdr *eth;
    struct iphdr *iph;
    struct udphdr *udph;

    FILE *fp;
    fp = fopen(argv[1], "rb+");
    fseek(fp, 0, SEEK_SET);
    int i = 0;
    while (fread(buf, sizeof(buf), 1, fp))
    {  
        if (i++ == 10)
            break;
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
                if (strcmp(ip, "239.1.127.130") == 0 && port == 51001)
                {
                    printf("\n==================================================channel id:121 239.1.127.130:51001======================================================\n");
                    printf("PktSize:%hu,", hdr->mPktSize);
                    printf("MsgCount:%hu,", hdr->mMsgCount);
                    printf("SeqNum:%lu\n", hdr->mSeqNum);
                    ProcessMessageHeader(msgPtr, hdr->mMsgCount);
                }
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
 
        }
    } 
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
    printf("orderBookPosition:%u\n\n", addOrder.orderbookPosition());

}