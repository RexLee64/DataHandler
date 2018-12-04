#ifndef STORAGE_H
#define STORAGE_H

#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ClientInstruments.h"

class Storage
{
  protected:
    std::ofstream out;
    std::string filename;

  public:
    Storage() {}
    void Open(std::string filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
    {
        this->filename = filename;
        this->out.open(filename, mode);
    }
    bool IsNull()
    {
        bool ret = false;
        std::ifstream in(this->filename);
        in.seekg(0, std::ios::end);
        std::streampos fp = in.tellg();
        if (int(fp) == 0)
            ret = true;
        in.close();

        return ret;
    }
    void Close() { this->out.close(); }
    // virtual int Write() = 0;
};

// 存储10档行情增量数据
class StorageOrderBook : public Storage
{
  public:
    StorageOrderBook() {}
    int Write(char *path, struct OrderBookData orderBook, uint64_t clientTime)
    {
        char filename[40];
        sprintf(filename, "%s/%s_orderbook.csv", path, orderBook.name);
        this->Open(filename, std::ios::app);
        // 初次创建文件，写入头
        if (IsNull())
        {
            out << "Symbol"
                << ",";
            for (int i = 1; i < 10; i++)
            {
                out << "BuyOrderId0" << i << ","
                    << "BuyPrice0" << i << ","
                    << "BuyQuantity0" << i << ","
                    << "BuyLotType0" << i << ","
                    << "BuyOrderType0" << i << ",";
            }
            out << "BuyOrderId10"
                << ","
                << "BuyPrice10"
                << ","
                << "BuyQuantity10"
                << ","
                << "BuyLotType10"
                << ","
                << "BuyOrderType10"
                << ",";
            for (int i = 1; i < 11; i++)
            {
                out << "SellOrderId0" << i << ","
                    << "SellPrice0" << i << ","
                    << "SellQuantity0" << i << ","
                    << "SellLotType0" << i << ","
                    << "SellOrderType0" << i << ",";
            }
            out << "SellOrderId10"
                << ","
                << "SellPrice10"
                << ","
                << "SellQuantity10"
                << ","
                << "SellLotType10"
                << ","
                << "SellOrderType10"
                << ",";
            out << "ServerTime"
                << ","
                << "ClientTime" << std::endl;
            out << "ServerTime"
                << ","
                << "ClientTime\n";
        }

        out << orderBook.name << ",";
        for (int i = 0; i < 10; i++)
        {
            out << orderBook.bid[i].orderId << ","
                << orderBook.bid[i].price << ","
                << orderBook.bid[i].quantity << ","
                << +orderBook.bid[i].lotType << ","
                << orderBook.bid[i].orderType << ",";
        }
        for (int i = 0; i < 10; i++)
        {
            out << orderBook.offer[i].orderId << ","
                << orderBook.offer[i].price << ","
                << orderBook.offer[i].quantity << ","
                << +orderBook.offer[i].lotType << ","
                << orderBook.offer[i].orderType << ",";
        }
        out << orderBook.serverTime << ","
            << clientTime << std::endl;
        this->Close();
    }
};

// 存储逐笔委托增量数据
class StorageOrder : public Storage
{
  public:
    StorageOrder() {}
    int Write(char *path, struct AddOrderData order, uint64_t clientTime)
    {
        char filename[40];
        sprintf(filename, "%s/%s_order.csv", path, order.name);
        this->Open(filename, std::ios::app);
        // 初次创建文件，写入头
        if (IsNull())
        {
            out << "Symbol"
                << ","
                << "OrderId"
                << ","
                << "Price"
                << ","
                << "Quantity"
                << ","
                << "side"
                << ","
                << "LotType"
                << ","
                << "OrderType"
                << ","
                << "OrderBookPosition"
                << ","
                << "ServerTime"
                << ","
                << "ClientTime\n";
        }
        out << order.name << ","
            << order.orderId << ","
            << order.price << ","
            << order.quantity << ","
            << +order.side << ","
            << +order.lotType << ","
            << order.orderType << ","
            << order.orderbookPosition << ","
            << order.serverTime << ","
            << clientTime << std::endl;
        this->Close();
    }
};

// 存储逐笔成交增量数据
class StorageTrade : public Storage
{
  public:
    StorageTrade() {}
    int Write(char *path, struct TradeData trade, uint64_t clientTime)
    {
        char filename[40];
        sprintf(filename, "%s/%s_trade.csv", path, trade.name);
        this->Open(filename, std::ios::app);
        // 初次创建文件，写入头
        if (IsNull())
        {
            out << "Symbol"
                << ","
                << "OrderId"
                << ","
                << "TradeId"
                << ","
                << "TradeTime"
                << ","
                << "Price"
                << ","
                << "Quantity"
                << ","
                << "Side"
                << ","
                << "ComboGroupId"
                << ","
                << "TradeCondition"
                << ","
                << "DealType"
                << ","
                << "DealInfo"
                << ","
                << "ServerTime"
                << ","
                << "ClientTime\n";
        }
        out << trade.name << ","
            << trade.orderId << ","
            << trade.tradeId << ","
            << trade.tradeTime
            << trade.price << ","
            << trade.quantity << ","
            << +trade.side << ","
            << trade.comboGroupId << ","
            << trade.tradeCondition << ","
            << +trade.dealType << ","
            << trade.dealInfo << ","
            << trade.serverTime << ","
            << clientTime << std::endl;
        this->Close();
    }
};

#endif