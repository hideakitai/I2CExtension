#pragma once
#ifndef I2CEXTENSION_H
#define I2CEXTENSION_H

#ifdef TEENSYDUINO
#include <i2c_t3.h>
#else
#include <Wire.h>
#endif

template <typename WireType>
class I2CExtension_
{
public:

    void attach(WireType& w) { wire = &w; }

    void scan() const
    {
        uint8_t count = 0;
        for (uint8_t i = 8; i < 120; ++i)
        {
            wire->beginTransmission (i);
            if (wire->endTransmission () == 0)
            {
                Serial.print("addr : 0x"); Serial.println(i, HEX);
                count++;
                delay(1);
            }
        }
        Serial.print("found ");
        Serial.print(count, DEC);
        Serial.println(" devices");
    }

    uint8_t status() const { return status_; }

    uint8_t readBit(uint8_t dev, uint8_t reg, uint8_t bit)
    {
        uint8_t b = readByte(dev, reg);
        b &= (1 << bit);
        return b;
    }

    uint16_t readBitWord(uint8_t dev, uint8_t reg, uint8_t bit)
    {
        uint16_t b = readWord(dev, reg);
        b &= (1 << bit);
        return b;
    }

    uint8_t readBits(uint8_t dev, uint8_t reg, uint8_t bit_begin, uint8_t size)
    {
        uint8_t b = readByte(dev, reg);
        uint8_t mask = ((1 << size) - 1) << (bit_begin - size + 1);
        b &= mask;
        b >>= (bit_begin - size + 1);
        return b;
    }

    uint16_t readBitsWord(uint8_t dev, uint8_t reg, uint8_t bit_begin, uint8_t size)
    {
        uint16_t w = readWord(dev, reg);
        uint16_t mask = ((1 << size) - 1) << (bit_begin - size + 1);
        w &= mask;
        w >>= (bit_begin - size + 1);
        return w;
    }

    uint8_t readByte(uint8_t dev, uint8_t reg)
    {
        uint8_t data;
        readBytes(dev, reg, 1, data);
        return data;
    }

    uint16_t readWord(uint8_t dev, uint8_t reg)
    {
        uint16_t data;
        readWords(dev, reg, 1, data);
        return data;
    }

    int8_t readBytes(uint8_t dev, uint8_t reg, uint8_t size, uint8_t *data)
    {
        wire->beginTransmission(dev);
        wire->write(reg);
        wire->endTransmission();
        wire->requestFrom(dev, size);
        int8_t count = 0;
        while (wire->available()) data[count++] = wire->read();
        return count;
    }

    int8_t readWords(uint8_t dev, uint8_t reg, uint8_t size, uint16_t *data)
    {
        wire->beginTransmission(dev);
        wire->write(reg);
        wire->endTransmission();
        wire->requestFrom(dev, (uint8_t)(size * 2));
        int8_t count = 0;
        bool msb = true;
        while (wire->available() && (count < size))
        {
            if (msb) data[count]    = wire->read() << 8;
            else     data[count++] |= wire->read();
            msb = !msb;
        }
        return count;
    }


    bool writeBit(uint8_t dev, uint8_t reg, uint8_t bit, uint8_t data)
    {
        uint8_t b = readByte(dev, reg);
        b = (data != 0) ? (b | (1 << bit)) : (b & ~(1 << bit));
        return writeByte(dev, reg, b);
    }

    bool writeBitWord(uint8_t dev, uint8_t reg, uint8_t bit, uint16_t data)
    {
        uint16_t w = readWord(dev, reg);
        w = (data != 0) ? (w | (1 << bit)) : (w & ~(1 << bit));
        return writeWord(dev, reg, w);
    }

    bool writeBits(uint8_t dev, uint8_t reg, uint8_t bit_begin, uint8_t size, uint8_t data)
    {
        uint8_t b = readByte(dev, reg)
        uint8_t mask = ((1 << size) - 1) << (bit_begin - size + 1);
        data <<= (bit_begin - size + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        b &= ~(mask); // zero all important bits in existing byte
        b |= data; // combine data with existing byte
        return writeByte(dev, reg, b);
    }

    bool writeBitsWord(uint8_t dev, uint8_t reg, uint8_t bit_begin, uint8_t size, uint16_t data)
    {
        uint16_t w = readWord(dev, reg);
        uint16_t mask = ((1 << size) - 1) << (bit_begin - size + 1);
        data <<= (bit_begin - size + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        w &= ~(mask); // zero all important bits in existing word
        w |= data; // combine data with existing word
        return writeWord(dev, reg, w);
    }

    bool writeByte(uint8_t dev, uint8_t reg, uint8_t data)
    {
        return writeBytes(dev, reg, 1, &data);
    }

    bool writeWord(uint8_t dev, uint8_t reg, uint16_t data)
    {
        return writeWords(dev, reg, 1, &data);
    }

    bool writeBytes(uint8_t dev, uint8_t reg, uint8_t size, uint8_t* data)
    {
        wire->beginTransmission(dev);
        wire->write(reg);
        for (uint8_t i = 0; i < size; i++)
            wire->write(data[i]);
        status_ = wire->endTransmission();
        return (status_ == 0);
    }

    bool writeWords(uint8_t dev, uint8_t reg, uint8_t size, uint16_t* data)
    {
        wire->beginTransmission(dev);
        wire->write(reg);
        for (uint8_t i = 0; i < size; i++)
        {
            wire->write((uint8_t)((data[i] >> 8) & 0x00FF));
            wire->write((uint8_t)((data[i] >> 0) & 0x00FF));
        }
        status_ = wire->endTransmission();
        return (status_ == 0);
    };

private:

    WireType* wire;
    uint8_t status_;
};

#ifdef TEENSYDUINO
using I2CExtension = I2CExtension_<i2c_t3>;
#else
using I2CExtension = I2CExtension_<TwoWire>;
#endif

#endif // I2CEXTENSION_H
