# I2CExtension
I2C extension/wrapper for Arduino

## Usage

```C++
#include <I2CExtension.h>
I2CExtension i2c;

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Wire.begin();
    i2c.attach(Wire);
    i2c.scan();
}

void loop()
{
    // read & write via I2C
}
```

## APIs

```C++
void attach(WireType& w)
void scan()

uint8_t status()

uint8_t readBit(uint8_t dev, uint8_t reg, uint8_t bit)
uint16_t readBitWord(uint8_t dev, uint8_t reg, uint8_t bit)
uint8_t readBits(uint8_t dev, uint8_t reg, uint8_t bit_begin, uint8_t size)
uint16_t readBitsWord(uint8_t dev, uint8_t reg, uint8_t bit_begin, uint8_t size)
uint8_t readByte(uint8_t dev, uint8_t reg)
uint16_t readWord(uint8_t dev, uint8_t reg)
int8_t readBytes(uint8_t dev, uint8_t reg, uint8_t size, uint8_t *data)
int8_t readWords(uint8_t dev, uint8_t reg, uint8_t size, uint16_t *data)

bool writeBit(uint8_t dev, uint8_t reg, uint8_t bit, uint8_t data)
bool writeBitWord(uint8_t dev, uint8_t reg, uint8_t bit, uint16_t data)
bool writeBits(uint8_t dev, uint8_t reg, uint8_t bit_begin, uint8_t size, uint8_t data)
bool writeBitsWord(uint8_t dev, uint8_t reg, uint8_t bit_begin, uint8_t size, uint16_t data)
bool writeByte(uint8_t dev, uint8_t reg, uint8_t data)
bool writeWord(uint8_t dev, uint8_t reg, uint16_t data)
bool writeBytes(uint8_t dev, uint8_t reg, uint8_t size, uint8_t* data)
bool writeWords(uint8_t dev, uint8_t reg, uint8_t size, uint16_t* data)
```

## License

MIT
