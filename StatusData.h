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
    std::string actualStartDate()
    {
        std::string val(mBuffer + mOffset + 12, 8);
        return val;
    }
    std::string actualStartTime()
    {
        std::string val(mBuffer + mOffset + 20, 6);
        return val;
    }
    std::string plannedStartDate()
    {
        std::string val(mBuffer + mOffset + 26, 8);
        return val;
    }
    std::string plannedStartTime()
    {
        std::string val(mBuffer + mOffset + 34, 6);
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
    std::string suspended()
    {
        std::string val(mBuffer + mOffset + 4, 1);
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
    std::string suspended()
    {
        std::string val(mBuffer + mOffset + 2, 1);
        return val;
    }
};

#endif