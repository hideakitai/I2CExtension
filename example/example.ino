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
}
