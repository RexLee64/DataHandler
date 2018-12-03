#ifndef STORAGE_H
#define STORAGE_H

#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Instrument.h"

class Storage
{
  protected:
    std::ofstream out;
    std::string filename;

  public:
    Storage() {}
    void Open(std::string filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
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
    void Close() { out.close(); }
    // virtual int Write() = 0;
};

// 存储10档行情增量数据
class StorageOrderBook : public Storage
{
  public:
    StorageOrderBook() {}
    int Write(struct OrderBookData orderBook, uint64_t clientTime)
    {
        // 初次创建文件，写入头
        if (IsNull())
        {
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
    }
};

// 存储逐笔委托增量数据
class StorageOrder : public Storage
{
  public:
    StorageOrder() {}
    int Write(struct AddOrderData order, uint64_t clientTime)
    {
        // 初次创建文件，写入头
        if (IsNull())
        {
            
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
    }
};

// 存储逐笔成交增量数据
class StorageTrade : public Storage
{
  public:
    StorageTrade() {}
    int Write(struct TradeData trade, uint64_t clientTime)
    {
        // 初次创建文件，写入头
        if (IsNull())
        {
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
    }
};

// 文件夹创建
// 创建文件夹 yyyymmdd , base_path: 文件所放目录
void createDir(char *date, const char *base_path, char *file_path, char *log_file = "cme.log")
{
    char year[5], month[3], day[3];
    substring(date, year, 0, 4);
    substring(date, month, 4, 6);
    substring(date, day, 6, 8);

    char path_1[15], path_2[20], path_3[25];
    sprintf(path_1, "%s/%s", base_path, year);
    sprintf(path_2, "%s/%s", path_1, month);
    sprintf(path_3, "%s/%s", path_2, day);

    FILE *fp = fopen(log_file, "a+");

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

#endif