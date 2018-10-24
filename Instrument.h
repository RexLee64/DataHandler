#ifndef INSTRUMENT_H
#define INSTRUMENT_H
#include <iostream>
#include <vector>

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
struct OrderBook
{
    char name[10];
    uint64_t transactTime;
    struct OrderData bid[10];
    struct OrderData offer[10];
};


// 订单簿类
class OrderBook
{
  private:
    uint32_t mOrderBookId;
    struct OrderData mBid;
    struct OrderData mOffer;

  public:
    OrderBook() : mOrderBookId(0) {}
    OrderBook(uint32_t orderBookId)
    {
        this->mOrderBookId = orderBookId;
    }
    uint32_t getOrderBookId()
    {
        return this->mOrderBookId;
    }
    struct OrderData getBid()
    {
        return this->mBid;
    }
    struct OrderData getOffer()
    {
        return this->mOffer;
    }
    void insertOrder(struct OrderData order);
    void modifyOrder(struct OrderData order);
    void deleteOrder(uint64_t orderId);
    void clearOrder(uint32_t orderbookId);
    void
};

#endif