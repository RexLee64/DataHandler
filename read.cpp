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
#include "OrderbookData.h"
#include "TradeData.h"
#include "ReferenceData.h"

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

void ProcessMessageHeader(char *buf, int msgCount);
void AddOrder(char *buf, uint16_t offset, uint16_t len);
void Trade(char *buf, uint16_t offset, uint16_t len);
void CommodityDefinition(char *buf, uint16_t offset, uint16_t len);
void SeriesDefinitionExtended(char *buf, uint16_t offset, uint16_t len);
void SeriesDefinitionBase(char *buf, uint16_t offset, uint16_t len);
void ClassDefinition(char *buf, uint16_t offset, uint16_t len);

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
        if (i++ == 100000)
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
                if (strcmp(ip, "239.1.127.153") == 0 && port == 51003)
                {
                    // printf("\n==================================================channel id:121 239.1.127.130:51001======================================================\n");
                    // printf("PktSize:%hu,", hdr->mPktSize);
                    // printf("MsgCount:%hu,", hdr->mMsgCount);
                    // printf("SeqNum:%lu\n", hdr->mSeqNum);
                    ProcessMessageHeader(msgPtr, hdr->mMsgCount);
                }
                // if (strcmp(ip, "239.1.127.128") == 0 && port == 51003)
                // {
                //     // printf("\n==================================================channel id:221 239.1.127.130:51002======================================================\n");
                //     // printf("PktSize:%hu,", hdr->mPktSize);
                //     // printf("MsgCount:%hu,", hdr->mMsgCount);
                //     // printf("SeqNum:%lu\n", hdr->mSeqNum);
                //     ProcessMessageHeader(msgPtr, hdr->mMsgCount);
                // }
                // if(strcmp(ip, "239.1.127.155") == 0 && port == 51001)
                // {
                //     printf("\n==================================================239.1.127.155:51001======================================================\n");
                //     printf("PktSize:%hu,", hdr->mPktSize);
                //     printf("MsgCount:%hu,", hdr->mMsgCount);
                //     printf("SeqNum:%lu\n", hdr->mSeqNum);
                //     ProcessMessageHeader(msgPtr, hdr->mMsgCount);
                // }
                //  if(strcmp(ip, "239.1.127.155") == 0 && port == 51002)
                // {
                //     printf("\n==================================================239.1.127.155:51002======================================================\n");
                //     printf("PktSize:%hu,", hdr->mPktSize);
                //     printf("MsgCount:%hu,", hdr->mMsgCount);
                //     printf("SeqNum:%lu\n", hdr->mSeqNum);
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
        // printf("msg %d: msgsize=%d, msgtype=%d\n", n, msghdr->mMsgSize, msghdr->mMsgType);
        // if (msghdr->mMsgType == ADDORDER)
        // {
        //     AddOrder(buf, 4, msghdr->mMsgSize);
        // }
        // if (msghdr->mMsgType == TRADE)
        // {
        //     Trade(buf, 4, msghdr->mMsgSize);
        // }

        // if (msghdr->mMsgType == COMMODITYDEFINITION)
        // {
        //     CommodityDefinition(buf, 4, msghdr->mMsgSize);
        // }
        if (msghdr->mMsgType == CLASSDEFINITION)
        {
            ClassDefinition(buf, 4, msghdr->mMsgSize);
        }
        // if (msghdr->mMsgType == SERIESDEFINITIONEXTENDED)
        // {
        //     SeriesDefinitionExtended(buf, 4, msghdr->mMsgSize);
        // }
        n++;
        buf = buf + msghdr->mMsgSize;
        size += msghdr->mMsgSize;
    }
    // printf("total size=%d\n", size + 16);
}

void AddOrder(char *buf, uint16_t offset, uint16_t len)
{
    XdpAddModOrder addOrder(buf, len, offset);
    printf("OrderbookID:%u\n", addOrder.orderbookId());
    printf("OrderID:%llu\n", addOrder.orderId());
    printf("price:%d\n", addOrder.price());
    printf("quantity:%u\n", addOrder.quantity());
    printf("side:%hu\n", addOrder.side());
    printf("lotType:%d\n", addOrder.lotType());
    printf("OrderType:%d\n", addOrder.orderType());
    printf("orderBookPosition:%u\n\n", addOrder.orderbookPosition());
}

void Trade(char *buf, uint16_t offset, uint16_t len)
{
    XdpTrade trade(buf, len, offset);

    printf("tradeTime:%u\n", trade.tradeTime());
    printf("orderbookId:%u\n", trade.orderbookId());
    printf("orderId:%llu\n", trade.orderId());
    printf("tradeId:%llu\n", trade.tradeId());
    printf("comboGroupId:%u\n", trade.comboGroupId());
    printf("price:%d\n", trade.price());
    printf("quantity:%u\n", trade.quantity());
    printf("side:%hhu\n", trade.side());
    printf("dealType:%hhu\n", trade.dealType());
    printf("tradeCondition:%hu\n", trade.tradeCondition());
    printf("dealInfo:%hu\n\n", trade.dealInfo());
}

