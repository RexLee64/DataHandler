#ifndef REFERENCEDATA_H
#define REFERENCEDATA_H
#include "BaseData.h"

class XdpCommodityDefinition : public XdpBaseData
{
  public:
    XdpCommodityDefinition() {}
    XdpCommodityDefinition(char *buffer, const uint16_t length, const uint16_t offset) : XdpBaseData(buffer, length, offset) {}
    uint16_t commodityCode()
    {
        uint16_t val;
        memcpy(&val, mBuffer + mOffset, sizeof(uint16_t));
        return val;
    }
    uint16_t decimalUnderlyingPrice()
    {
        uint16_t val;
        memcpy(&val, mBuffer + mOffset + 2, sizeof(uint16_t));
        return val;
    }
    string isinCode()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 4, sizeof(string));
        return val;
    }
    string baseCurrency()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 16, sizeof(string));
        return val;
    }
    uint8_t underlyingPriceUnit()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 19, sizeof(uint8_t));
        return val;
    }
    string commodityName()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 20, sizeof(string));
        return val;
    }
    int64_t nominalValue()
    {
        int64_t val;
        memcpy(&val, mBuffer + mOffset + 52, sizeof(int64_t));
        return val;
    }
    string underlyingCode()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 60, sizeof(string));
        return val;
    }
    uint8_t underlyingType()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 80, sizeof(uint8_t));
        return val;
    }
    uint8_t effectiveTomorrow()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 81, sizeof(uint8_t));
        return val;
    }
    string commodityId()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 82, sizeof(string));
        return val;
    }
};

class XdpClassDefinition : public XdpBaseData
{
  public:
    XdpClassDefinition() {}
    XdpClassDefinition(char *buffer, const uint16_t length, const uint16_t offset) : XdpBaseData(buffer, length, offset) {}
    uint8_t country()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset, sizeof(uint8_t));
        return val;
    }
    uint8_t market()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 1, sizeof(uint8_t));
        return val;
    }
    uint8_t instrumentGroup()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 2, sizeof(uint8_t));
        return val;
    }
    uint8_t modifier()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 3, sizeof(uint8_t));
        return val;
    }
    uint16_t commodityCode()
    {
        uint16_t val;
        memcpy(&val, mBuffer + mOffset + 4, sizeof(uint16_t));
        return val;
    }
    int32_t priceQuotationFactor()
    {
        int32_t val;
        memcpy(&val, mBuffer + mOffset + 8, sizeof(int32_t));
        return val;
    }
    uint32_t contractSize()
    {
        uint32_t val;
        memcpy(&val, mBuffer + mOffset + 12, sizeof(uint32_t));
        return val;
    }
    uint16_t decimalInStrikePrice()
    {
        uint16_t val;
        memcpy(&val, mBuffer + mOffset + 16, sizeof(uint16_t));
        return val;
    }
    uint16_t decimalInContractSize()
    {
        uint16_t val;
        memcpy(&val, mBuffer + mOffset + 18, sizeof(uint16_t));
        return val;
    }
    uint16_t decimalInPremium()
    {
        uint16_t val;
        memcpy(&val, mBuffer + mOffset + 20, sizeof(uint16_t));
        return val;
    }
    uint16_t rnkingType()
    {
        uint16_t val;
        memcpy(&val, mBuffer + mOffset + 22, sizeof(uint16_t));
        return val;
    }
    uint8_t tradable()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 24, sizeof(uint8_t));
        return val;
    }
    uint8_t premiumUnit4Price()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 25, sizeof(uint8_t));
        return val;
    }
    string baseCurrency()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 26, sizeof(string));
        return val;
    }
    string instrumentClassID()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 29, sizeof(string));
        return val;
    }
    string instrumentClassName()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 43, sizeof(string));
        return val;
    }
    string isFractions()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 75, sizeof(string));
        return val;
    }
    string settlementCurrencyID()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 76, sizeof(string));
        return val;
    }
    uint8_t effectiveTomorrow()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 108, sizeof(uint8_t));
        return val;
    }
    int32_t tickStepSize()
    {
        int32_t val;
        memcpy(&val, mBuffer + mOffset + 109, sizeof(int32_t));
        return val;
    }
};

