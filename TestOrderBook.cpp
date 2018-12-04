
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <exception>
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
#include <fstream>
#include "test.h"
#include "OrderbookData.h"
#include "TradeData.h"
#include "Instrument.h"
#include "MsgQueue.h"

#define BUFF_SIZE 4096
#define ADDR "172.21.2.1"
// #define ADDR "127.0.0.1"
#define PORT_1 26666 // 前10档订单簿数据
#define PORT_2 26667 // 逐笔委托增量数据
#define PORT_3 26668 // 逐笔委托快照数据
#define PORT_4 26669 // 逐笔成交增量数据

static const char *g_szIfName = "ens3f1np1";                    // 网卡接口
pthread_mutex_t thread_error_mutex = PTHREAD_MUTEX_INITIALIZER; // 错误文件锁
msgQueue q;                                                     // 消息队列

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

void ProcessMessageHeader(char *buf, int msgCount, uint64_t serverTime, Instrument *ins, int len, int ob_fd, struct sockaddr_in addr1, int ord_fd, struct sockaddr_in addr3, int tr_fd, struct sockaddr_in addr4);
void ProcessSnapshot(char *buf, int msgCount, Instrument *ins, int len, int os_fd, struct sockaddr_in addr2);
std::string trim(std::string s);
void out(struct OrderBookData ob);
void *procData(void *arg);
void *recvData(void *arg);
void out(const struct OrderBookData orderBook);

int main(int argc, char *argv[])
{
    int ret_thrd = -1;
    pthread_t proc_thrd, recv_thrd;

    // 创建处理线程
    ret_thrd = pthread_create(&proc_thrd, NULL, procData, NULL);
    if (ret_thrd == 0)
        printf("处理线程创建成功!\n");
    // 创建接收线程
    ret_thrd = pthread_create(&recv_thrd, NULL, recvData, NULL);
    if (ret_thrd == 0)
        printf("接收线程创建成功!\n");
    pthread_join(proc_thrd, NULL);
    pthread_join(recv_thrd, NULL);

    return 0;
}

void *recvData(void *arg)
{
    int sockfd, n;
    char buf[BUFF_SIZE];
    struct sockaddr_ll sll;
    struct ifreq ifr;
    struct ethhdr *eth;
    struct iphdr *iph;
    struct udphdr *udph;
    sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    if (sockfd == -1)
    {
        printf("sockt error!\n");
        return (void *)-1;
    }
    strncpy(ifr.ifr_name, g_szIfName, IFNAMSIZ);
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1) // 获取网卡信息
    {
        perror("ioctl()");
        close(sockfd);
        return (void *)-1;
    }
    memset(&sll, 0, sizeof(sll));
    sll.sll_family = PF_PACKET;
    sll.sll_ifindex = ifr.ifr_ifindex;
    sll.sll_protocol = htons(ETH_P_IP);
    if (bind(sockfd, (struct sockaddr *)&sll, sizeof(sll)) == -1)
    {
        perror("bind()");
        close(sockfd);
        return (void *)-1;
    }
    try
    {
        while (1)
        {
            n = recvfrom(sockfd, buf, sizeof(buf), 0, NULL, NULL);
            if (n == -1)
            {
                perror("recvfrom()");
                break;
            }
            // 存入消息队列
            EnQueue((msgQueue *)&q, buf, n);
        }
        close(sockfd);
    }
    catch (std::exception &e)
    {
        pthread_mutex_lock(&thread_error_mutex);

        FILE *fp = fopen("thread_error.txt", "a+");
        if (fp != NULL)
        {
            fprintf(fp, "Standard exception: %s\n", e.what());
        }

        fclose(fp);

        pthread_mutex_unlock(&thread_error_mutex);
        close(sockfd);
    }
}

