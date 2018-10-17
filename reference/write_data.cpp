/*
 * @Author: Rex 
 * @Date: 2018-09-25 10:58:51 
 * @Last Modified by:  
 * @Last Modified time: 2018-09-28 16:42:05
 */
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
#include <time.h>
#define BUFF_SIZE 2048
static const char *g_szIfName = "ens3f1np1"; // 网卡接口

void ProcessMessageHeader(char *buf, int msgCount, int id);
void AddOrder(char *buf, uint16_t offset, uint16_t len);
void Trade(char *buf, uint16_t offset, uint16_t len);
void CommodityDefinition(char *buf, uint16_t offset, uint16_t len);
void SeriesDefinitionExtended(char *buf, uint16_t offset, uint16_t len);
void SeriesDefinitionBase(char *buf, uint16_t offset, uint16_t len);
void ClassDefinition(char *buf, uint16_t offset, uint16_t len);
std::string trim(std::string s);
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
    // UDP_HEADER *udph;

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
        // if (i++ == 204800)
        //     break;
        n = recvfrom(sockfd, buf, sizeof(buf), 0, NULL, NULL);
        if (n == -1)
        {
            perror("recvfrom()");
            break;
        }

        eth = (struct ethhdr *)buf;
        iph = (struct iphdr *)(buf + sizeof(struct ethhdr));
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
                if (strcmp(ip, "239.1.127.128") == 0 && port == 51003)
                {
                    ProcessMessageHeader(msgPtr, hdr->mMsgCount, 1);
                }
                if (strcmp(ip, "239.1.127.153") == 0 && port == 51003)
                {
                    ProcessMessageHeader(msgPtr, hdr->mMsgCount, 2);
                }
            }
        }
    }
    close(sockfd);
    return 0;
}

std::string trim(std::string s)
{

    if (!s.empty())
    {
        s.erase(0, s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
    }

    return s;
}

void ProcessMessageHeader(char *buf, int msgCount, int id)
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

        if (msghdr->mMsgType == COMMODITYDEFINITION)
        {
            CommodityDefinition(buf, 4, msghdr->mMsgSize, id);
        }
        // if (msghdr->mMsgType == CLASSDEFINITION)
        // {
        //     ClassDefinition(buf, 4, msghdr->mMsgSize);
        // }
        if (msghdr->mMsgType == SERIESDEFINITIONEXTENDED)
        {
            SeriesDefinitionExtended(buf, 4, msghdr->mMsgSize, id);
        }
        // if (msghdr->mMsgType == SERIESDEFINITIONBASE)
        // {
        //     SeriesDefinitionBase(buf, 4, msghdr->mMsgSize);
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
    if (AddOrder.orderbookId() == 56102818)
    {
        printf("OrderbookID:%u\n", addOrder.orderbookId());
        printf("OrderID:%llu\n", addOrder.orderId());
        printf("price:%d\n", addOrder.price());
        printf("quantity:%u\n", addOrder.quantity());
        printf("side:%hu\n", addOrder.side());
        printf("lotType:%d\n", addOrder.lotType());
        printf("OrderType:%d\n", addOrder.orderType());
        printf("orderBookPosition:%u\n\n", addOrder.orderbookPosition());
    }
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

void CommodityDefinition(char *buf, uint16_t offset, uint16_t len, int id)
{
    XdpCommodityDefinition commodity(buf, len, offset);
    // commodity.commodityId();
    std::ofstream outfile;
    if (id == 1)
        outfile("real-time-commo.txt", std::ios::app);
    else if (id == 2)
        outfile("snap-commo.txt", std::ios::app);
    if (trim(commodity.commodityName()) == "HANG SENG INDEX")
    {
        time_t timep;
        time(&timep);s
        char *p = ctime(&timep);
        outfile << p << std::endl;
        outfile << "commodityCode:" << commodity.commodityCode() << std::endl;
        outfile << "commodityId:" << commodity.commodityId() << std::endl;
        outfile << "underlyingCode:" << commodity.underlyingCode() << std::endl;
        outfile << "commodityName:" << commodity.commodityName() << std::endl;
        outfile << "isinCode:" << commodity.isinCode() << std::endl;
        outfile << "baseCurrency:" << commodity.baseCurrency() << std::endl;
        outfile << "decimalUnderlyingPrice:" << commodity.decimalUnderlyingPrice() << std::endl;
        outfile << "underlyingPriceUnit:" << +commodity.underlyingPriceUnit() << std::endl;
        outfile << "nominalValue:" << commodity.nominalValue() << std::endl;
        outfile << "underlyingType:" << +commodity.underlyingType() << std::endl;
        outfile << "effectiveTomorrow:" << +commodity.effectiveTomorrow() << std::endl;
        outfile << "===============================================================" << std::endl;
    }
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

void SeriesDefinitionExtended(char *buf, uint16_t offset, uint16_t len, int id)
{
    XdpSeriesDefinitionExtented sde(buf, len, offset);

    // if((sde.commodityCode() == 4002  || sde.commodityCode() == 4010) && sde.instrumentGroup() == 4)
    std::ofstream outfile;
    if (id == 1)
        outfile("real-time-extended.txt", std::ios::app);
    else if (id == 2)
        outfile("snap-extended.txt", std::ios::app);
    if (trim(sde.symbol()) == "HSIV8")
    {
        time_t timep;
        time(&timep);
        char *p = ctime(&timep);
        outfile << p << std::endl;
        outfile << "orderbookId:" << sde.orderbookId() << std::endl;
        outfile << "symbol:" << sde.symbol() << std::endl;
        outfile << "country:" << +sde.country() << std::endl;
        outfile << "market:" << +sde.market() << std::endl;
        outfile << "instrumentGroup:" << +sde.instrumentGroup() << std::endl;
        outfile << "modifier:" << +sde.modifier() << std::endl;
        outfile << "commodityCode:" << sde.commodityCode() << std::endl;
        outfile << "expirationDate:" << sde.expirationDate() << std::endl;
        outfile << "strikePrice:" << sde.strikePrice() << std::endl;
        outfile << "contractSize:" << +sde.contractSize() << std::endl;
        outfile << "isinCode:" << sde.isinCode() << std::endl;
        outfile << "seriesStatus:" << +sde.seriesStatus() << std::endl;
        outfile << "effectiveTomorrow:" << +sde.effectiveTomorrow() << std::endl;
        outfile << "priceQuotationFactor:" << sde.priceQuotationFactor() << std::endl;
        outfile << "effectiveExpDate:" << sde.effectiveExpDate() << std::endl;
        outfile << "dateTimeLastTrading:" << sde.dateTimeLastTrading() << std::endl;
        outfile << "===============================================================" << std::endl;
    }
    outfile.close();
}

void ClassDefinition(char *buf, uint16_t offset, uint16_t len)
{
    XdpClassDefinition cd(buf, len, offset);

    if (cd.commodityCode() == 4002 || cd.commodityCode() == 4010)
    {
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
}