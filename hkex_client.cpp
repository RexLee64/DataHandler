#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 26666
#define ADDR "172.21.12.11"
// #define ADDR "127.0.0.1"
#define SIZE 1024

// 订单数据定义
struct OrderData
{
    uint64_t orderId;
    int32_t price;
    uint32_t quantity;
    uint8_t lotType;    //0:Undefined, 1:Odd Lot, 2:Round Lot, 3:Block Lot, 4:All or None Lot
    uint16_t orderType; //0:Not applicable, 1:Force, 2:Short Sell, 4:Market Bid, 8:Price Stabilization, 16:Override Crossing
                        //32:Undisclosed, 1024:Fill-and-kill immediately, 2048:Firm color disabled, 4096:Convert to aggressive (if locked market), 8192:Bait/implied order
};

// 订单簿定义
struct OrderBookData
{
    char name[20];
    uint64_t serverTime;
    struct OrderData bid[10];
    struct OrderData offer[10];
};

// 逐笔委托数据（增量数据和快照数据通用）
struct AddOrderData
{
    char name[20];
    uint64_t serverTime;
    uint64_t orderId;
    int32_t price;
    uint32_t quantity;
    uint8_t side;       // 0:bid, 1:offer
    uint8_t lotType;    //0:Undefined, 1:Odd Lot, 2:Round Lot, 3:Block Lot, 4:All or None Lot
    uint32_t orderType; //0:Not applicable, 1:Force, 2:Short Sell, 4:Market Bid, 8:Price Stabilization, 16:Override Crossing
                        //32:Undisclosed, 1024:Fill-and-kill immediately, 2048:Firm color disabled, 4096:Convert to aggressive (if locked market), 8192:Bait/implied order
    uint16_t orderbookPosition;
};

// 逐笔成交数据
struct TradeData
{
    char name[20];
    uint64_t serverTime;
    uint64_t orderId; // 0: If Not Available
    int32_t price;
    uint64_t tradeId;
    uint32_t comboGroupId;
    uint8_t side;            //0:Not Available, 1:Not Defined, 2:Buy Order, 3:Sell Order
    uint8_t dealType;        //0:None, 1:Printable (see note), 2:Occurred at Cross, 4:Reported Trade
    uint16_t tradeCondition; //0:None, 1:Late Trade, 2:Internal Trade / Crossing, 8 Buy Write, 16:Off Market
    uint16_t dealInfo;       //0:None, 1:Reported Trade
    uint64_t quantity;
    uint64_t tradeTime; //Date and time of the last trade in UTC timestamp
                        //(nanoseconds since 1970) precision to the nearest 1/100th second
};

int main(int argc, char *argv[])
{
    struct sockaddr_in addr;
    int sock;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(26667);
    // addr.sin_addr.s_addr = inet_addr(ADDR);
    addr.sin_addr.s_addr=htonl(INADDR_ANY);

    if (addr.sin_addr.s_addr == INADDR_NONE)
    {
        printf("Incorrect ip address!");
        close(sock);
        return -1;
    }
    int err = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	if(err < 0)
	{
		perror("bind error!\n");
		return -1;
	}
	else
	{
		printf("bind!\n");
	}
    char buff[SIZE];
    int len = sizeof(addr);

    while (1)
    {
        int n = recvfrom(sock, buff, SIZE, 0, (struct sockaddr *)&addr, (socklen_t *)&len); 
        if (n > 0)
        {
            // struct OrderBookData orderbook = *(struct OrderBookData*)buff;
            // printf("%s\n", orderbook.name);
            // for (int i = 0; i < 10; i++)
            // {    
            //     printf("pos %d  ", i + 1);
            //     printf("%llu,%d,%u || ", orderbook.bid[i].orderId, orderbook.bid[i].price, orderbook.bid[i].quantity);
            //     printf("pos %d  ", i + 1);
            //     printf("%llu,%d,%u\n", orderbook.offer[i].orderId, orderbook.offer[i].price, orderbook.offer[i].quantity);
            // }
            // printf("===================================================================\n");
            //buff[n] = 0;

            struct AddOrderData addOrder = *(struct AddOrderData*)buff;
            printf("%llu:add,%s,%lld,%d,%d,%u,%d,%u,%hu\n",addOrder.serverTime,addOrder.name, addOrder.orderId,addOrder.side, addOrder.price, addOrder.quantity, addOrder.lotType,addOrder.orderType,addOrder.orderbookPosition);
            
            // struct TradeData tr = *(struct TradeData*)buff; 
            // printf("trade-%s,%llu,%llu,%d,%llu,",tr.name,tr.orderId,tr.tradeId,tr.price,tr.quantity);
            // printf("%u,%hhu,%hhu,%hu,%hu,%llu\n",tr.comboGroupId,tr.side,tr.dealType,tr.tradeCondition,tr.dealInfo,tr.tradeTime);
        }
        else if (n == 0)
        {
            printf("server closed\n");
            close(sock);
            break;
        }
        else if (n == -1)
        {
            perror("recvfrom");
            close(sock);
            break;
        } 
 
    }
    close(sock);
    
    return 0;
}