void *procData(void *arg)
{
    // 从消息队列中提取、处理数据
    try
    {
        char buf[BUFF_SIZE];
        int recvLen = 0;
        struct sockaddr_ll sll;
        struct ifreq ifr;
        struct ethhdr *eth;
        struct iphdr *iph;
        struct udphdr *udph;

        int len = 2;
        Instrument insArray[2] = {
            Instrument("HSIZ8", 0, 76484514),
            Instrument("HSIF9", 0, 60755874),
        };
        // Instrument ins("HSIZ8", 0, 76484514);
        uint32_t preNum221 = 0; // channel 221
        uint32_t preNum721 = 0; // channel 721

        // udp
        int ob_fd;  // orderbook socket
        int os_fd;  // order snapshot
        int ord_fd; // order
        int tr_fd;  // trade

        struct sockaddr_in addr1, addr2, addr3, addr4;
        if ((ob_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            perror("socket");
            exit(1);
        }
        if ((os_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            perror("socket");
            exit(1);
        }
        if ((ord_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            perror("socket");
            exit(1);
        }
        if ((tr_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            perror("socket");
            exit(1);
        }
        addr1.sin_family = AF_INET;
        addr1.sin_port = htons(PORT_1);
        addr1.sin_addr.s_addr = inet_addr(ADDR);
        if (addr1.sin_addr.s_addr == INADDR_NONE)
        {
            printf("Incorrect ip address!");
            close(ob_fd);
            exit(1);
        }
        addr2.sin_family = AF_INET;
        addr2.sin_port = htons(PORT_2);
        addr2.sin_addr.s_addr = inet_addr(ADDR);
        if (addr2.sin_addr.s_addr == INADDR_NONE)
        {
            printf("Incorrect ip address!");
            close(ob_fd);
            exit(1);
        }
        addr3.sin_family = AF_INET;
        addr3.sin_port = htons(PORT_3);
        addr3.sin_addr.s_addr = inet_addr(ADDR);
        if (addr2.sin_addr.s_addr == INADDR_NONE)
        {
            printf("Incorrect ip address!");
            close(ob_fd);
            exit(1);
        }
        addr4.sin_family = AF_INET;
        addr4.sin_port = htons(PORT_4);
        addr4.sin_addr.s_addr = inet_addr(ADDR);
        if (addr4.sin_addr.s_addr == INADDR_NONE)
        {
            printf("Incorrect ip address!");
            close(ob_fd);
            exit(1);
        }

        while (1)
        {
            memset(&buf, 0, sizeof(buf));
            // 取消息队列
            int res = DeQueue((msgQueue *)&q, buf, &recvLen);
            if (res != 0)
            {
                usleep(5000);
                continue;
            }

            // 数据处理
            iph = (struct iphdr *)(buf + sizeof(struct ethhdr)); // ip头
            if (iph->protocol == IPPROTO_UDP)
            {
                udph = (struct udphdr *)(buf + sizeof(struct ethhdr) + sizeof(struct iphdr)); // udp头
                char *packetPtr = buf + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr);
                PacketHeader *hdr = (PacketHeader *)(packetPtr); // 数据包头
                char *msgPtr = packetPtr + 16;                   // packet header len = 16bytes
                char *ip = inet_ntoa(*(struct in_addr *)&iph->daddr);
                int port = ntohs(udph->dest);

                if (hdr->mMsgCount > 0)
                {
                    if (strcmp(ip, "239.1.127.130") == 0 && port == 51002)
                    {
                        if (preNum221 == 0 || preNum221 == hdr->mSeqNum)
                        {
                            ProcessMessageHeader(msgPtr, hdr->mMsgCount, hdr->mSendTime, insArray, len, ob_fd, addr1, ord_fd, addr2, tr_fd, addr4);
                            preNum221 = hdr->mSeqNum + hdr->mMsgCount;
                        }
                    }
                    if (strcmp(ip, "239.1.127.155") == 0 && port == 51002)
                    {
                        if (preNum721 == 0 || preNum721 == hdr->mSeqNum)
                        {
                            ProcessSnapshot(msgPtr, hdr->mMsgCount, insArray, len, os_fd, addr3);
                            preNum721 = hdr->mSeqNum + hdr->mMsgCount;
                        }
                    }
                }
            }
        }
        close(ob_fd);
        close(os_fd);
        close(ord_fd);
        close(tr_fd);
    }
    catch (std::exception &e)
    {
        pthread_mutex_lock(&thread_error_mutex);

        FILE *fp = fopen("thread_thread_error.txt", "a+");
        if (fp != NULL)
        {
            fprintf(fp, "Standard exception: %s\n", e.what());
        }

        fclose(fp);

        pthread_mutex_unlock(&thread_error_mutex);
    }
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

void ProcessMessageHeader(char *buf, int msgCount, uint64_t serverTime, Instrument *ins, int len, int ob_fd, struct sockaddr_in addr1, int ord_fd, struct sockaddr_in addr2, int tr_fd, struct sockaddr_in addr4)
{
    int n = 0;
    int size = 0;
    // printf("msgcount %d\n", msgCount);
    // std::ofstream outfile("/app/hkex/file/orderbook.txt", std::ios::app);
    while (n < msgCount)
    {
        MessageHeader *msghdr = (MessageHeader *)buf;
        // printf("msg %d: msgsize=%d, msgtype=%d\n", n, msghdr->mMsgSize, msghdr->mMsgType);
        if (msghdr->mMsgType == ADDORDER)
        {
            XdpAddOrder addOrder(buf, msghdr->mMsgSize, 4);

            if (addOrder.orderbookId() == ins->getOrderBookId())
            {
                struct OrderData order;
                order.orderId = addOrder.orderId();
                order.price = addOrder.price();
                order.quantity = addOrder.quantity();
                order.lotType = addOrder.lotType();
                order.orderType = addOrder.orderType();
                int ret = ins->changeOrderBook(order, addOrder.orderbookPosition(), addOrder.side(), ADDORDER);

                // 发送逐笔委托增量数据
                struct AddOrderData ord;
                strcpy(ord.name, ins->getSymbol().c_str());
                ord.serverTime = serverTime;
                ord.orderId = addOrder.orderId();
                ord.price = addOrder.price();
                ord.side = addOrder.side();
                ord.quantity = addOrder.quantity();
                ord.lotType = addOrder.lotType();
                ord.orderType = addOrder.orderType();
                ord.orderbookPosition = addOrder.orderbookPosition();
                int n = sendto(ord_fd, (char *)&ord, sizeof(ord), 0, (struct sockaddr *)&addr2, sizeof(addr2));
                if (n < 0)
                {
                    perror("ord sendto");
                    close(ord_fd);
                }

                if (ret >= 1 && ret <= 10)
                {
                    struct OrderBookData orderBook;
                    ins->getStructOrderBook(&orderBook);
                    orderBook.serverTime = serverTime;
                    // 发送订单簿
                    int n = sendto(ob_fd, (char *)&orderBook, sizeof(orderBook), 0, (struct sockaddr *)&addr1, sizeof(addr1));
                    if (n < 0)
                    {
                        perror("ob sendto");
                        close(ob_fd);
                    }
                    // 输出订单本
                    // out(orderBook);

                    // 存储订单本数据
                    // outfile << "add," << ret << ","
                    //         << +addOrder.side() << ","
                    //         << addOrder.price() << ","
                    //         << addOrder.quantity() << ","
                    //         << addOrder.orderId()
                    //         << std::endl;
                }
                // 输出逐笔委托
                // printf("add,%d,%d,%d,%u,%lld\n", ret, addOrder.side(), addOrder.price(), addOrder.quantity(), addOrder.orderId());

                // 存储逐笔委托数据
                // std::ofstream addOrderFile("/app/hkex/file/addOrder.txt", std::ios::app);
                // addOrderFile << "orderbookId=" << addOrder.orderbookId()
                //              << ",orderId=" << addOrder.orderId()
                //              << ",side=" << +addOrder.side()
                //              << ",orderbookposition=" << addOrder.orderbookPosition()
                //              << ",price=" << addOrder.price()
                //              << ",quantity=" << addOrder.quantity()
                //              << ",lotType=" << +addOrder.lotType()
                //              << ",orderType=" << addOrder.orderType()
                //              << std::endl;
                // addOrderFile.close();
            }
        }
        if (msghdr->mMsgType == DELETEORDER)
        {
            XdpDeleteOrder deleteOrder(buf, msghdr->mMsgSize, 4);
            if (deleteOrder.orderbookId() == ins->getOrderBookId())
            {
                struct OrderData order;
                order.orderId = deleteOrder.orderId();
                int ret = ins->changeOrderBook(order, 0, deleteOrder.side(), DELETEORDER);

                if (ret >= 1 && ret <= 10)
                {

                    struct OrderBookData orderBook;
                    ins->getStructOrderBook(&orderBook);
                    orderBook.serverTime = serverTime;
                    // 发送订单簿
                    int n = sendto(ob_fd, (char *)&orderBook, sizeof(orderBook), 0, (struct sockaddr *)&addr1, sizeof(addr1));
                    if (n < 0)
                    {
                        perror("ob sendto");
                        close(ob_fd);
                    }

                    // 输出
                    // out(orderBook);
                    // ins->outputOrderBook();

                    // 存储订单本
                    // outfile << "delete," << ret << "," << deleteOrder.orderId() << std::endl;
                }

                // 输出删除订单数据
                // printf("delete,%d,%llu\n", ret, deleteOrder.orderId());
            }
        }
        if (msghdr->mMsgType == MODIFYORDER)
        {
            XdpModifyOrder modifyOrder(buf, msghdr->mMsgSize, 4);
            if (modifyOrder.orderbookId() == ins->getOrderBookId())
            {
                struct OrderData order;
                order.orderId = modifyOrder.orderId();
                order.price = modifyOrder.price();
                order.quantity = modifyOrder.quantity();
                order.orderType = modifyOrder.orderType();
                int ret = ins->changeOrderBook(order, modifyOrder.orderbookPosition(), modifyOrder.side(), MODIFYORDER);

                if (ret >= 1 && ret <= 10)
                {
                    // 发送订单簿
                    struct OrderBookData orderBook;
                    ins->getStructOrderBook(&orderBook);
                    orderBook.serverTime = serverTime;
                    int n = sendto(ob_fd, (char *)&orderBook, sizeof(orderBook), 0, (struct sockaddr *)&addr1, sizeof(addr1));
                    if (n < 0)
                    {
                        perror("ob sendto");
                        close(ob_fd);
                    }

                    // 输出订单簿
                    // out(orderBook);
                    // ins->outputOrderBook();

                    // 存储订单簿
                    // outfile << "modify," << ret << std::endl;
                }
                // printf("modify,%d\n", ret);
            }
        }
        if (msghdr->mMsgType == CLEARORDER)
        {
            XdpClearOrder clearOrder(buf, msghdr->mMsgSize, 4);
            if (clearOrder.orderbookId() == ins->getOrderBookId())
            {
                struct OrderData order;
                ins->changeOrderBook(order, 0, 0, CLEARORDER);
                printf("clear\n");
                // outfile << "clear\n";
            }
        }
        if (msghdr->mMsgType == TRADE)
        {
            XdpTrade trade(buf, msghdr->mMsgSize, 4);
            if (trade.orderbookId() == ins->getOrderBookId())
            {
                // 发送成交数据
                struct TradeData tr;
                strcpy(tr.name, ins->getSymbol().c_str());
                tr.serverTime = serverTime;
                tr.orderId = trade.orderId();
                tr.tradeId = trade.tradeId();
                tr.price = trade.price();
                tr.quantity = trade.quantity();
                tr.comboGroupId = trade.comboGroupId();
                tr.side = trade.side();
                tr.dealType = trade.dealType();
                tr.tradeCondition = trade.tradeCondition();
                tr.dealInfo = trade.dealInfo();
                tr.tradeTime = trade.tradeTime();
                int n = sendto(tr_fd, (char *)&tr, sizeof(tr), 0, (struct sockaddr *)&addr4, sizeof(addr4));
                if (n < 0)
                {
                    perror("tr sendto");
                    close(tr_fd);
                }

                // 订单簿处理
                struct OrderData order;
                order.orderId = trade.orderId();
                order.price = trade.price();
                order.quantity = trade.quantity();
                int ret = ins->changeOrderBook(order, 0, trade.side(), TRADE);

                if (ret >= 1 && ret <= 10)
                {
                    struct OrderBookData orderBook;
                    ins->getStructOrderBook(&orderBook);
                    // 发送订单簿
                    int n = sendto(ob_fd, (char *)&orderBook, sizeof(orderBook), 0, (struct sockaddr *)&addr1, sizeof(addr1));
                    if (n < 0)
                    {
                        perror("ob sendto");
                        close(ob_fd);
                    }

                    // 输出订单簿
                    // out(orderBook);
                    // ins->outputOrderBook();
                    // outfile << "trade," << ret << "," << +trade.side() << "," << trade.price() << "," << trade.quantity() << "," << trade.orderId() << std::endl;

                    //
                }

                // 输出逐笔成交
                // printf("trade,%d,%d,%d,%u,%lld\n", ret, trade.side(), trade.price(), trade.quantity(), trade.orderId());

                // 存储逐笔成交数据
                // std::ofstream tradeFile("/app/hkex/file/trade.txt", std::ios::app);
                // tradeFile << "tradeTime=" << trade.tradeTime()
                //           << ",orderbookId=" << trade.orderbookId()
                //           << ",orderId=" << trade.orderId()
                //           << ",tradeId=" << trade.tradeId()
                //           << ",side=" << +trade.side()
                //           << ",price=" << trade.price()
                //           << ",quantity=" << trade.quantity()
                //           << ",comboGroupId=" << trade.comboGroupId()
                //           << ",dealType=" << +trade.dealType()
                //           << ",tradeCondition=" << trade.tradeCondition()
                //           << ",dealInfo=" << trade.dealInfo()
                //           << std::endl;
                // tradeFile.close();
            }
        }

        n++;
        buf = buf + msghdr->mMsgSize;
        size += msghdr->mMsgSize;
    }
    // outfile.close();
}

void ProcessSnapshot(char *buf, int msgCount, Instrument *ins, int len, int os_fd, struct sockaddr_in addr3)
{
    int n = 0;
    int size = 0;

    while (n < msgCount)
    {
        MessageHeader *msghdr = (MessageHeader *)buf;
        if (msghdr->mMsgType == ADDORDER)
        {
            XdpAddOrder addOrder(buf, msghdr->mMsgSize, 4);

            if (addOrder.orderbookId() == ins->getOrderBookId())
            {
                struct AddOrderData os;
                strcpy(os.name, ins->getSymbol().c_str());
                os.orderId = addOrder.orderId();
                os.price = addOrder.price();
                os.quantity = addOrder.quantity();
                os.lotType = addOrder.lotType();
                os.orderType = addOrder.orderType();
                os.side = addOrder.side();
                os.orderbookPosition = addOrder.orderbookPosition();
                // 发送逐笔委托快照数据
                int n = sendto(os_fd, (char *)&os, sizeof(os), 0, (struct sockaddr *)&addr3, sizeof(addr3));
                if (n < 0)
                {
                    perror("os sendto");
                    close(os_fd);
                }

                // 存储逐笔委托快照数据
                // std::ofstream addSnapFile("/app/hkex/file/addOrder-snapshot.txt", std::ios::app);
                // addSnapFile << "orderbookID:"
                //             << addOrder.orderbookId() << std::endl;
                // addSnapFile << "orderId:"
                //             << addOrder.orderId() << std::endl;
                // addSnapFile << "price:"
                //             << addOrder.price() << std::endl;
                // addSnapFile << "quantity:"
                //             << addOrder.quantity() << std::endl;
                // addSnapFile << "side:"
                //             << +addOrder.side() << std::endl;
                // addSnapFile << "lotType:"
                //             << +addOrder.lotType() << std::endl;
                // addSnapFile << "orderType:"
                //             << addOrder.orderType() << std::endl;
                // addSnapFile << "orderbookPosition:"
                //             << addOrder.orderbookPosition() << std::endl;
                // addSnapFile << "=====================================\n";
                // addSnapFile.close();
            }
        }

        n++;
        buf = buf + msghdr->mMsgSize;
        size += msghdr->mMsgSize;
    }
}

void out(const struct OrderBookData orderBook)
{
    std::ofstream outfile("/app/hkex/file/orderbook.txt", std::ios::app);
    for (int i = 0; i < 10; i++)
    {
        outfile << "pos  " << i + 1;
        outfile << "  " << orderBook.bid[i].orderId << "," << orderBook.bid[i].price << "," << orderBook.bid[i].quantity << " || ";
        outfile << "pos  " << i + 1;
        outfile << "  " << orderBook.offer[i].orderId << "," << orderBook.offer[i].price << "," << orderBook.offer[i].quantity << std::endl;
    }
    outfile << "===================================================================\n";
    outfile.close();
    for (int i = 0; i < 10; i++)
    {
        printf("pos %d  ", i + 1);
        printf("%llu,%d,%u || ", orderBook.bid[i].orderId, orderBook.bid[i].price, orderBook.bid[i].quantity);
        printf("pos %d  ", i + 1);
        printf("%llu,%d,%u\n", orderBook.offer[i].orderId, orderBook.offer[i].price, orderBook.offer[i].quantity);
    }
    printf("===================================================================\n");
}
