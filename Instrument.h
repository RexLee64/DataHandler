#ifndef INSTRUMENT_H
#define INSTRUMENT_H
#include <iostream>
#include <vector>
#include <string.h>
#include "test.h"
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
    char name[10];
    struct OrderData bid[10];
    struct OrderData offer[10];
};

// 订单簿类
class OrderBook
{
  private:
    uint32_t mOrderBookId;
    struct OrderData mBid[2000];
    struct OrderData mOffer[2000];

  public:
    OrderBook(uint32_t orderBookId = 0) : mOrderBookId(orderBookId)
    {
        memset(this->mBid, 0, sizeof(struct OrderData) * 2000);
        memset(this->mOffer, 0, sizeof(struct OrderData) * 2000);
    }
    uint32_t getOrderBookId()
    {
        return this->mOrderBookId;
    }
    void setOrderBookId(uint32_t orderBookId) { this->mOrderBookId = orderBookId; }
    struct OrderData *getBid()
    {
        return this->mBid;
    }
    struct OrderData *getOffer()
    {
        return this->mOffer;
    }
    //返回修改的位置, -1:未找到
    int insertOrder(struct OrderData order, int orderBookPosition, int side);
    int modifyOrder(struct OrderData order, int orderBookPosition, int side);
    int deleteOrder(uint64_t orderId, int side);
    int changePosition(struct OrderData, int orderBookPosition, int side);
    int increseQuantity(uint64_t orderId, uint32_t quantity, int side);
    int reduceQuantity(struct OrderData order, int side);
    void clearOrder();
    void output();
};

class Instrument
{
  private:
    std::string mSymbol; 
    int mNumberOfDecimalsPrice;
    class OrderBook mOrderBook;

  public:
    Instrument() : mNumberOfDecimalsPrice(0) {}
    Instrument(std::string symbol, int numberOfDecimalsPrice, uint32_t orderBookId)
    {
        this->mSymbol = symbol; 
        this->mNumberOfDecimalsPrice = numberOfDecimalsPrice;
        mOrderBook.setOrderBookId(orderBookId);
    }
    std::string getSymbol() { return this->mSymbol; }
    int getNumberOfDecimalsPrice() { return this->mNumberOfDecimalsPrice; }
    uint32_t getOrderBookId() { return this->mOrderBook.getOrderBookId(); }
    int changeOrderBook(struct OrderData order, int orderBookPosition, int side, int style);
    void getStructOrderBook(struct OrderBookData *orderBook);
    void outputOrderBook();
};

#endif