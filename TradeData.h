#ifndef TRADEDATA_H
#define TRADEDATA_H

#include "BaseData.h"

class XdpTrade : public XdpBaseData
{
  public:
    XdpTrade() {}
    XdpTrade(char *buffer, const uint16_t length, const uint16_t offset = 0) : XdpBaseData(buffer, length, offset) {}

    uint32_t orderbookId()
    {
        uint32_t val;
        memcpy(&val, mBuffer + mOffset, sizeof(uint32_t));
        return val;
    }
    uint64_t orderId()
    {
        uint64_t val;
        memcpy(&val, mBuffer + mOffset + 4, sizeof(uint64_t));
        return val;
    }
    int32_t price()
    {
        int32_t val;
        memcpy(&val, mBuffer + mOffset + 12, sizeof(int32_t));
        return val;
    }
    uint64_t tradeId()
    {
        uint64_t val;
        memcpy(&val, mBuffer + mOffset + 16, sizeof(uint64_t));
        return val;
    }
    uint32_t comboGroupId()
    {
        uint32_t val;
        memcpy(&val, mBuffer + mOffset + 24, sizeof(uint32_t));
        return val;
    }
    uint8_t side()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 28, sizeof(uint8_t));
        return val;
    }
    uint8_t dealType()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 29, sizeof(uint8_t));
        return val;
    }
    uint16_t tradeCondition()
    {
        uint16_t val;
        memcpy(&val, mBuffer + mOffset + 30, sizeof(uint16_t));
        return val;
    }
    uint16_t dealInfo()
    {
        uint16_t val;
        memcpy(&val, mBuffer + mOffset + 32, sizeof(uint16_t));
        return val;
    }
    uint64_t quantity()
    {
        uint64_t val;
        memcpy(&val, mBuffer + mOffset + 36, sizeof(uint64_t));
        return val;
    }
    uint64_t tradeTime()
    {
        uint64_t val;
        memcpy(&val, mBuffer + mOffset + 44, sizeof(uint64_t));
        return val;
    }
};

class XdpTradeAmendment : public XdpBaseData
{
  public:
    XdpTradeAmendment() {}
    XdpTradeAmendment(char *buffer, const uint16_t length, const uint16_t offset = 0) : XdpBaseData(buffer, length, offset) {}
    uint64_t tradeId()
    {
        uint64_t val;
        memcpy(&val, mBuffer + mOffset, sizeof(uint64_t));
        return val;
    }
    uint32_t comboGroupId()
    {
        uint32_t val;
        memcpy(&val, mBuffer + mOffset + 8, sizeof(uint32_t));
        return val;
    }
    int32_t price()
    {
        int32_t val;
        memcpy(&val, mBuffer + mOffset + 12, sizeof(int32_t));
        return val;
    }
    uint64_t quantity()
    {
        uint64_t val;
        memcpy(&val, mBuffer + mOffset + 16, sizeof(uint64_t));
        return val;
    }
    uint64_t tradeTime()
    {
        uint64_t val;
        memcpy(&val, mBuffer + mOffset + 24, sizeof(uint64_t));
        return val;
    }
    uint8_t tradeState()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 32, sizeof(uint8_t));
        return val;
    }
};

class XdpCalculatedOpeningPrice : public XdpBaseData
{
  public:
    XdpCalculatedOpeningPrice() {}
    XdpCalculatedOpeningPrice(char *buffer, const uint16_t length, const uint16_t offset = 0) : XdpBaseData(buffer, length, offset) {}
    uint32_t orderbookId()
    {
        uint32_t val;
        memcpy(&val, mBuffer + mOffset, sizeof(uint32_t));
        return val;
    }
    int32_t calculatedOpeningPrice()
    {
        int32_t val;
        memcpy(&val, mBuffer + mOffset + 4, sizeof(int32_t));
        return val;
    }
    uint64_t quantity()
    {
        uint64_t val;
        memcpy(&val, mBuffer + mOffset + 12, sizeof(uint64_t));
        return val;
    }
};

#endif