#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <iostream>
#include "MsgQueue.h"
#include "Storage.h"
#define ADDR "172.21.2.1"
#define PORT_1 26666 // 前10档订单簿数据
#define PORT_2 26667 // 逐笔委托增量数据
#define PORT_3 26669 // 逐笔成交增量数据
#define BUFF_SIZE 4096

const char bak_base_path[10] = "bak_data";
pthread_mutex_t thread_error_mutex = PTHREAD_MUTEX_INITIALIZER; // 错误文件锁
msgQueue q;                                                     // 消息队列
char bak_file_path[20];

uint64_t nowtime_ns64();
void *RecvOrder(void *arg);
void *RecvTrade(void *arg);
void *RecvOrderBook(void *arg);
void *ProcData(void *arg);
void substring(char *src, char *dest, int start, int end);
void createDir(char *date, const char *base_path, char *file_path);
void nowtime2str(char *str_time, char *time_type);
int main(int argc, char *argv[])
{

    int ret_thrd = -1;
    pthread_t proc_thrd, recv_thrd1, recv_thrd2, recv_thrd3;

    // 创建文件夹
    char now_date[20];

    nowtime2str(now_date, "%Y%m%d");
    createDir(now_date, bak_base_path, bak_file_path);

    ret_thrd = pthread_create(&recv_thrd1, NULL, RecvOrderBook, NULL);
    if (ret_thrd == 0)
        printf("接收10档行情线程创建成功!\n");
    ret_thrd = pthread_create(&recv_thrd2, NULL, RecvOrder, NULL);
    if (ret_thrd == 0)
        printf("接收逐笔委托增量数据线程创建成功!\n");
    ret_thrd = pthread_create(&recv_thrd3, NULL, RecvTrade, NULL);
    if (ret_thrd == 0)
        printf("接收逐笔成交增量数据线程创建成功!\n");
    ret_thrd = pthread_create(&proc_thrd, NULL, ProcData, NULL);
    if (ret_thrd == 0)
        printf("接收线程创建成功!\n");

    pthread_join(proc_thrd, NULL);
    pthread_join(recv_thrd1, NULL);
    pthread_join(recv_thrd2, NULL);
    pthread_join(recv_thrd3, NULL);
    return 0;
}

// 接收10档行情
void *RecvOrderBook(void *ard)
{
    struct sockaddr_in addr;
    int sock;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_1);
    // addr.sin_addr.s_addr = inet_addr(ADDR);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (addr.sin_addr.s_addr == INADDR_NONE)
    {
        printf("Incorrect ip address!");
        close(sock);
        exit(1);
    }
    int err = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (err < 0)
    {
        perror("bind error!\n");
        exit(1);
    }
    else
    {
        printf("bind!\n");
    }
    char buff[BUFF_SIZE];
    int len = sizeof(addr);
    try
    {
        while (1)
        {
            int n = recvfrom(sock, buff, BUFF_SIZE, 0, (struct sockaddr *)&addr, (socklen_t *)&len);
            if (n > 0)
            {
                // 存入消息队列
                EnQueue((msgQueue *)&q, buff, PORT_1);
            }
        }
        close(sock);
    }
    catch (std::exception &e)
    {
        printf("recv orderbook error!\n");
        close(sock);
    }
}

// 接收逐笔委托增量数据
void *RecvOrder(void *ard)
{
    struct sockaddr_in addr;
    int sock;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_2);
    // addr.sin_addr.s_addr = inet_addr(ADDR);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (addr.sin_addr.s_addr == INADDR_NONE)
    {
        printf("Incorrect ip address!");
        close(sock);
        exit(1);
    }
    int err = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (err < 0)
    {
        perror("bind error!\n");
        exit(1);
    }
    else
    {
        printf("bind!\n");
    }
    char buff[BUFF_SIZE];
    int len = sizeof(addr);
    try
    {
        while (1)
        {
            int n = recvfrom(sock, buff, BUFF_SIZE, 0, (struct sockaddr *)&addr, (socklen_t *)&len);
            if (n > 0)
            {
                // 存入消息队列
                EnQueue((msgQueue *)&q, buff, PORT_2);
            }
        }
        close(sock);
    }
    catch (std::exception &e)
    {
        printf("recv orderbook error!\n");
        close(sock);
    }
}

// 接收逐笔成交增量数据
void *RecvTrade(void *ard)
{
    struct sockaddr_in addr;
    int sock;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_3);
    // addr.sin_addr.s_addr = inet_addr(ADDR);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (addr.sin_addr.s_addr == INADDR_NONE)
    {
        printf("Incorrect ip address!");
        close(sock);
        exit(1);
    }
    int err = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (err < 0)
    {
        perror("bind error!\n");
        exit(1);
    }
    else
    {
        printf("bind!\n");
    }
    char buff[BUFF_SIZE];
    int len = sizeof(addr);
    try
    {
        while (1)
        {
            int n = recvfrom(sock, buff, BUFF_SIZE, 0, (struct sockaddr *)&addr, (socklen_t *)&len);
            if (n > 0)
            {
                // 存入消息队列
                EnQueue((msgQueue *)&q, buff, PORT_3);
            }
        }
        close(sock);
    }
    catch (std::exception &e)
    {
        printf("recv orderbook error!\n");
        close(sock);
    }
}