class XdpSeriesDefinitionBase : public XdpBaseData
{
  public:
    XdpSeriesDefinitionBase() {}
    XdpSeriesDefinitionBase(char *buffer, const uint16_t length, const uint16_t offset) : XdpBaseData(buffer, length, offset) {}
    uint32_t orderbookId()
    {
        uint32_t val;
        memcpy(&val, mBuffer + mOffset, sizeof(uint32_t));
        return val;
    }
    string symbol()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 4, sizeof(string));
        return val;
    }
    uint8_t financialProduct()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 36, sizeof(uint8_t));
        return val;
    }
    uint16_t numberOfDecimalsPrice()
    {
        uint16_t val;
        memcpy(&val, mBuffer + mOffset + 37, sizeof(uint16_t));
        return val;
    }
    uint8_t numberOfLegs()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 39, sizeof(uint8_t));
        return val;
    }
    uint32_t strikePrice()
    {
        uint32_t val;
        memcpy(&val, mBuffer + mOffset + 40, sizeof(uint32_t));
        return val;
    }
    string expirationDate()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 44, sizeof(string));
        return val;
    }
    uint8_t putOrCall()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 54, sizeof(uint8_t));
        return val;
    }
};

class XdpSeriesDefinitionExtented : public XdpBaseData
{
  public:
    XdpSeriesDefinitionExtented() {}
    XdpSeriesDefinitionExtented(char *buffer, const uint16_t length, const uint16_t offset) : XdpBaseData(buffer, length, offset) {}
    uint32_t orderbookID()
    {
        uint32_t val;
        memcpy(&val, mBuffer + mOffset, sizeof(uint32_t));
        return val;
    }
    string symbol()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 4, sizeof(string));
        return val;
    }
    uint8_t country()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 36, sizeof(uint8_t));
        return val;
    }
    uint8_t market()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 37, sizeof(uint8_t));
        return val;
    }
    uint8_t instrumentGroup()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 38, sizeof(uint8_t));
        return val;
    }
    uint8_t modifier()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 39, sizeof(uint8_t));
        return val;
    }
    uint16_t commodityCode()
    {
        uint16_t val;
        memcpy(&val, mBuffer + mOffset + 40, sizeof(uint16_t));
        return val;
    }
    uint16_t expirationDate()
    {
        uint16_t val;
        memcpy(&val, mBuffer + mOffset + 42, sizeof(uint16_t));
        return val;
    }
    int32_t strikePrice()
    {
        int32_t val;
        memcpy(&val, mBuffer + mOffset + 44, sizeof(int32_t));
        return val;
    }
    int64_t contractSize()
    {
        int64_t val;
        memcpy(&val, mBuffer + mOffset + 48, sizeof(int64_t));
        return val;
    }
    string isinCode()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 56, sizeof(string));
        return val;
    }
    uint8_t seriesStatus()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 68, sizeof(uint8_t));
        return val;
    }
    uint8_t effectiveTomorrow()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 69, sizeof(uint8_t));
        return val;
    }
    int32_t priceQuotationFactor()
    {
        int32_t val;
        memcpy(&val, mBuffer + mOffset + 70, sizeof(int32_t));
        return val;
    }
    string effectiveExpDate()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 76, sizeof(string));
        return val;
    }
    int64_t contractSize()
    {
        int64_t val;
        memcpy(&val, mBuffer + mOffset + 84, sizeof(int64_t));
        return val;
    }
};

class XdpCombinationDefinition : public XdpBaseData
{
  public:
    XdpCombinationDefinition() {}
    XdpCombinationDefinition(char *buffer, const uint16_t length, const uint16_t offset) : XdpBaseData(buffer, length, offset) {}
    uint32_t comboOrderbookId()
    {
        uint32_t val;
        memcpy(&val, mBuffer + mOffset, sizeof(uint32_t));
        return val;
    }
    uint32_t legOrderbookId()
    {
        uint32_t val;
        memcpy(&val, mBuffer + mOffset + 4, sizeof(uint32_t));
        return val;
    }
    string legSide()
    {
        string val;
        memcpy(&val, mBuffer + mOffset + 11, sizeof(string));
        return val;
    }
    int32_t legOrderbookId()
    {
        int32_t val;
        memcpy(&val, mBuffer + mOffset + 12, sizeof(int32_t));
        return val;
    }
};

#endif