void CommodityDefinition(char *buf, uint16_t offset, uint16_t len)
{
    XdpCommodityDefinition commodity(buf, len, offset);
    // commodity.commodityId();
    std::cout << "tradeCondition:" << commodity.commodityId() << std::endl;
    printf("commodityCode:%hu\n", commodity.commodityCode());
    std::cout << "commodityName:" << commodity.commodityName() << std::endl;
    printf("orderbookId:%hu\n", commodity.decimalUnderlyingPrice());
    std::cout << "isinCode:" << commodity.isinCode() << std::endl;
    std::cout << "baseCurrency:" << commodity.baseCurrency() << std::endl;
    printf("comboGroupId:%hhu\n", commodity.underlyingPriceUnit());
    printf("price:%lld\n", commodity.nominalValue());
    std::cout << "underlyingCode:" << commodity.underlyingCode() << std::endl;
    printf("side:%hhu\n", commodity.underlyingType());
    printf("dealType:%hhu\n\n", commodity.effectiveTomorrow());
}

void SeriesDefinitionBase(char *buf, uint16_t offset, uint16_t len)
{
    XdpSeriesDefinitionBase sdb(buf, len, offset);

    printf("orderbookId:%u\n", sdb.orderbookId());
    std::cout << "symbol:" << sdb.symbol() << std::endl;
    printf("financialProduct:%hhu\n", sdb.financialProduct());
    printf("numberOfDecimalsPrice:%hu\n", sdb.numberOfDecimalsPrice());
    printf("numberOfLegs:%hhu\n", sdb.numberOfLegs());
    printf("strikePrice:%u\n", sdb.strikePrice());
    std::cout << "expirationDate:" << sdb.expirationDate() << std::endl;
    printf("price:%hhu\n", sdb.putOrCall());
}

void SeriesDefinitionExtended(char *buf, uint16_t offset, uint16_t len)
{
    XdpSeriesDefinitionExtented sde(buf, len, offset);

    printf("orderbookId:%u\n", sde.orderbookId());
    std::cout << "symbol:" << sde.symbol() << std::endl;
    printf("country:%hhu\n", sde.country());
    printf("market:%hhu\n", sde.market());
    printf("instrumentGroup:%hhu\n", sde.instrumentGroup());
    printf("modifier:%hhu\n", sde.modifier());
    printf("commodityCode:%hu\n", sde.commodityCode());
    printf("expirationDate:%hu\n", sde.expirationDate());
    printf("strikePrice:%d\n", sde.strikePrice());
    printf("contractSize:%lld\n", sde.contractSize());
    std::cout << "isinCode:" << sde.isinCode() << std::endl;
    printf("seriesStatus:%hhu\n", sde.seriesStatus());
    printf("effectiveTomorrow:%hhu\n", sde.effectiveTomorrow());
    printf("priceQuotationFactor:%d\n", sde.priceQuotationFactor());
    std::cout << "effectiveExpDate:" << sde.effectiveExpDate() << std::endl;
    printf("dateTimeLastTrading:%lld\n\n", sde.dateTimeLastTrading());
}

void ClassDefinition(char *buf, uint16_t offset, uint16_t len)
{
    XdpClassDefinition cd(buf, len, offset);

    printf("country:%hhu\n", cd.country());
    printf("market:%hhu\n", cd.market());
    printf("instrumentGroup:%hhu\n", cd.instrumentGroup());
    printf("modifier:%hhu\n", cd.modifier());
    printf("commodityCode:%hu\n", cd.commodityCode());
    printf("expirationDate:%d\n", cd.priceQuotationFactor());
    printf("strikePrice:%u\n", cd.contractSize());
    printf("contractSize:%hu\n", cd.decimalInStrikePrice());
    printf("decimalInContractSize:%hu\n", cd.decimalInContractSize());
    printf("decimalInPremium:%hu\n", cd.decimalInPremium());
    printf("rnkingType:%hu\n", cd.rnkingType());
    printf("tradable:%hhu\n", cd.tradable());
    printf("premiumUnit4Price:%hhu\n", cd.premiumUnit4Price());
    std::cout << "baseCurrency:" << cd.baseCurrency() << std::endl;
    std::cout << "instrumentClassID:" << cd.instrumentClassID() << std::endl;
    std::cout << "instrumentClassName:" << cd.instrumentClassName() << std::endl;
    std::cout << "isFractions:" << cd.isFractions() << std::endl;
    std::cout << "settlementCurrencyID:" << cd.settlementCurrencyID() << std::endl;
    printf("market:%hhu\n", cd.effectiveTomorrow());
    printf("instrumentGroup:%d\n\n", cd.tickStepSize());
}