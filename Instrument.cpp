#include "Instrument.h"

// 0:bid,1:side
// 返回值：插入的位置（从1开始）
int OrderBook::insertOrder(struct OrderData order, int orderBookPosition, int side)
{

    if (orderBookPosition > 2000)
        return -1;
    if (orderBookPosition <= 2000) // 不处理超过1000档的订单
    {
        if (side == 0)
        {
            for (int i = 1999; i > orderBookPosition - 1; i--)
            {
                this->mBid[i].orderId = this->mBid[i - 1].orderId;
                this->mBid[i].price = this->mBid[i - 1].price;
                this->mBid[i].quantity = this->mBid[i - 1].quantity;
                this->mBid[i].lotType = this->mBid[i - 1].lotType;
                this->mBid[i].orderType = this->mBid[i - 1].orderType;
            }
            this->mBid[orderBookPosition - 1].orderId = order.orderId;
            this->mBid[orderBookPosition - 1].price = order.price;
            this->mBid[orderBookPosition - 1].quantity = order.quantity;
            this->mBid[orderBookPosition - 1].lotType = order.lotType;
            this->mBid[orderBookPosition - 1].orderType = order.orderType;
        }
        if (side == 1)
        {
            for (int i = 1999; i > orderBookPosition - 1; i--)
            {
                this->mOffer[i].orderId = this->mOffer[i - 1].orderId;
                this->mOffer[i].price = this->mOffer[i - 1].price;
                this->mOffer[i].quantity = this->mOffer[i - 1].quantity;
                this->mOffer[i].lotType = this->mOffer[i - 1].lotType;
                this->mOffer[i].orderType = this->mOffer[i - 1].orderType;
            }
            this->mOffer[orderBookPosition - 1].orderId = order.orderId;
            this->mOffer[orderBookPosition - 1].price = order.price;
            this->mOffer[orderBookPosition - 1].quantity = order.quantity;
            this->mOffer[orderBookPosition - 1].lotType = order.lotType;
            this->mOffer[orderBookPosition - 1].orderType = order.orderType;
        }
    }
    return orderBookPosition;
}

// 0:bid,1:side
// 返回值：删除的位置（从1开始）
int OrderBook::deleteOrder(uint64_t orderId, int side)
{
    int pos = -1;
    if (side == 0)
    {
        if (this->mBid[1999].orderId != orderId)
        {
            for (int i = 0; i < 1999; i++)
            {
                if (this->mBid[i].orderId == orderId)
                {
                    for (int j = i; j < 1999; j++)
                    {
                        this->mBid[j].orderId = this->mBid[j + 1].orderId;
                        this->mBid[j].price = this->mBid[j + 1].price;
                        this->mBid[j].quantity = this->mBid[j + 1].quantity;
                        this->mBid[j].lotType = this->mBid[j + 1].lotType;
                        this->mBid[j].orderType = this->mBid[j + 1].orderType;
                    }
                    pos = i;
                    break;
                }
            }
        }
        if (pos != -1)
        {
            this->mBid[1999].orderId = 0;
            this->mBid[1999].price = 0;
            this->mBid[1999].quantity = 0;
            this->mBid[1999].lotType = 0;
            this->mBid[1999].orderType = 0;
        }
    }
    if (side == 1)
    {
        if (this->mBid[1999].orderId != orderId)
        {
            for (int i = 0; i < 1999; i++)
            {
                if (this->mOffer[i].orderId == orderId)
                {
                    for (int j = i; j < 1999; j++)
                    {
                        this->mOffer[j].orderId = this->mOffer[j + 1].orderId;
                        this->mOffer[j].price = this->mOffer[j + 1].price;
                        this->mOffer[j].quantity = this->mOffer[j + 1].quantity;
                        this->mOffer[j].lotType = this->mOffer[j + 1].lotType;
                        this->mOffer[j].orderType = this->mOffer[j + 1].orderType;
                    }
                    pos = i;
                    break;
                }
            }
        }
        if (pos != -1)
        {
            this->mOffer[1999].orderId = 0;
            this->mOffer[1999].price = 0;
            this->mOffer[1999].quantity = 0;
            this->mOffer[1999].lotType = 0;
            this->mOffer[1999].orderType = 0;
        }
    }
    return pos + 1;
}

void OrderBook::clearOrder()
{
    memset(mBid, 0, sizeof(struct OrderData) * 2000);
    memset(mOffer, 0, sizeof(struct OrderData) * 2000);
}

