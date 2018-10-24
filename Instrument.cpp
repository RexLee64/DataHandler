#include "Instrument.h"

// 0:bid,1:side
void OrderBook::insertOrder(struct OrderData order, int side)
{
    if (order.orderBookPosition <= 1000) // 不处理超过1000档的订单
    {
        if (side == 0)
        {
            for (int i = 1999; i > order.orderBookPosition - 1; i--)
            {
                this->mBid[i].orderId = this->mBid[i - 1].orderId;
                this->mBid[i].price = this->mBid[i - 1].price;
                this->mBid[i].quantity = this->mBid[i - 1].quantity;
                this->mBid[i].lotType = this->mBid[i - 1].lotType;
                this->mBid[i].orderType = this->mBid[i - 1].orderType;
                this->mBid[i].orderBookPosition = this->mBid[i - 1].orderBookPosition;
            }
            this->mBid[order.orderBookPosition - 1].orderId = order.orderId;
            this->mBid[order.orderBookPosition - 1].price = order.price;
            this->mBid[order.orderBookPosition - 1].quantity = order.quantity;
            this->mBid[order.orderBookPosition - 1].lotType = order.lotType;
            this->mBid[order.orderBookPosition - 1].orderType = order.orderType;
            this->mBid[order.orderBookPosition - 1].orderBookPosition = order.orderBookPosition;
        }
        if (side == 1)
        {
            for (int i = 1999; i > order.orderBookPosition - 1; i--)
            {
                this->mOffer[i].orderId = this->mOffer[i - 1].orderId;
                this->mOffer[i].price = this->mOffer[i - 1].price;
                this->mOffer[i].quantity = this->mOffer[i - 1].quantity;
                this->mOffer[i].lotType = this->mOffer[i - 1].lotType;
                this->mOffer[i].orderType = this->mOffer[i - 1].orderType;
                this->mOffer[i].orderBookPosition = this->mOffer[i - 1].orderBookPosition;
            }
            this->mOffer[order.orderBookPosition - 1].orderId = order.orderId;
            this->mOffer[order.orderBookPosition - 1].price = order.price;
            this->mOffer[order.orderBookPosition - 1].quantity = order.quantity;
            this->mOffer[order.orderBookPosition - 1].lotType = order.lotType;
            this->mOffer[order.orderBookPosition - 1].orderType = order.orderType;
            this->mOffer[order.orderBookPosition - 1].orderBookPosition = order.orderBookPosition;
        }
    }
}

// 0:bid,1:side
void OrderBook::deleteOrder(uint64_t orderId, int side)
{
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
                        this->mBid[j].orderBookPosition = this->mBid[j + 1].orderBookPosition;
                    }
                }
            }
        }
        this->mBid[1999].orderId = 0;
        this->mBid[1999].price = 0;
        this->mBid[1999].quantity = 0;
        this->mBid[1999].lotType = 0;
        this->mBid[1999].orderType = 0;
        this->mBid[1999].orderBookPosition = 0;
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
                        this->mOffer[j].orderBookPosition = this->mOffer[j + 1].orderBookPosition;
                    }
                }
            }
        }
        this->mOffer[1999].orderId = 0;
        this->mOffer[1999].price = 0;
        this->mOffer[1999].quantity = 0;
        this->mOffer[1999].lotType = 0;
        this->mOffer[1999].orderType = 0;
        this->mOffer[1999].orderBookPosition = 0;
    }
}

void OrderBook::clearOrder()
{
    memset(mBid, 0, sizeof(struct OrderData) * 2000);
    memset(mOffer, 0, sizeof(struct OrderData) * 2000);
}

void OrderBook::modifyOrder(struct OrderData order, int side)
{
    if (side == 0)
    {
        if (this->mBid[1999].orderId != order.orderId)
        {
            for (int i = 0; i < 1999; i++)
            {
                if (this->mBid[i].orderId == order.orderId)
                {
                    if (this->mBid[i].orderBookPosition == order.orderBookPosition)
                    {
                        this->mBid[i].price = order.price;
                        this->mBid[i].quantity = order.quantity;
                        this->mBid[i].orderType = order.orderType;
                    }
                    else
                    {
                        order.lotType = this->mBid[i].lotType;
                        changePosition(order, side);
                    }
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
                    if (this->mOffer[i].orderBookPosition == order.orderBookPosition)
                    {
                        this->mOffer[i].price = order.price;
                        this->mOffer[i].quantity = order.quantity;
                        this->mOffer[i].orderType = order.orderType;
                    }
                    else
                    {
                        order.lotType = this->mOffer[i].lotType;
                        changePosition(order, side);
                    }
                }
            }
        }
    }
}

// 换位
void OrderBook::changePosition(struct OrderData order, int side)
{
    // 先删除
    deleteOrder(order.orderId, side);
    // 后增加
    insertOrder(order, side);
}

// 比对交易订单
// 查到id, 减少手数，如果为0删除
void OrderBook::reduceQuantity(uint64_t orderId, uint32_t quantity, int side)
{
    if (side == 0)
    {
        if (this->mBid[1999].orderId != orderId)
        {
            for (int i = 0; i < 1999; i++)
            {
                if (this->mBid[i].orderId == orderId)
                {
                    this->mBid[i].quantity -= quantity;
                    if (this->mBid[i].quantity == 0)
                        deleteOrder(orderId, side);
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
                    this->mOffer[i].quantity -= quantity;
                    if (this->mOffer[i].quantity == 0)
                        deleteOrder(orderId, side);
                }
            }
        }
    }
}

void OrderBook::increseQuantity(uint64_t orderId, uint32_t quantity, int side)
{
    if (side == 0)
    {
        if (this->mBid[1999].orderId != orderId)
        {
            for (int i = 0; i < 1999; i++)
            {
                if (this->mBid[i].orderId == orderId)
                {
                    this->mBid[i].quantity += quantity;
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
                }
            }
        }
    }
}

struct OrderBookData Instrument::getStructOrderBook()
{
    struct OrderData *bid = this->mOrderBook.getBid();
    struct OrderData *offer = this->mOrderBook.getOffer();
    struct OrderBookData orderBook;
    strcpy(orderBook.name, this->mSymbol.c_str());

    for (int i = 0; i < 9; i++)
    {
        orderBook.bid[i].orderId = bid[i].orderId;
        orderBook.bid[i].price = bid[i].price;
        orderBook.bid[i].quantity = bid[i].quantity;
        orderBook.bid[i].lotType = bid[i].lotType;
        orderBook.bid[i].orderType = bid[i].orderType;
        orderBook.bid[i].orderBookPosition = bid[i].orderBookPosition;

        orderBook.offer[i].orderId = offer[i].orderId;
        orderBook.offer[i].price = offer[i].price;
        orderBook.offer[i].quantity = offer[i].quantity;
        orderBook.offer[i].lotType = offer[i].lotType;
        orderBook.offer[i].orderType = offer[i].orderType;
        orderBook.offer[i].orderBookPosition = offer[i].orderBookPosition;
    }

    return orderBook;
}