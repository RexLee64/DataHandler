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
    std::string isinCode()
    {
        std::string val(mBuffer + mOffset + 4, 12);
        return val;
    }
    std::string baseCurrency()
    {
        std::string val(mBuffer + mOffset + 16, 3);
        return val;
    }
    uint8_t underlyingPriceUnit()
    {
        uint8_t val;
        memcpy(&val, mBuffer + mOffset + 19, sizeof(uint8_t));
        return val;
    }
    std::string commodityName()
    {
        std::string val(mBuffer + mOffset + 20, 32);
        return val;
    }
    int64_t nominalValue()
    {
        int64_t val;
        memcpy(&val, mBuffer + mOffset + 52, sizeof(int64_t));
        return val;
    }
    std::string underlyingCode()
    {
        std::string val(mBuffer + mOffset + 60, 20);
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
    std::string commodityId()
    {
        std::string val(mBuffer + mOffset + 82, 6);
        // printf("%d,%s\n",mOffset,mBuffer + mOffset + 82);
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
    std::string baseCurrency()
    {
        std::string val(mBuffer + mOffset + 26, 3);
        return val;
    }
    std::string instrumentClassID()
    {
        std::string val(mBuffer + mOffset + 29, 14);
        return val;
    }
    std::string instrumentClassName()
    {
        std::string val(mBuffer + mOffset + 43, 32);
        return val;
    }
    std::string isFractions()
    {
        std::string val(mBuffer + mOffset + 75, 1);
        return val;
    }
    std::string settlementCurrencyID()
    {
        std::string val(mBuffer + mOffset + 76, 32);
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
    std::string symbol()
    {
        std::string val(mBuffer + mOffset + 4, 32);
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
    std::string expirationDate()
    {
        std::string val(mBuffer + mOffset + 44, 8);
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
    uint32_t orderbookId()
    {
        uint32_t val;
        memcpy(&val, mBuffer + mOffset, sizeof(uint32_t));
        return val;
    }
    std::string symbol()
    {
        std::string val(mBuffer + mOffset + 4, 32);
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
    std::string isinCode()
    {
        std::string val(mBuffer + mOffset + 56, 12);
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
    std::string effectiveExpDate()
    {
        std::string val(mBuffer + mOffset + 76, 8);
        return val;
    }
    int64_t dateTimeLastTrading()
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
    std::string legSide()
    {
        std::string val(mBuffer + mOffset + 11, 1);
        return val;
    }
    int32_t legRatio()
    {
        int32_t val;
        memcpy(&val, mBuffer + mOffset + 12, sizeof(int32_t));
        return val;
    }
};

#endif