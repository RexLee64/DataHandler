#ifndef BASEDTA_H
#define BASEDTA_H
#include <iostream>
#include <string>

class XdpBaseData
{
  protected:
    char *mBuffer;
    uint16_t mBufferLength;
    uint16_t mOffset;

    inline void reset(char *buff, const uint16_t length, const uint16_t offset)
    {
        mBuffer = buff;
        mBufferLength = length;
        mOffset = offset;
    }

  public:
    XdpBaseData() : mBuffer(nullptr), mBufferLength(0), mOffset(0) {}
    XdpBaseData(char *buffer, const uint16_t length, const uint16_t offset = 0)
    {
        reset(buffer, length, offset);
    }
};

#endif