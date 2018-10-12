#ifndef BASEDTA_H
#define BASEDTA_H

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
};

#endif