#ifndef ORDERBOOKDATA_H
#define ORDERBOOKDATA_H 
#include "BaseData.h"

class XdpAddModOrder : public XdpBaseData
{
  public:
    XdpAddModOrder() {}
    XdpAddModOrder(char *buffer, const uint16_t length, const uint16_t offset = 0) : XdpBaseData(buffer, length, offset) {}
    // XdpAddModOrder(char *buffer, const uint16_t length, const uint16_t offset)
    // {
    //     reset(buffer, length, 0);
    // }
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
    uint32_t quantity()
    {
        uint32_t val;
        memcpy(&val, mBuffer + mOffset + 16, sizeof(uint32_t));
        return val;
    }
    uint8_t side()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 20, sizeof(uint8_t));
        return val;
    }
    uint8_t lotType()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 21, sizeof(uint8_t));
        return val;
    }
    uint16_t orderType()
    {
        uint16_t val;
        memcpy(&val, mBuffer + mOffset + 22, sizeof(uint16_t));
        return val;
    }
    uint32_t orderbookPosition()
    {
        uint32_t val;
        memcpy(&val, mBuffer + mOffset + 24, sizeof(uint32_t));
        return val;
    }
};

class XdpDeleteOrder : public XdpBaseData
{
  public:
    XdpDeleteOrder() {}
    XdpDeleteOrder(char *buffer, const uint16_t length, const uint16_t offset = 0) : XdpBaseData(buffer, length, offset) {}

    uint32_t orderbookId()
    {
        uint32_t val;
        memcpy(&val, mBuffer + mOffset, sizeof(uint32_t));
        return val;
    }
    uint64_t orderId()
    {
        uint64_t val;
        memcpy(&val, mBuffer + mOffset, sizeof(uint64_t));
        return val;
    }
    uint8_t side()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset, sizeof(uint8_t));
        return val;
    }
};

class XdpClearOrder : public XdpBaseData
{
  public:
    XdpClearOrder() {}
    XdpClearOrder(char *buffer, const uint16_t length, const uint16_t offset = 0) : XdpBaseData(buffer, length, offset) {}

    uint32_t orderbookId()
    {
        uint32_t val;
        memcpy(&val, mBuffer + mOffset, sizeof(uint32_t));
        return val;
    }
};


#endif