// 返回值：修改的位置（从1开始）
int OrderBook::modifyOrder(struct OrderData order, int orderBookPosition, int side)
{
    int pos = -1;
    if (side == 0)
    {
        if (this->mBid[1999].orderId != order.orderId)
        {
            for (int i = 0; i < 1999; i++)
            {
                if (this->mBid[i].orderId == order.orderId)
                {
                    if (orderBookPosition == i + 1)
                    {
                        this->mBid[i].price = order.price;
                        this->mBid[i].quantity = order.quantity;
                        this->mBid[i].orderType = order.orderType;
                    }
                    else
                    {
                        order.lotType = this->mBid[i].lotType;
                        changePosition(order, orderBookPosition, side);
                    }
                    pos = i;
                    break;
                }
            }
        }
    }
    if (side == 1)
    {
        if (this->mOffer[1999].orderId != order.orderId)
        {
            for (int i = 0; i < 1999; i++)
            {
                if (this->mOffer[i].orderId == order.orderId)
                {
                    if (orderBookPosition == i + 1)
                    {
                        this->mOffer[i].price = order.price;
                        this->mOffer[i].quantity = order.quantity;
                        this->mOffer[i].orderType = order.orderType;
                    }
                    else
                    {
                        order.lotType = this->mOffer[i].lotType;
                        changePosition(order, orderBookPosition, side);
                    }
                    pos = i;
                    break;
                }
            }
        }
    }
    return pos + 1;
}

// 换位
int OrderBook::changePosition(struct OrderData order, int orderBookPosition, int side)
{
    // 先删除
    deleteOrder(order.orderId, side);
    // 后增加
    insertOrder(order, orderBookPosition, side);
}

// 比对交易订单,bid:side=2; offer:side=3
// 查到id, 减少手数，如果为0删除
// 返回值：修改的位置（从1开始）
int OrderBook::reduceQuantity(struct OrderData order, int side)
{
    int pos = -1;
    if (side == 2)
    {
        if (this->mBid[1999].orderId != order.orderId)
        {
            for (int i = 0; i < 1999; i++)
            {
                if (this->mBid[i].orderId == order.orderId)
                {
                    this->mBid[i].quantity -= order.quantity;
                    if (this->mBid[i].quantity == 0)
                        deleteOrder(order.orderId, 0);
                    pos = i;
                    break;
                }
            }
        }
    }
    if (side == 3)
    {
        if (this->mOffer[1999].orderId != order.orderId)
        {
            for (int i = 0; i < 1999; i++)
            {
                if (this->mOffer[i].orderId == order.orderId)
                { 
                    this->mOffer[i].quantity -= order.quantity;
                    if (this->mOffer[i].quantity == 0)
                        deleteOrder(order.orderId, 1);
                    pos = i;
                    break;
                }
            }
        }
    }
    return pos + 1;
}

// 返回值：修改的位置（从1开始）
int OrderBook::increseQuantity(uint64_t orderId, uint32_t quantity, int side)
{
    int pos = -1;
    if (side == 0)
    {
        if (this->mBid[1999].orderId != orderId)
        {
            for (int i = 0; i < 1999; i++)
            {
                if (this->mBid[i].orderId == orderId)
                {
                    this->mBid[i].quantity += quantity;
                    pos = i;
                    break;
                }
            }
        }
    }
    if (side == 1)
    {
        if (this->mOffer[1999].orderId != orderId)
        {
            for (int i = 0; i < 1999; i++)
            {
                if (this->mOffer[i].orderId == orderId)
                {
                    this->mOffer[i].quantity += quantity;
                    pos = i;
                    break;
                }
            }
        }
    }
    return pos + 1;
}

void OrderBook::output()
{
    for (int i = 0; i < 10; i++)
    {
        printf("pos %d  ", i + 1);
        printf("%llu,%d,%u || ", mBid[i].orderId, mBid[i].price, mBid[i].quantity);
        printf("pos %d  ", i + 1);
        printf("%llu,%d,%u\n", mOffer[i].orderId, mOffer[i].price, mOffer[i].quantity);
    }
    printf("===================================================================\n");
}

void Instrument::getStructOrderBook(struct OrderBookData *orderBook)
{
    struct OrderData *bid = this->mOrderBook.getBid();
    struct OrderData *offer = this->mOrderBook.getOffer();
    strcpy(orderBook->name, this->mSymbol.c_str());

    for (int i = 0; i < 10; i++)
    {
        orderBook->bid[i].orderId = bid[i].orderId;
        orderBook->bid[i].price = bid[i].price;
        orderBook->bid[i].quantity = bid[i].quantity;
        orderBook->bid[i].lotType = bid[i].lotType;
        orderBook->bid[i].orderType = bid[i].orderType;

        orderBook->offer[i].orderId = offer[i].orderId;
        orderBook->offer[i].price = offer[i].price;
        orderBook->offer[i].quantity = offer[i].quantity;
        orderBook->offer[i].lotType = offer[i].lotType;
        orderBook->offer[i].orderType = offer[i].orderType;
    }
}

int Instrument::changeOrderBook(struct OrderData order, int orderBookPosition, int side, int style)
{
    int ret;
    if (style == ADDORDER)
    {
        ret = this->mOrderBook.insertOrder(order, orderBookPosition, side);
    }
    if (style == DELETEORDER)
    {
        ret = this->mOrderBook.deleteOrder(order.orderId, side);
    }
    if (style == MODIFYORDER)
    {
        ret = this->mOrderBook.modifyOrder(order, orderBookPosition, side);
    }
    if (style == CLEARORDER)
    {
        this->mOrderBook.clearOrder();
        ret = 1;
    }
    if (style == TRADE)
    {
        ret = this->mOrderBook.reduceQuantity(order, side);
    }
    return ret;
}

void Instrument::outputOrderBook()
{
    mOrderBook.output();
}