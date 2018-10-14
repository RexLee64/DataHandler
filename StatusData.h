#ifndef STATUSDATA_H
#define STATUSDATA_H
#include "BaseData.h"

class XdpMarketStatus : public XdpBaseData
{
  public:
    XdpMarketStatus() {}
    XdpMarketStatus(char *buffer, const uint16_t length, const uint16_t offset) : XdpBaseData(buffer, length, offset) {}
    uint16_t stateLevcel()
    {
        uint16_t val;
        memcpy(&val, mBuffer + mOffset, sizeof(uint16_t));
        return val;
    }
    uint8_t market()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 2, sizeof(uint8_t));
        return val;
    }
    uint8_t instrument()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 3, sizeof(uint8_t));
        return val;
    }
    uint32_t orderbookID()
    {
        uint32_t val;
        memcpy(&val, mBuffer + mOffset + 4, sizeof(uint32_t));
        return val;
    }
    uint16_t commodityCode()
    {
        uint16_t val;
        memcpy(&val, mBuffer + mOffset + 8, sizeof(uint16_t));
        return val;
    }
    string actualStartDate()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 12, sizeof(string));
        return val;
    }
    string actualStartTime()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 20, sizeof(string));
        return val;
    }
    string plannedStartDate()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 26, sizeof(string));
        return val;
    }
    string plannedStartTime()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 34, sizeof(string));
        return val;
    }
    uint16_t secondsToStateChange()
    {
        uint16_t val;
        memcpy(&val, mBuffer + mOffset + 40, sizeof(uint16_t));
        return val;
    }
    uint16_t state()
    {
        uint16_t val;
        memcpy(&val, mBuffer + mOffset + 42, sizeof(uint16_t));
        return val;
    }
    uint8_t piority()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 44, sizeof(uint8_t));
        return val;
    }
};

class XdpSeriesStatus : public XdpBaseData
{
  public:
    XdpSeriesStatus() {}
    XdpSeriesStatus(char *buffer, const uint16_t length, const uint16_t offset) : XdpBaseData(buffer, length, offset) {}
    uint32_t orderbookID()
    {
        uint32_t val;
        memcpy(&val, mBuffer + mOffset, sizeof(uint32_t));
        return val;
    }
    string suspended()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 4, sizeof(string));
        return val;
    }
};

class XdpCommodityStatus : public XdpBaseData
{
  public:
    XdpCommodityStatus() {}
    XdpCommodityStatus(char *buffer, const uint16_t length, const uint16_t offset) : XdpBaseData(buffer, length, offset) {}
    uint16_t commodityCode()
    {
        uint16_t val;
        memcpy(&val, mBuffer + mOffset, sizeof(uint16_t));
        return val;
    }
    string suspended()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 2, sizeof(string));
        return val;
    }
};

#endif