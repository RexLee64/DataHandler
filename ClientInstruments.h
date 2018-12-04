#ifndef CLIENTINSTRUMENTS_H
#define CLIENTINSTRUMENTS_H
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
#endif