// 接收并存储数据
void *ProcData(void *arg)
{
    char buff[BUFF_SIZE];
    int id = 0;
    // 客户机时间
    uint64_t clientTime = nowtime_ns64();

    //
    while (1)
    {
        memset(&buff, 0, sizeof(buff));
        // 取消息队列
        int res = DeQueue((msgQueue *)&q, buff, &id);
        if (res != 0)
        {
            usleep(5000);
            continue;
        }
        // 10档
        if (id == PORT_1)
        {
            struct OrderBookData orderbook = *(struct OrderBookData *)buff;
            StorageOrderBook sob;
            sob.Write(bak_file_path, orderbook, clientTime);

            // struct OrderBookData orderbook = *(struct OrderBookData *)buff;
            // printf("%s,%llu,%llu\n", orderbook.name,orderbook.serverTime,clientTime);
            // for (int i = 0; i < 10; i++)
            // {
            //     printf("pos %d  ", i + 1);
            //     printf("%llu,%d,%u || ", orderbook.bid[i].orderId, orderbook.bid[i].price, orderbook.bid[i].quantity);
            //     printf("pos %d  ", i + 1);
            //     printf("%llu,%d,%u\n", orderbook.offer[i].orderId, orderbook.offer[i].price, orderbook.offer[i].quantity);
            // }
            // printf("===================================================================\n");
        }
        // 逐笔委托增量
        if (id == PORT_2)
        {
            struct AddOrderData addOrder = *(struct AddOrderData *)buff;
            StorageOrder so;
            so.Write(bak_file_path, addOrder, clientTime);
            // struct AddOrderData addOrder = *(struct AddOrderData *)buff;
            // printf("add,%llu,%llu:\n%s,%lld,%d,%d,%u,%d,%u,%hu\n", addOrder.serverTime, clientTime, addOrder.name, addOrder.orderId, addOrder.side, addOrder.price, addOrder.quantity, addOrder.lotType, addOrder.orderType, addOrder.orderbookPosition);
            // printf("===================================================================\n");
        }
        // 逐笔成交增量
        if (id == PORT_3)
        {
            struct TradeData tr = *(struct TradeData *)buff;
            StorageTrade st;
            st.Write(bak_file_path, tr, clientTime);

            // struct TradeData tr = *(struct TradeData *)buff;
            // printf("trade,%llu,%llu:\n%s,%llu,%llu,%d,%llu,", tr.serverTime, clientTime, tr.name, tr.orderId, tr.tradeId, tr.price, tr.quantity);
            // printf("%u,%hhu,%hhu,%hu,%hu,%llu\n", tr.comboGroupId, tr.side, tr.dealType, tr.tradeCondition, tr.dealInfo, tr.tradeTime);
            // printf("===================================================================\n");
        }
    }
}

// 当前时间
uint64_t nowtime_ns64()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    uint64_t now_ns64 = ts.tv_sec * 1000000000 + ts.tv_nsec;
    return now_ns64;
}

// 字符串截取,提取从下标为start到end-1的字符串（下标从0开始）
void substring(char *src, char *dest, int start, int end)
{
    int src_len = strlen(src);
    int i;

    if (start > src_len)
        return;
    if (end > src_len)
        end = src_len;

    for (i = start; i < end; i++)
        dest[i - start] = src[i];

    dest[i - start] = '\0';
}
// 文件夹创建
// 创建文件夹 yyyymmdd , base_path: 文件所放目录
void createDir(char *date, const char *base_path, char *file_path)
{

    char year[5], month[3], day[3];
    substring(date, year, 0, 4);
    substring(date, month, 4, 6);
    substring(date, day, 6, 8);

    char path_1[15], path_2[20], path_3[25];
    sprintf(path_1, "%s/%s", base_path, year);
    sprintf(path_2, "%s/%s", path_1, month);
    sprintf(path_3, "%s/%s", path_2, day);

    FILE *fp = fopen("hkex.log", "a+");
    // 创建基础目录
    // if(opendir(base_path) == NULL)
    if (access(path_1, F_OK) != 0)
        if (mkdir(base_path, 0777) == 0)
            fprintf(fp, "create %s\n", base_path);

    if (access(path_1, F_OK) != 0) // 成功执行时，返回0。失败返回-1
    {
        if (mkdir(path_1, 0777) == 0)
            fprintf(fp, "create %s\n", path_1);
    }
    if (access(path_2, F_OK) != 0) // 成功执行时，返回0。失败返回-1
    {
        if (mkdir(path_2, 0777) == 0)
            fprintf(fp, "create %s\n", path_2);
    }
    if (access(path_3, F_OK) != 0) // 成功执行时，返回0。失败返回-1
    {
        if (mkdir(path_3, 0777) == 0)
            fprintf(fp, "create %s\n", path_3);
    }

    sprintf(file_path, "%s", path_3);

    fclose(fp);
}

void nowtime2str(char *str_time, char *time_type)
{
    time_t timer = time(NULL);
    char date_time[20];
    strftime(date_time, sizeof(date_time), time_type, localtime(&timer));
    memcpy(str_time, date_time, sizeof(date_time));
}